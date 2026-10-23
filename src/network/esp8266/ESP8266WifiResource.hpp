#ifndef ARDUINO_MANAGER_ESP8266_WIFI_RESOURCE_HPP
#define ARDUINO_MANAGER_ESP8266_WIFI_RESOURCE_HPP

#include "WifiResourceSettings.hpp"
#include "ESP8266ComponentService.hpp"
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

    bool addComponent(const String &attr, ComponentUpdatePolicy policy, uint32_t intervalSeconds,
                   ESP8266ComponentService::Reader read,
                   ESP8266ComponentService::Writer write = ESP8266ComponentService::Writer()) {
      return _components.addComponent(attr, policy, intervalSeconds, read, write);
    }
    bool addCompositeComponent(const String &attr,
        const std::vector<ESP8266ComponentService::Channel> &channels,
        ComponentUpdatePolicy policy, uint32_t intervalSeconds,
        ESP8266ComponentService::JointReader read, uint32_t minimumReadIntervalMs = 0) {
      return _components.addCompositeComponent(attr, channels, policy, intervalSeconds,
                                        read, minimumReadIntervalMs);
    }
    bool addDHT11Component(const String &attr, const String &temperatureAttr,
        const String &humidityAttr, ComponentUpdatePolicy policy,
        uint32_t intervalSeconds, Components::DHT11Component &dht) {
      return _components.addDHT11Component(attr, temperatureAttr, humidityAttr, policy, intervalSeconds, dht);
    }
    bool begin();
    void update();
    WifiResourceState getState() const;
    bool resetProvisioning();

  private:
    bool startProvisioning();
    void startStationConnection();
    void updateConnection(uint32_t now);
    void updateRegistration(uint32_t now);

    ESP8266ComponentService _components;
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
