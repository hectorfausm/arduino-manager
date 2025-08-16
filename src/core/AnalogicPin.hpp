#ifndef ANALOGIC_PIN_HOME
#define ANALOGIC_PIN_HOME

#include "Pin.hpp"

namespace ArduinoManager
{
  namespace Core
  {

    /**
     * @class AnalogicPin
     * @brief Representa un pin analógico en Arduino.
     * 
     * Esta clase hereda de @ref Pin y redefine los métodos 
     * setValue y perform para adaptar su comportamiento a pines analógicos.
     */
    class AnalogicPin : public Pin
    {
    public:
      /**
       * @brief Constructor de AnalogicPin.
       * @param pin Número físico del pin.
       * @param mode Modo del pin (INPUT, OUTPUT, etc.).
       */
      AnalogicPin(int pin, int mode) : Pin(pin, mode) {}

      /**
       * @brief Establece el valor del pin analógico.
       * @param value Valor flotante entre 0 y 1.
       * @return true si el valor es válido y se estableció correctamente, false en caso contrario.
       * @note Valores fuera del rango [0,1] devolverán error (false).
       * @override
       */
      boolean setValue(float value) override;

      /**
       * @brief Ejecuta la acción correspondiente al pin según su modo.
       * 
       * - Si el modo es INPUT, lee el valor analógico desde el pin y lo almacena internamente.
       * - Si el modo es OUTPUT, escribe el valor almacenado en el pin analógico.
       * 
       * @return true si la operación se realizó con éxito, false en caso contrario.
       * @override
       */
      boolean perform() override;

      /**
       * @brief Devuelve una representación en cadena del estado actual del pin analógico.
       * @return Cadena que refleja el estado del pin.
       * @override
       */
      String toString() override;
    };

  } // namespace Core
} // namespace ArduinoManager

#endif // ANALOGIC_PIN_HOME