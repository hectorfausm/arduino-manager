#include "DHT11.h"

#define MIN_INTERVAL 2000     // Tiempo mínimo entre lecturas (ms)
#define TIMEOUT 10000         // Límite de espera para señales
#define START_SIGNAL_MS 20    // Duración del pulso bajo para iniciar comunicación

DHT11::DHT11(uint8_t pin) : _pin(pin), _lastReadTime(0) {}

void DHT11::begin() {
  pinMode(_pin, INPUT_PULLUP);
  delay(1000); // Tiempo para estabilización inicial del sensor
}

float DHT11::readTemperature() {
  if (!read()) return NAN;
  return data[2]; // DHT11: temperatura entera en el byte 3
}

float DHT11::readHumidity() {
  if (!read()) return NAN;
  return data[0]; // DHT11: humedad entera en el byte 1
}

bool DHT11::read() {
  uint32_t currentTime = millis();
  if (currentTime - _lastReadTime < MIN_INTERVAL) {
    return false;
  }
  _lastReadTime = currentTime;

  // Reset de datos
  for (int i = 0; i < 5; ++i) data[i] = 0;

  // Señal de inicio
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, LOW);
  delay(START_SIGNAL_MS);
  digitalWrite(_pin, HIGH);
  delayMicroseconds(40);
  pinMode(_pin, INPUT);

  if (expectPulse(LOW) == TIMEOUT) return false;
  if (expectPulse(HIGH) == TIMEOUT) return false;

  // Leer 40 bits (5 bytes)
  for (int i = 0; i < 40; ++i) {
    if (expectPulse(LOW) == TIMEOUT) return false;
    uint32_t highTime = expectPulse(HIGH);
    if (highTime == TIMEOUT) return false;

    data[i / 8] <<= 1;
    if (highTime > 40) { // Umbral empírico para distinguir "1" y "0"
      data[i / 8] |= 1;
    }
  }

  // Verificar checksum
  uint8_t sum = data[0] + data[1] + data[2] + data[3];
  return (data[4] == (sum & 0xFF));
}

uint32_t DHT11::expectPulse(bool level) {
  uint32_t count = 0;
  while (digitalRead(_pin) == level) {
    if (++count >= TIMEOUT) return TIMEOUT;
  }
  return count;
}
