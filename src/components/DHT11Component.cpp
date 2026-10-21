#include "DHT11Component.hpp"

namespace ArduinoManager
{
  namespace Components
  {
    namespace
    {
      const uint32_t MIN_INTERVAL_MS = 2000;
      const uint32_t PULSE_TIMEOUT_US = 100;
      const uint32_t START_SIGNAL_MS = 18;
    }

    DHT11Component::DHT11Component(long code, int pin)
        : Core::Component(code, "DHT11"), dataPin(nullptr),
          _lastAttemptTime(0), _initialized(false), _hasAttempted(false),
          _hasValidData(false)
    {
      // Pin only accepts INPUT/OUTPUT in its constructor. The DHT11 pull-up
      // is enabled explicitly in begin(), after registering the pin as INPUT.
      dataPin = addDigitalPin(pin, INPUT, "DHT11 Data Pin", 0);
      for (uint8_t i = 0; i < sizeof(data); ++i)
        data[i] = 0;
    }

    void DHT11Component::begin()
    {
      if (dataPin == nullptr || _initialized)
        return;

      pinMode(dataPin->getPin(), INPUT_PULLUP);
      delay(1000); // Allow the sensor to stabilize after power-up.
      _initialized = true;
    }

    float DHT11Component::readTemperature()
    {
      if (!readSensor())
        return NAN;

      // DHT11 stores the sign in bit 7 of the integer temperature byte.
      float temperature = data[2] + (data[3] & 0x0F) * 0.1f;
      return (data[3] & 0x80) ? -temperature : temperature;
    }

    float DHT11Component::readHumidity()
    {
      if (!readSensor())
        return NAN;

      return data[0] + data[1] * 0.1f;
    }

    bool DHT11Component::perform()
    {
      return readSensor();
    }

    bool DHT11Component::readSensor()
    {
      if (!_initialized || dataPin == nullptr)
        return false;

      const uint32_t now = millis();
      if (_hasAttempted && (uint32_t)(now - _lastAttemptTime) < MIN_INTERVAL_MS)
        return _hasValidData;

      _lastAttemptTime = now;
      _hasAttempted = true;
      uint8_t sample[5] = {0, 0, 0, 0, 0};
      const int pin = dataPin->getPin();

      // DHT11 requires a low start pulse of at least 18 ms.
      pinMode(pin, OUTPUT);
      digitalWrite(pin, LOW);
      delay(START_SIGNAL_MS);
      digitalWrite(pin, HIGH);
      delayMicroseconds(30);
      pinMode(pin, INPUT_PULLUP);

      const uint32_t responseLow = expectPulse(LOW);
      const uint32_t responseHigh = expectPulse(HIGH);
      if (responseLow == 0 || responseLow == PULSE_TIMEOUT_US ||
          responseHigh == 0 || responseHigh == PULSE_TIMEOUT_US)
        return false;

      // Each data bit starts with a low pulse; the following high pulse
      // encodes zero or one by its duration.
      for (uint8_t bit = 0; bit < 40; ++bit)
      {
        const uint32_t lowTime = expectPulse(LOW);
        if (lowTime == 0 || lowTime == PULSE_TIMEOUT_US)
          return false;

        const uint32_t highTime = expectPulse(HIGH);
        if (highTime == 0 || highTime == PULSE_TIMEOUT_US)
          return false;

        sample[bit / 8] <<= 1;
        if (highTime > 50)
          sample[bit / 8] |= 1;
      }

      const uint8_t checksum = sample[0] + sample[1] + sample[2] + sample[3];
      if (sample[4] != checksum)
        return false;

      for (uint8_t i = 0; i < sizeof(data); ++i)
        data[i] = sample[i];
      _hasValidData = true;
      return true;
    }

    uint32_t DHT11Component::expectPulse(bool level)
    {
      const uint32_t start = micros();
      const int pin = dataPin->getPin();

      while (digitalRead(pin) == level)
      {
        if ((uint32_t)(micros() - start) >= PULSE_TIMEOUT_US)
          return PULSE_TIMEOUT_US;
      }

      const uint32_t duration = (uint32_t)(micros() - start);
      return duration >= PULSE_TIMEOUT_US ? PULSE_TIMEOUT_US : duration;
    }

  } // namespace Components
} // namespace ArduinoManager
