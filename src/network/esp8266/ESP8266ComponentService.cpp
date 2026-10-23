#if defined(ESP8266)
#include "ESP8266ComponentService.hpp"
#include <cmath>
#include <cstdlib>

namespace ArduinoManager { namespace Network {
  ESP8266ComponentService::ESP8266ComponentService(const WifiResourceSettings &settings)
      : _server(settings.localResourcePort), _client(settings), _running(false), _retryAt(0) {
    _server.on("/status", HTTP_GET, [this]() {
      for (auto &component : _components) sample(component, millis(), false);
      _server.send(200, "application/json", statusJson(false));
    });
    _server.onNotFound([this]() {
      const String uri = _server.uri();
      for (auto &component : _components) for (auto &channel : component.channels) {
        if (uri != "/" + channel.attr) continue;
        if (_server.method() != HTTP_POST || !channel.write) {
          _server.send(405, "text/plain", "Read-only component"); return;
        }
        String text = _server.arg("value");
        char *end = nullptr;
        double value = strtod(text.c_str(), &end);
        if (text.length() == 0 || end == text.c_str() || *end != '\0' ||
            !std::isfinite(value) || !channel.write(value)) {
          _server.send(400, "text/plain", "Invalid value"); return;
        }
        sample(component, millis(), true);
        _server.send(200, "text/plain", "Value updated"); return;
      }
      _server.send(404, "text/plain", "Endpoint not found");
    });
  }

  bool ESP8266ComponentService::addComponent(const String &attr, ComponentUpdatePolicy policy,
      uint32_t intervalSeconds, Reader read, Writer write) {
    if (!read) return false;
    return addCompositeComponent(attr, {{attr, write}}, policy, intervalSeconds,
        [read](std::vector<double> &values) { values[0] = read(); return true; });
  }

  bool ESP8266ComponentService::validAttr(const String &attr) {
    if (attr.length() == 0 || attr.length() > 64 ||
        attr == "status" || attr == "getCredentials") return false;
    for (size_t i = 0; i < attr.length(); ++i) {
      char c = attr[i];
      if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
            (c >= '0' && c <= '9') || c == '_' || c == '-')) return false;
    }
    return true;
  }

  bool ESP8266ComponentService::addCompositeComponent(const String &attr,
      const std::vector<Channel> &channels, ComponentUpdatePolicy policy,
      uint32_t intervalSeconds, JointReader read, uint32_t minimumReadIntervalMs) {
    if (_running || !read || !validAttr(attr) || _components.size() >= 16 ||
        channels.empty() || channels.size() > 16 ||
        (policy != ComponentUpdatePolicy::PERIODIC && policy != ComponentUpdatePolicy::ON_CHANGE) ||
        (policy == ComponentUpdatePolicy::PERIODIC &&
         (intervalSeconds < 1 || intervalSeconds > 86400 ||
          intervalSeconds * 1000UL < minimumReadIntervalMs)) ||
        (policy == ComponentUpdatePolicy::ON_CHANGE && intervalSeconds != 0)) return false;
    for (const auto &component : _components) if (component.attr == attr) return false;
    for (size_t i = 0; i < channels.size(); ++i) {
      if (!validAttr(channels[i].attr)) return false;
      for (size_t j = 0; j < i; ++j)
        if (channels[j].attr == channels[i].attr) return false;
      for (const auto &component : _components) for (const auto &channel : component.channels)
        if (channel.attr == channels[i].attr) return false;
    }
    _components.push_back({attr, policy, intervalSeconds, read, channels,
        std::vector<double>(channels.size(), NAN), minimumReadIntervalMs,
        false, false, true, 0});
    return true;
  }

  bool ESP8266ComponentService::addDHT11Component(const String &attr,
      const String &temperatureAttr, const String &humidityAttr,
      ComponentUpdatePolicy policy, uint32_t intervalSeconds, Components::DHT11Component &dht) {
    return addCompositeComponent(attr, {{temperatureAttr, Writer()}, {humidityAttr, Writer()}},
        policy, intervalSeconds, [&dht](std::vector<double> &values) {
          float temperature, humidity;
          bool healthy = dht.readSample(temperature, humidity);
          values[0] = temperature; values[1] = humidity;
          return healthy;
        }, 2000);
  }

  void ESP8266ComponentService::begin() {
    if (_running || _components.empty()) return;
    // Preserve acquisition cadence across reconnects; re-publish the complete snapshot.
    for (auto &component : _components) component.pending = true;
    _retryAt = millis();
    _server.begin(); _running = true;
  }
  void ESP8266ComponentService::stop() {
    if (_running) _server.stop();
    _running = false;
  }
  void ESP8266ComponentService::sample(Component &component, uint32_t now, bool force) {
    const uint32_t elapsed = now - component.sampledAt;
    if (component.sampled && (elapsed < component.minimumReadIntervalMs ||
        (!force && component.policy == ComponentUpdatePolicy::PERIODIC &&
         elapsed < component.intervalSeconds * 1000UL))) return;
    std::vector<double> values(component.channels.size(), NAN);
    bool healthy = component.read(values);
    healthy = healthy && values.size() == component.channels.size();
    for (double value : values) healthy = healthy && std::isfinite(value);
    bool changed = !component.sampled || component.healthy != healthy;
    if (healthy && component.values != values) changed = true;
    if (changed || component.policy == ComponentUpdatePolicy::PERIODIC) component.pending = true;
    if (healthy) component.values = values;
    component.healthy = healthy; component.sampled = true; component.sampledAt = millis();
  }

  String ESP8266ComponentService::statusJson(bool pendingOnly) const {
    String json = "{\"components\":[";
    bool first = true;
    for (const auto &component : _components) {
      if (pendingOnly && !component.pending) continue;
      if (!first) json += ',';
      first = false;
      json += "{\"attr\":\"" + component.attr + "\",\"values\":[";
      for (size_t i = 0; i < component.channels.size(); ++i) {
        if (i) json += ',';
        json += "{\"attr\":\"" + component.channels[i].attr + "\",\"value\":";
        json += component.healthy ? String(component.values[i], 6) : String("null");
        json += ",\"healthy\":"; json += component.healthy ? "true" : "false";
        json += ",\"updatePolicy\":\"";
        json += component.policy == ComponentUpdatePolicy::PERIODIC ? "PERIODIC" : "ON_CHANGE";
        json += "\",\"expectedIntervalSeconds\":";
        json += component.policy == ComponentUpdatePolicy::PERIODIC ? String(component.intervalSeconds) : String("null");
        json += '}';
      }
      json += "]}";
    }
    return json + "]}";
  }
  void ESP8266ComponentService::update(const String &token) {
    if (!_running) return;
    _server.handleClient();
    const uint32_t now = millis();
    bool pending = false;
    for (auto &component : _components) { sample(component, now, false); pending |= component.pending; }
    if (!pending || static_cast<int32_t>(now - _retryAt) < 0) return;
    if (_client.publishTelemetry(token, statusJson(true))) {
      for (auto &component : _components) component.pending = false;
    }
    _retryAt = millis() + 3000;
  }
} }
#endif
