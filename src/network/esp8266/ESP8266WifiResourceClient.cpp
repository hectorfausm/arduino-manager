#if defined(ESP8266)
#include "ESP8266WifiResourceClient.hpp"

#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

namespace ArduinoManager
{
namespace Network
{
  namespace
  {
    String encodePathSegment(const String &value)
    {
      static const char HEX_DIGITS[] = "0123456789ABCDEF";
      String encoded;
      for (size_t i = 0; i < value.length(); ++i)
      {
        const uint8_t c = static_cast<uint8_t>(value[i]);
        const bool unreserved = (c >= 'a' && c <= 'z') ||
                                (c >= 'A' && c <= 'Z') ||
                                (c >= '0' && c <= '9') || c == '-' ||
                                c == '_' || c == '.' || c == '~';
        if (unreserved)
          encoded += static_cast<char>(c);
        else
        {
          encoded += '%';
          encoded += HEX_DIGITS[c >> 4];
          encoded += HEX_DIGITS[c & 0x0F];
        }
      }
      return encoded;
    }
  }

  ESP8266WifiResourceClient::ESP8266WifiResourceClient(
      const WifiResourceSettings &settings)
      : _settings(settings)
  {
  }

  bool ESP8266WifiResourceClient::publishTelemetry(const String &token, const String &json)
  {
    if (token.length() == 0 || _settings.serverBaseUrl.length() == 0 ||
        _settings.httpTimeoutMs == 0) return false;
    String base = _settings.serverBaseUrl;
    while (base.endsWith("/")) base.remove(base.length() - 1);
    String prefix = _settings.serverPathPrefix;
    if (prefix.length() && !prefix.startsWith("/")) prefix = "/" + prefix;
    while (prefix.endsWith("/")) prefix.remove(prefix.length() - 1);
    WiFiClient client;
    HTTPClient http;
    http.setTimeout(_settings.httpTimeoutMs);
    const String url = base + ":" + String(_settings.serverPort) + prefix +
        "/wifiresource/" + encodePathSegment(token) + "/telemetry";
    if (!http.begin(client, url)) { http.end(); return false; }
    http.addHeader("Content-Type", "application/json");
    if (_settings.authUsername.length() || _settings.authPassword.length())
      http.setAuthorization(_settings.authUsername.c_str(), _settings.authPassword.c_str());
    int code = http.POST(json);
    http.end();
    return code == HTTP_CODE_OK;
  }

  bool ESP8266WifiResourceClient::patchResourceUrl(
      const String &token, const IPAddress &localIP, uint16_t localPort)
  {
    if (token.length() == 0 || _settings.serverBaseUrl.length() == 0 ||
        _settings.serverPort == 0 || localPort == 0 ||
        _settings.httpTimeoutMs == 0)
      return false;

    String baseUrl = _settings.serverBaseUrl;
    while (baseUrl.endsWith("/"))
      baseUrl.remove(baseUrl.length() - 1);
    String pathPrefix = _settings.serverPathPrefix;
    if (pathPrefix.length() > 0 && !pathPrefix.startsWith("/"))
      pathPrefix = "/" + pathPrefix;
    while (pathPrefix.endsWith("/"))
      pathPrefix.remove(pathPrefix.length() - 1);
    const String url = baseUrl + ":" + String(_settings.serverPort) +
                       pathPrefix + "/wifiresource/" + encodePathSegment(token);
    const String localUrl = localIP.toString() + ":" + String(localPort);
    const String body = "[{\"op\":\"replace\",\"path\":\"/localUrl\",\"value\":\"" +
                        localUrl + "\"}]";

    WiFiClient client;
    HTTPClient http;
    http.setTimeout(_settings.httpTimeoutMs);
    if (!http.begin(client, url))
    {
      http.end();
      return false;
    }

    http.addHeader("Content-Type", "application/json-patch+json");
    if (_settings.authUsername.length() > 0 ||
        _settings.authPassword.length() > 0)
      http.setAuthorization(_settings.authUsername.c_str(),
                            _settings.authPassword.c_str());

    const int responseCode = http.sendRequest("PATCH", body);
    http.end();
    return responseCode == HTTP_CODE_OK;
  }
}
}
#endif // ESP8266
