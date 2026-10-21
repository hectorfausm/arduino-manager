#ifndef ARDUINO_MANAGER_ESP8266_HPP
#define ARDUINO_MANAGER_ESP8266_HPP

// Include this entry point explicitly in ESP8266 sketches. The core aggregator
// ArduinoManager.hpp remains independent of ESP8266-specific dependencies.
#include "network/esp8266/ESP8266WifiResource.hpp"
#include "network/esp8266/ESP8266ProvisioningServer.hpp"
#include "network/esp8266/WifiCredentials.hpp"
#include "network/esp8266/ESP8266EEPROMCredentialStore.hpp"
#include "network/esp8266/ESP8266WifiResourceClient.hpp"

#endif // ARDUINO_MANAGER_ESP8266_HPP
