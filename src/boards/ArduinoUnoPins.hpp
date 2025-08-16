#ifndef ARDUINO_UNO_PINS_HOME
#define ARDUINO_UNO_PINS_HOME

#include "../config/PinFunction.hpp"
#include "../config/PinConf.hpp"

namespace ArduinoManager
{
  namespace Boards
  {

    /**
     * @brief Array constante con la configuración de todos los pines del Arduino Uno.
     *
     * Algunos pines físicos no tienen número lógico (p. ej. VIN, 5V, GND),
     * representados con número -1.
     */
    extern const Config::PinConf arduinoUnoPins[];

    /**
     * @brief Acceso simbólico a los pines del Arduino Uno, estilo enumerado.
     *
     * Permite acceder directamente a objetos PinConf como ArduinoUnoPins::D1, ArduinoUnoPins::A5, etc.
     */
    class ArduinoUnoPins
    {
    public:
      static const Config::PinConf &D0;
      static const Config::PinConf &D1;
      static const Config::PinConf &D2;
      static const Config::PinConf &D3;
      static const Config::PinConf &D4;
      static const Config::PinConf &D5;
      static const Config::PinConf &D6;
      static const Config::PinConf &D7;
      static const Config::PinConf &D8;
      static const Config::PinConf &D9;
      static const Config::PinConf &D10;
      static const Config::PinConf &D11;
      static const Config::PinConf &D12;
      static const Config::PinConf &D13;

      static const Config::PinConf &A0;
      static const Config::PinConf &A1;
      static const Config::PinConf &A2;
      static const Config::PinConf &A3;
      static const Config::PinConf &A4;
      static const Config::PinConf &A5;

      static const Config::PinConf &VIN;
      static const Config::PinConf &V5;
      static const Config::PinConf &V3_3;
      static const Config::PinConf &GND;
      static const Config::PinConf &AREF;
      static const Config::PinConf &RESET;

      static const float DEFAULT_VCC = 5.0;
    };

  } // namespace Boards
} // namespace ArduinoManager

#endif // ARDUINO_UNO_PINS_HOME