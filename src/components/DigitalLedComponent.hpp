#ifndef DIGITAL_LED_COMPONENT_HOME
#define DIGITAL_LED_COMPONENT_HOME

#include "../core/Component.hpp"
#include "../core/DigitalPin.hpp"

namespace ArduinoManager
{
  namespace Components
  {
    /**
     * @class DigitalLedComponent
     * @brief Representa un LED digital controlado mediante un pin digital.
     * @details Esta clase hereda de Component y contiene un DigitalPin para gestionar
     * la lógica específica de un LED digital conectado a una placa Arduino.
     */
    class DigitalLedComponent : public Core::Component
    {
    public:
      /**
       * @brief Constructor que crea un DigitalLedComponent con nombre y pin.
       * @param code  Código identificativo del Led digital
       * @param pin   Número del pin digital donde está conectado el LED.
       */
      DigitalLedComponent(long code, int pin);

      /**
       * @brief Enciende el LED.
       * @return true si la operación fue exitosa, false en caso contrario.
       */
      bool turnOn();

      /**
       * @brief Apaga el LED.
       * @return true si la operación fue exitosa, false en caso contrario.
       */
      bool turnOff();

      /**
       * @brief Cambia el estado del LED.
       */
      void toggle();

      /**
       * @brief Realiza la actualización del estado del LED (si es necesaria).
       * @return true si la actualización fue exitosa, false si hubo error.
       */
      bool perform();

      /**
       * @brief Devuelve el estado actual del LED.
       * @return true si está encendido, false si está apagado.
       */
      bool isOn() const;

    private:
      Core::DigitalPin *ledPin; ///< Pin digital asociado al LED.
    };
  }
}

#endif // DIGITAL_LED_COMPONENT_HOME