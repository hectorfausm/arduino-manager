#ifndef ARDUINO_MANAGER_WIFI_RESOURCE_SETTINGS_HPP
#define ARDUINO_MANAGER_WIFI_RESOURCE_SETTINGS_HPP

#include <Arduino.h>

namespace ArduinoManager
{
namespace Network
{
  /** Configuration for the ESP8266 provisioning AP and REST server. */
  struct WifiResourceSettings
  {
    String provisioningSsid;
    String provisioningPassword;
    String serverBaseUrl;
    String serverPathPrefix;
    uint16_t serverPort;
    uint16_t localResourcePort;
    uint16_t httpTimeoutMs;
    String authUsername;
    String authPassword;

    WifiResourceSettings()
        : serverPort(80), localResourcePort(80), httpTimeoutMs(5000)
    {
    }
  };

  enum class WifiResourceState
  {
    PROVISIONING,
    CONNECTING,
    REGISTERING,
    READY,
    ERROR
  };
}
}

#endif // ARDUINO_MANAGER_WIFI_RESOURCE_SETTINGS_HPP
