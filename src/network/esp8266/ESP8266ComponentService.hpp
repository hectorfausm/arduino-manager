#ifndef ARDUINO_MANAGER_ESP8266_COMPONENT_SERVICE_HPP
#define ARDUINO_MANAGER_ESP8266_COMPONENT_SERVICE_HPP
#include "ESP8266WifiResourceClient.hpp"
#include <ESP8266WebServer.h>
#include <functional>
#include <vector>
#include "../../components/DHT11Component.hpp"

namespace ArduinoManager { namespace Network {
  enum class ComponentUpdatePolicy { PERIODIC, ON_CHANGE };

  /** Callbacks run in the main loop. Return NAN on a component read failure. */
  class ESP8266ComponentService {
  public:
    using Reader = std::function<double()>;
    using Writer = std::function<bool(double)>;
    struct Channel {
      String attr;
      Writer write;
    };
    // Fill outputs in channel order. False or a non-finite output fails the whole sample.
    using JointReader = std::function<bool(std::vector<double> &)>;
    bool addCompositeComponent(const String &attr, const std::vector<Channel> &channels,
                            ComponentUpdatePolicy policy, uint32_t intervalSeconds,
                            JointReader read, uint32_t minimumReadIntervalMs = 0);
    bool addDHT11Component(const String &attr, const String &temperatureAttr,
                       const String &humidityAttr, ComponentUpdatePolicy policy,
                       uint32_t intervalSeconds, Components::DHT11Component &dht);
    explicit ESP8266ComponentService(const WifiResourceSettings &settings);
    bool addComponent(const String &attr, ComponentUpdatePolicy policy, uint32_t intervalSeconds,
                   Reader read, Writer write = Writer());
    void begin();
    void stop();
    void update(const String &token);
  private:
    struct Component {
      String attr;
      ComponentUpdatePolicy policy;
      uint32_t intervalSeconds;
      JointReader read;
      std::vector<Channel> channels;
      std::vector<double> values;
      uint32_t minimumReadIntervalMs;
      bool healthy;
      bool sampled;
      bool pending;
      uint32_t sampledAt;
    };
    void sample(Component &component, uint32_t now, bool force);
    static bool validAttr(const String &attr);
    String statusJson(bool pendingOnly) const;
    ESP8266WebServer _server;
    ESP8266WifiResourceClient _client;
    std::vector<Component> _components;
    bool _running;
    uint32_t _retryAt;
  };
} }
#endif
