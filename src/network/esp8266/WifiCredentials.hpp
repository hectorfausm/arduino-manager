#ifndef ARDUINO_MANAGER_WIFI_CREDENTIALS_HPP
#define ARDUINO_MANAGER_WIFI_CREDENTIALS_HPP

#include <Arduino.h>

namespace ArduinoManager
{
namespace Network
{
  // Limits follow ESP8266 WiFi SSID/password limits. Tokens are bounded so
  // stores can use a fixed-size, validated record.
  enum
  {
    WIFI_SSID_MAX_LENGTH = 32,
    WIFI_PASSWORD_MAX_LENGTH = 64,
    WIFI_TOKEN_MAX_LENGTH = 128
  };

  struct WifiCredentials
  {
    String ssid;
    String password;
    String token;
  };

  class ICredentialStore
  {
  public:
    virtual ~ICredentialStore() {}
    virtual bool begin() = 0;
    virtual bool load(WifiCredentials &credentials) = 0;
    virtual bool save(const WifiCredentials &credentials) = 0;
    virtual bool clear() = 0;
  };
}
}

#endif // ARDUINO_MANAGER_WIFI_CREDENTIALS_HPP
