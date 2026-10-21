#include "D1MiniPins.hpp"

namespace ArduinoManager
{
  namespace Boards
  {
    const float D1MiniPins::DEFAULT_VCC = 3.3f;

    const Config::PinConf d1MiniPins[] = {
        {"D0", 16, Config::FUNC_D_IO},
        {"D1", 5, Config::FUNC_D_IO | Config::FUNC_PWM | Config::FUNC_I2C_SCL},
        {"D2", 4, Config::FUNC_D_IO | Config::FUNC_PWM | Config::FUNC_I2C_SDA},
        {"D3", 0, Config::FUNC_D_IO | Config::FUNC_PWM},
        {"D4", 2, Config::FUNC_D_IO | Config::FUNC_PWM | Config::FUNC_LED_BUILTIN},
        {"D5", 14, Config::FUNC_D_IO | Config::FUNC_PWM | Config::FUNC_SPI_SCK},
        {"D6", 12, Config::FUNC_D_IO | Config::FUNC_PWM | Config::FUNC_SPI_MISO},
        {"D7", 13, Config::FUNC_D_IO | Config::FUNC_PWM | Config::FUNC_SPI_MOSI},
        {"D8", 15, Config::FUNC_D_IO | Config::FUNC_PWM | Config::FUNC_SPI_SS},
        {"RX", 3, Config::FUNC_D_IO | Config::FUNC_PWM | Config::FUNC_RX},
        {"TX", 1, Config::FUNC_D_IO | Config::FUNC_PWM | Config::FUNC_TX},
        {"A0", 17, Config::FUNC_A_IO},
        {"5V", -4, Config::FUNC_5V},
        {"3V3", -5, Config::FUNC_3V3},
        {"GND", -3, Config::FUNC_GND},
        {"RST", -2, Config::FUNC_RESET}};

    const Config::PinConf &D1MiniPins::D0 = d1MiniPins[0];
    const Config::PinConf &D1MiniPins::D1 = d1MiniPins[1];
    const Config::PinConf &D1MiniPins::D2 = d1MiniPins[2];
    const Config::PinConf &D1MiniPins::D3 = d1MiniPins[3];
    const Config::PinConf &D1MiniPins::D4 = d1MiniPins[4];
    const Config::PinConf &D1MiniPins::D5 = d1MiniPins[5];
    const Config::PinConf &D1MiniPins::D6 = d1MiniPins[6];
    const Config::PinConf &D1MiniPins::D7 = d1MiniPins[7];
    const Config::PinConf &D1MiniPins::D8 = d1MiniPins[8];
    const Config::PinConf &D1MiniPins::RX = d1MiniPins[9];
    const Config::PinConf &D1MiniPins::TX = d1MiniPins[10];
    const Config::PinConf &D1MiniPins::A0 = d1MiniPins[11];
    const Config::PinConf &D1MiniPins::V5 = d1MiniPins[12];
    const Config::PinConf &D1MiniPins::V3_3 = d1MiniPins[13];
    const Config::PinConf &D1MiniPins::GND = d1MiniPins[14];
    const Config::PinConf &D1MiniPins::RST = d1MiniPins[15];

  } // namespace Boards
} // namespace ArduinoManager
