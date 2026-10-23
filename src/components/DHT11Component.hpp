#ifndef DHT11_COMPONENT_H
#define DHT11_COMPONENT_H

#include "../core/Component.hpp"
#include "../core/DigitalPin.hpp"
#include <stdint.h>

namespace ArduinoManager
{
  namespace Components
  {
    class DHT11Component : public Core::Component
    {
    public:
      /**
       * @brief Constructor
       * @param code Código identificador del componente
       * @param pin  Número del pin donde está conectado el DHT11
       */
      DHT11Component(long code, int pin);

      /** Inicializa el sensor. Debe llamarse una vez desde setup(). */
      void begin();

      /**
       * @brief Lee la temperatura desde el sensor
       */
      float readTemperature();

      /**
       * @brief Lee la humedad desde el sensor
       */
      float readHumidity();

      /** Both outputs share one attempt; failure sets both to NAN. */
      bool readSample(float &temperature, float &humidity);

      /**
       * @brief Método del framework para operaciones
       */
      bool perform();

    private:
      Core::DigitalPin *dataPin;
      uint8_t data[5];
      uint32_t _lastAttemptTime;
      bool _initialized;
      bool _hasAttempted;
      bool _lastAttemptSucceeded;

      bool readSensor();
      uint32_t expectPulse(bool level);
    };
  }
}

#endif
