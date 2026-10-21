#ifndef ARDUINO_MANAGER_ESP8266_PROVISIONING_SERVER_HPP
#define ARDUINO_MANAGER_ESP8266_PROVISIONING_SERVER_HPP

#include <ESP8266WebServer.h>

#include "WifiCredentials.hpp"

namespace ArduinoManager
{
namespace Network
{
  /** Local HTTP endpoint for provisioning WiFi credentials and resource token. */
  class ESP8266ProvisioningServer
  {
  public:
    explicit ESP8266ProvisioningServer(ICredentialStore &credentialStore,
                                       uint16_t port = 80);

    bool begin(const String &ssid, const String &password);
    void update();
    void stop();
    bool credentialsSaved() const;

  private:
    void handleCredentials();

    ICredentialStore &_credentialStore;
    ESP8266WebServer _server;
    bool _started;
    bool _credentialsSaved;
  };
}
}

#endif // ARDUINO_MANAGER_ESP8266_PROVISIONING_SERVER_HPP
