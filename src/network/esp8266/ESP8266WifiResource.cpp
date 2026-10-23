#if defined(ESP8266)
#include "ESP8266WifiResource.hpp"

#include <ESP8266WiFi.h>

namespace ArduinoManager
{
namespace Network
{
  namespace
  {
    const uint32_t INITIAL_RETRY_DELAY_MS = 1000;
    const uint32_t MAX_RETRY_DELAY_MS = 30000;

    uint32_t nextDelay(uint32_t delay)
    {
      return delay >= MAX_RETRY_DELAY_MS / 2 ? MAX_RETRY_DELAY_MS : delay * 2;
    }
  }

  ESP8266WifiResource::ESP8266WifiResource(
      const WifiResourceSettings &settings, ICredentialStore &credentialStore)
      : _components(settings), _settings(settings), _credentialStore(credentialStore),
        _provisioningServer(credentialStore), _client(settings),
        _state(WifiResourceState::ERROR), _nextWifiAttempt(0),
        _wifiRetryDelay(INITIAL_RETRY_DELAY_MS), _nextRegistrationAttempt(0),
        _registrationRetryDelay(INITIAL_RETRY_DELAY_MS), _registeredIP(0, 0, 0, 0)
  {
  }

  bool ESP8266WifiResource::begin()
  {
    if (!_credentialStore.begin())
    {
      _state = WifiResourceState::ERROR;
      return false;
    }

    if (_credentialStore.load(_credentials))
    {
      startStationConnection();
      return true;
    }

    return startProvisioning();
  }

  bool ESP8266WifiResource::startProvisioning()
  {
    if (!_provisioningServer.begin(_settings.provisioningSsid,
                                   _settings.provisioningPassword))
    {
      _state = WifiResourceState::ERROR;
      return false;
    }
    _state = WifiResourceState::PROVISIONING;
    return true;
  }

  void ESP8266WifiResource::startStationConnection()
  {
    _components.stop();
    _provisioningServer.stop();
    WiFi.mode(WIFI_STA);
    WiFi.begin(_credentials.ssid.c_str(), _credentials.password.c_str());
    _state = WifiResourceState::CONNECTING;
    _wifiRetryDelay = INITIAL_RETRY_DELAY_MS;
    _nextWifiAttempt = millis() + 15000;
  }

  void ESP8266WifiResource::update()
  {
    const uint32_t now = millis();

    if (_state == WifiResourceState::PROVISIONING)
    {
      _provisioningServer.update();
      if (_provisioningServer.credentialsSaved() &&
          _credentialStore.load(_credentials))
        startStationConnection();
      return;
    }

    if (_state == WifiResourceState::CONNECTING)
    {
      updateConnection(now);
      return;
    }

    if (_state == WifiResourceState::REGISTERING)
    {
      updateRegistration(now);
      return;
    }

    if (_state == WifiResourceState::READY)
    {
      if (WiFi.status() != WL_CONNECTED)
      {
        startStationConnection();
      }
      else if (WiFi.localIP() != _registeredIP)
      {
        _components.stop();
        _state = WifiResourceState::REGISTERING;
        _nextRegistrationAttempt = now;
        _registrationRetryDelay = INITIAL_RETRY_DELAY_MS;
      }
      else _components.update(_credentials.token);
    }
  }

  void ESP8266WifiResource::updateConnection(uint32_t now)
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      _state = WifiResourceState::REGISTERING;
      _nextRegistrationAttempt = now;
      _registrationRetryDelay = INITIAL_RETRY_DELAY_MS;
      return;
    }

    if (static_cast<int32_t>(now - _nextWifiAttempt) >= 0)
    {
      WiFi.begin(_credentials.ssid.c_str(), _credentials.password.c_str());
      _nextWifiAttempt = now + _wifiRetryDelay;
      _wifiRetryDelay = nextDelay(_wifiRetryDelay);
    }
  }

  void ESP8266WifiResource::updateRegistration(uint32_t now)
  {
    if (WiFi.status() != WL_CONNECTED)
    {
      startStationConnection();
      return;
    }

    if (static_cast<int32_t>(now - _nextRegistrationAttempt) < 0)
      return;

    const IPAddress currentIP = WiFi.localIP();
    if (_client.patchResourceUrl(_credentials.token, currentIP,
                                 _settings.localResourcePort))
    {
      _registeredIP = currentIP;
      _state = WifiResourceState::READY;
      _components.begin();
      _registrationRetryDelay = INITIAL_RETRY_DELAY_MS;
      return;
    }

    _nextRegistrationAttempt = now + _registrationRetryDelay;
    _registrationRetryDelay = nextDelay(_registrationRetryDelay);
  }

  WifiResourceState ESP8266WifiResource::getState() const
  {
    return _state;
  }

  bool ESP8266WifiResource::resetProvisioning()
  {
    if (!_credentialStore.clear())
      return false;

    _components.stop();
    _provisioningServer.stop();
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
    _credentials = WifiCredentials();
    _registeredIP = IPAddress(0, 0, 0, 0);
    return startProvisioning();
  }
}
}
#endif // ESP8266
