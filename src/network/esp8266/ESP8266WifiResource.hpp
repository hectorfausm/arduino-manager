#ifndef ARDUINO_MANAGER_ESP8266_WIFI_RESOURCE_HPP
#define ARDUINO_MANAGER_ESP8266_WIFI_RESOURCE_HPP

#include "WifiResourceSettings.hpp"
#include "ESP8266ProvisioningServer.hpp"
#include "ESP8266WifiResourceClient.hpp"

namespace ArduinoManager
{
namespace Network
{
  /** Coordinates provisioning, WiFi connection and REST resource registration. */
  class ESP8266WifiResource
  {
  public:
    ESP8266WifiResource(const WifiResourceSettings &settings,
                        ICredentialStore &credentialStore);

    bool begin();
    void update();
    WifiResourceState getState() const;
    bool resetProvisioning();

  private:
    bool startProvisioning();
    void startStationConnection();
    void updateConnection(uint32_t now);
    void updateRegistration(uint32_t now);

    WifiResourceSettings _settings;
    ICredentialStore &_credentialStore;
    ESP8266ProvisioningServer _provisioningServer;
    ESP8266WifiResourceClient _client;
    WifiCredentials _credentials;
    WifiResourceState _state;
    uint32_t _nextWifiAttempt;
    uint32_t _wifiRetryDelay;
    uint32_t _nextRegistrationAttempt;
    uint32_t _registrationRetryDelay;
    IPAddress _registeredIP;
  };
}
}

#endif // ARDUINO_MANAGER_ESP8266_WIFI_RESOURCE_HPP
