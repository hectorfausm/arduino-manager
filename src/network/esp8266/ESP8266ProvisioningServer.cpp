#if defined(ESP8266)
#include "ESP8266ProvisioningServer.hpp"

#include <ESP8266WiFi.h>

namespace ArduinoManager
{
namespace Network
{
  ESP8266ProvisioningServer::ESP8266ProvisioningServer(
      ICredentialStore &credentialStore, uint16_t port)
      : _credentialStore(credentialStore), _server(port), _started(false),
        _credentialsSaved(false)
  {
  }

  bool ESP8266ProvisioningServer::begin(const String &ssid,
                                        const String &password)
  {
    _credentialsSaved = false;
    if (ssid.length() == 0 || ssid.length() > WIFI_SSID_MAX_LENGTH ||
        (password.length() != 0 &&
         (password.length() < 8 || password.length() > 63)))
      return false;

    WiFi.mode(WIFI_AP);
    if (!WiFi.softAP(ssid.c_str(), password.c_str()))
      return false;

    _server.on("/getCredentials", HTTP_POST,
               [this]() { handleCredentials(); });
    _server.onNotFound([this]() {
      _server.send(404, "text/plain", "Endpoint not found");
    });
    _server.begin();
    _started = true;
    return true;
  }

  void ESP8266ProvisioningServer::update()
  {
    if (_started)
      _server.handleClient();
  }

  void ESP8266ProvisioningServer::stop()
  {
    if (!_started)
      return;

    _server.stop();
    WiFi.softAPdisconnect(true);
    _started = false;
  }

  bool ESP8266ProvisioningServer::credentialsSaved() const
  {
    return _credentialsSaved;
  }

  void ESP8266ProvisioningServer::handleCredentials()
  {
    if (!_server.hasArg("essid") || !_server.hasArg("password") ||
        !_server.hasArg("token"))
    {
      _server.send(400, "text/plain",
                   "Required fields: essid, password, token");
      return;
    }

    WifiCredentials credentials;
    credentials.ssid = _server.arg("essid");
    credentials.password = _server.arg("password");
    credentials.token = _server.arg("token");

    if (credentials.ssid.length() == 0 ||
        credentials.password.length() == 0 || credentials.token.length() == 0)
    {
      _server.send(400, "text/plain", "Credential fields cannot be empty");
      return;
    }

    if (credentials.ssid.length() > WIFI_SSID_MAX_LENGTH ||
        credentials.password.length() > WIFI_PASSWORD_MAX_LENGTH ||
        credentials.token.length() > WIFI_TOKEN_MAX_LENGTH)
    {
      _server.send(413, "text/plain", "Credential field is too long");
      return;
    }

    if (!_credentialStore.save(credentials))
    {
      _server.send(500, "text/plain", "Could not save credentials");
      return;
    }

    _credentialsSaved = true;
    _server.send(200, "text/plain", "Credentials saved");
  }
}
}
#endif // ESP8266
