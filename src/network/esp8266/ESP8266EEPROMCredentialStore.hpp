#ifndef ARDUINO_MANAGER_ESP8266_EEPROM_CREDENTIAL_STORE_HPP
#define ARDUINO_MANAGER_ESP8266_EEPROM_CREDENTIAL_STORE_HPP

#include "WifiCredentials.hpp"

namespace ArduinoManager
{
namespace Network
{
  /** EEPROM-backed credential store for ESP8266. */
  class ESP8266EEPROMCredentialStore : public ICredentialStore
  {
  public:
    ESP8266EEPROMCredentialStore();

    bool begin();
    bool load(WifiCredentials &credentials);
    bool save(const WifiCredentials &credentials);
    bool clear();

  private:
    bool _initialized;
  };
}
}

#endif // ARDUINO_MANAGER_ESP8266_EEPROM_CREDENTIAL_STORE_HPP
