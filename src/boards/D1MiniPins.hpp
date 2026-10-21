#ifndef D1_MINI_PINS_HOME
#define D1_MINI_PINS_HOME

#include "../config/PinFunction.hpp"
#include "../config/PinConf.hpp"

namespace ArduinoManager
{
  namespace Boards
  {
    /**
     * @brief Pin distribution for the LOLIN/WEMOS D1 mini (ESP8266).
     * GPIO numbers are used for digital pins, and 17 is the ESP8266 core's
     * A0 identifier for analogRead(A0).
     */
    extern const Config::PinConf d1MiniPins[];

    class D1MiniPins
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
      static const Config::PinConf &RX;
      static const Config::PinConf &TX;
      static const Config::PinConf &A0;
      static const Config::PinConf &V5;
      static const Config::PinConf &V3_3;
      static const Config::PinConf &GND;
      static const Config::PinConf &RST;

      static const float DEFAULT_VCC;
      static const int PIN_COUNT = 16;
    };

  } // namespace Boards
} // namespace ArduinoManager

#endif // D1_MINI_PINS_HOME
