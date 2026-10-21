#include <ArduinoManagerESP8266.hpp>

using namespace ArduinoManager::Network;

namespace
{
  WifiResourceSettings makeWifiSettings()
  {
    WifiResourceSettings settings;
    settings.provisioningSsid = "ArduinoManager-Setup";
    settings.provisioningPassword = ""; // Configure an 8-63 character password to secure the AP.
    settings.serverBaseUrl = "http://192.168.1.10"; // Set the REST server host.
    settings.serverPort = 8080;
    settings.localResourcePort = 80; // Port used by the device's own resource endpoint.
    settings.httpTimeoutMs = 3000;
    // Optional HTTP Basic authentication:
    // settings.authUsername = "...";
    // settings.authPassword = "...";
    return settings;
  }

  WifiResourceSettings wifiSettings = makeWifiSettings();
  ESP8266EEPROMCredentialStore credentialStore;
  ESP8266WifiResource wifiResource(wifiSettings, credentialStore);
  WifiResourceState lastState = WifiResourceState::ERROR;

  const char *stateName(WifiResourceState state)
  {
    switch (state)
    {
    case WifiResourceState::PROVISIONING:
      return "PROVISIONING";
    case WifiResourceState::CONNECTING:
      return "CONNECTING";
    case WifiResourceState::REGISTERING:
      return "REGISTERING";
    case WifiResourceState::READY:
      return "READY";
    case WifiResourceState::ERROR:
    default:
      return "ERROR";
    }
  }
}

void setup()
{
  Serial.begin(115200);
  if (!wifiResource.begin())
    Serial.println("WiFi resource initialization failed");
}

void loop()
{
  wifiResource.update();

  const WifiResourceState state = wifiResource.getState();
  if (state != lastState)
  {
    Serial.println(stateName(state));
    lastState = state;
  }

  // Send 'r' over Serial to erase saved credentials and reopen provisioning.
  if (Serial.available() && Serial.read() == 'r')
    wifiResource.resetProvisioning();
}
