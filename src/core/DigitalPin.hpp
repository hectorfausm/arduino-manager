#ifndef DIGITAL_PIN_HOME
#define DIGITAL_PIN_HOME

#include "Pin.hpp"

namespace ArduinoManager
{
  namespace Core
  {

    /**
     * @class DigitalPin
     * @brief Representa un pin digital en Arduino.
     * 
     * Esta clase hereda de @ref Pin y redefine los métodos 
     * setValue y perform para adaptar su comportamiento a pines digitales.
     */
    class DigitalPin : public Pin
    {
    public:
      /**
       * @brief Constructor de DigitalPin.
       * @param pin Número físico del pin.
       * @param mode Modo del pin (INPUT, OUTPUT, etc.).
       */
      DigitalPin(int pin, int mode) : Pin(pin, mode) {}

      /**
       * @brief Establece el valor del pin digital.
       * @param value Valor a establecer: 0.0 para LOW, 1.0 para HIGH.
       * @return true si el valor es válido y se estableció correctamente, false en caso contrario.
       * @note Valores diferentes a 0.0 o 1.0 devolverán error (false).
       * @override
       */
      boolean setValue(float value) override;

      /**
       * @brief Ejecuta la acción correspondiente al pin según su modo.
       * 
       * - Si el modo es INPUT, lee el valor digital desde el pin y lo almacena internamente.
       * - Si el modo es OUTPUT, escribe el valor almacenado en el pin digital.
       * 
       * @return true si la operación se realizó con éxito, false en caso contrario.
       * @override
       */
      boolean perform() override;

      /**
       * @brief Devuelve una representación en cadena del estado actual del pin digital.
       * @return Cadena que refleja el estado del pin.
       * @override
       */
      String toString() override;
    };

  } // namespace Core
} // namespace ArduinoManager

#endif // DIGITAL_PIN_HOME
