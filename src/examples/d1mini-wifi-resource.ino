#include <ArduinoManagerESP8266.hpp>
#include <components/DHT11Component.hpp>

using namespace ArduinoManager::Network;

namespace
{
  WifiResourceSettings makeWifiSettings()
  {
    WifiResourceSettings settings;
    settings.provisioningSsid = "ArduinoManager-Setup";
    settings.provisioningPassword = ""; // Configure an 8-63 character password to secure the AP.
    settings.serverBaseUrl = "http://192.168.1.10"; // Set the REST server host.
    settings.serverPathPrefix = "/domo/rest"; // Java REST API base path; use empty for root routes.
    settings.serverPort = 9024;
    settings.localResourcePort = 80; // Port used by the device's own resource endpoint.
    settings.httpTimeoutMs = 3000;
    // Optional HTTP Basic authentication:
    // settings.authUsername = "...";
    // settings.authPassword = "...";
    return settings;
  }

  ArduinoManager::Components::DHT11Component climate(1, 4); // D2 = GPIO4

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
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  // DHT11 data on D2/GPIO4; power and pull-up at 3.3 V, common GND.
  climate.begin();
  if (!wifiResource.addDHT11Component("climate", "temperature", "humidity",
          ComponentUpdatePolicy::PERIODIC, 10, climate)) {
    Serial.println("Invalid climate configuration");
    return;
  }
  // Attribute names must already exist in domo for this resource.
  if (!wifiResource.addComponent("led", ComponentUpdatePolicy::ON_CHANGE, 0,
      []() -> double { return digitalRead(LED_BUILTIN) == LOW ? 1 : 0; },
      [](double value) -> bool {
        if (value != 0 && value != 1) return false;
        digitalWrite(LED_BUILTIN, value == 1 ? LOW : HIGH);
        return true;
      })) {
    Serial.println("Invalid LED configuration");
    return;
  }
  // Example for a board with an LM35 on A0 (adapt voltage scaling to your board):
  // wifiResource.addComponent("lm35Temperature", ComponentUpdatePolicy::PERIODIC, 10,
  //     []() -> double { return analogRead(A0) * 3.3 / 1023.0 * 100.0; });
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
