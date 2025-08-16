#ifndef DHT11_H
#define DHT11_H

#include <Arduino.h>

class DHT11 {
public:
  DHT11(uint8_t pin);
  void begin();
  float readTemperature();
  float readHumidity();

private:
  uint8_t _pin;
  uint8_t data[5];
  uint32_t _lastReadTime;
  bool read();
  uint32_t expectPulse(bool level);
};

#endif