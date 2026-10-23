#ifndef ARDUINO_MANAGER_ESP8266_WIFI_RESOURCE_CLIENT_HPP
#define ARDUINO_MANAGER_ESP8266_WIFI_RESOURCE_CLIENT_HPP

#include "WifiResourceSettings.hpp"
#include <IPAddress.h>

namespace ArduinoManager
{
namespace Network
{
  /** Performs the REST PATCH that publishes the device's local address. */
  class ESP8266WifiResourceClient
  {
  public:
    explicit ESP8266WifiResourceClient(const WifiResourceSettings &settings);

    bool patchResourceUrl(const String &token, const IPAddress &localIP,
                          uint16_t localPort);

    bool publishTelemetry(const String &token, const String &json);

  private:
    WifiResourceSettings _settings;
  };
}
}

#endif // ARDUINO_MANAGER_ESP8266_WIFI_RESOURCE_CLIENT_HPP
