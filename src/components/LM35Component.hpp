#ifndef LM35_COMPONENT_HOME
#define LM35_COMPONENT_HOME

#include "../core/Component.hpp"
#include "../core/AnalogicPin.hpp"

namespace ArduinoManager
{
  namespace Components
  {
    /**
     * @class LM35Component
     * @brief Representa un sensor de temperatura LM35 conectado a un pin analógico.
     * 
     * @details Esta clase hereda de Component y utiliza un objeto AnalogicPin para 
     * leer la temperatura desde un sensor LM35. El sensor proporciona una salida 
     * analógica proporcional a la temperatura ambiente.
     */
    class LM35Component : public Core::Component
    {
    public:
      /**
       * @brief Constructor del componente LM35.
       * 
       * @param code Código identificador del componente.
       * @param vcc  Voltaje de la placa.
       * @param pin  Número del pin analógico al que está conectado el sensor LM35.
       */
      LM35Component(long code, float vcc, int pin);

      /**
       * @brief Lee la temperatura actual medida por el sensor LM35.
       * 
       * @return Temperatura en grados Celsius.
       */
      float readTemperature();

    private:
      float vcc; ///< Voltaje de la placa
      Core::AnalogicPin *readPin; ///< Pin analógico de lectura.
    };
  }
}

#endif // LM35_COMPONENT_HOME