#if defined(ESP8266)
#include "ESP8266EEPROMCredentialStore.hpp"

#include <EEPROM.h>
#include <string.h>

namespace ArduinoManager
{
namespace Network
{
  namespace
  {
    enum
    {
      EEPROM_CAPACITY = 256,
      RECORD_SIZE = 234,
      HEADER_SIZE = 8,
      CHECKSUM_OFFSET = 232,
      FORMAT_VERSION = 1
    };

    const uint8_t MAGIC[4] = {0x41, 0x4D, 0x57, 0x46}; // "AMWF"

    uint16_t checksum(const uint8_t *data, size_t length)
    {
      uint16_t crc = 0xFFFF;
      for (size_t i = 0; i < length; ++i)
      {
        crc ^= static_cast<uint16_t>(data[i]) << 8;
        for (uint8_t bit = 0; bit < 8; ++bit)
          crc = (crc & 0x8000) ? static_cast<uint16_t>((crc << 1) ^ 0x1021)
                               : static_cast<uint16_t>(crc << 1);
      }
      return crc;
    }

    bool matchesMagic(const uint8_t *record)
    {
      return memcmp(record, MAGIC, sizeof(MAGIC)) == 0;
    }

    uint16_t storedChecksum(const uint8_t *record)
    {
      return static_cast<uint16_t>(record[CHECKSUM_OFFSET]) |
             (static_cast<uint16_t>(record[CHECKSUM_OFFSET + 1]) << 8);
    }
  }

  ESP8266EEPROMCredentialStore::ESP8266EEPROMCredentialStore()
      : _initialized(false)
  {
  }

  bool ESP8266EEPROMCredentialStore::begin()
  {
    if (_initialized)
      return true;
    _initialized = EEPROM.begin(EEPROM_CAPACITY);
    return _initialized;
  }

  bool ESP8266EEPROMCredentialStore::load(WifiCredentials &credentials)
  {
    if (!_initialized)
      return false;

    uint8_t record[RECORD_SIZE];
    for (size_t i = 0; i < RECORD_SIZE; ++i)
      record[i] = EEPROM.read(i);

    if (!matchesMagic(record) || record[4] != FORMAT_VERSION)
      return false;

    const size_t ssidLength = record[5];
    const size_t passwordLength = record[6];
    const size_t tokenLength = record[7];
    if (ssidLength == 0 || ssidLength > WIFI_SSID_MAX_LENGTH ||
        passwordLength == 0 || passwordLength > WIFI_PASSWORD_MAX_LENGTH ||
        tokenLength == 0 || tokenLength > WIFI_TOKEN_MAX_LENGTH)
      return false;

    if (storedChecksum(record) != checksum(record + 4, CHECKSUM_OFFSET - 4))
      return false;

    char ssid[WIFI_SSID_MAX_LENGTH + 1];
    char password[WIFI_PASSWORD_MAX_LENGTH + 1];
    char token[WIFI_TOKEN_MAX_LENGTH + 1];
    size_t offset = HEADER_SIZE;
    memcpy(ssid, record + offset, ssidLength);
    ssid[ssidLength] = '\0';
    offset += WIFI_SSID_MAX_LENGTH;
    memcpy(password, record + offset, passwordLength);
    password[passwordLength] = '\0';
    offset += WIFI_PASSWORD_MAX_LENGTH;
    memcpy(token, record + offset, tokenLength);
    token[tokenLength] = '\0';

    WifiCredentials loaded;
    loaded.ssid = ssid;
    loaded.password = password;
    loaded.token = token;
    credentials = loaded;
    return true;
  }

  bool ESP8266EEPROMCredentialStore::save(const WifiCredentials &credentials)
  {
    if (!_initialized || credentials.ssid.length() == 0 ||
        credentials.ssid.length() > WIFI_SSID_MAX_LENGTH ||
        credentials.password.length() == 0 ||
        credentials.password.length() > WIFI_PASSWORD_MAX_LENGTH ||
        credentials.token.length() == 0 ||
        credentials.token.length() > WIFI_TOKEN_MAX_LENGTH)
      return false;

    uint8_t record[RECORD_SIZE] = {0};
    memcpy(record, MAGIC, sizeof(MAGIC));
    record[4] = FORMAT_VERSION;
    record[5] = static_cast<uint8_t>(credentials.ssid.length());
    record[6] = static_cast<uint8_t>(credentials.password.length());
    record[7] = static_cast<uint8_t>(credentials.token.length());

    size_t offset = HEADER_SIZE;
    memcpy(record + offset, credentials.ssid.c_str(), credentials.ssid.length());
    offset += WIFI_SSID_MAX_LENGTH;
    memcpy(record + offset, credentials.password.c_str(),
           credentials.password.length());
    offset += WIFI_PASSWORD_MAX_LENGTH;
    memcpy(record + offset, credentials.token.c_str(),
           credentials.token.length());

    const uint16_t crc = checksum(record + 4, CHECKSUM_OFFSET - 4);
    record[CHECKSUM_OFFSET] = static_cast<uint8_t>(crc & 0xFF);
    record[CHECKSUM_OFFSET + 1] = static_cast<uint8_t>(crc >> 8);

    for (size_t i = 0; i < RECORD_SIZE; ++i)
      EEPROM.write(i, record[i]);
    if (!EEPROM.commit())
      return false;

    for (size_t i = 0; i < RECORD_SIZE; ++i)
      if (EEPROM.read(i) != record[i])
        return false;
    return true;
  }

  bool ESP8266EEPROMCredentialStore::clear()
  {
    if (!_initialized)
      return false;

    for (size_t i = 0; i < sizeof(MAGIC); ++i)
      EEPROM.write(i, 0);
    if (!EEPROM.commit())
      return false;

    for (size_t i = 0; i < sizeof(MAGIC); ++i)
      if (EEPROM.read(i) != 0)
        return false;
    return true;
  }
}
}
#endif // ESP8266
