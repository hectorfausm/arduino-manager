#include "ArduinoUnoPins.hpp"

namespace ArduinoManager
{
  namespace Boards
  {

    const float ArduinoUnoPins::DEFAULT_VCC = 5.0f;

    // --- Array de pines ---
    const Config::PinConf arduinoUnoPins[] = {
        {"D0", 0, Config::FUNC_D_IO | Config::FUNC_RX},                                   // 0
        {"D1", 1, Config::FUNC_D_IO | Config::FUNC_TX},                                   // 1
        {"D2", 2, Config::FUNC_D_IO},                                                     // 2
        {"D3", 3, Config::FUNC_D_IO | Config::FUNC_PWM},                                  // 3
        {"D4", 4, Config::FUNC_D_IO},                                                     // 4
        {"D5", 5, Config::FUNC_D_IO | Config::FUNC_PWM},                                  // 5
        {"D6", 6, Config::FUNC_D_IO | Config::FUNC_PWM},                                  // 6
        {"D7", 7, Config::FUNC_D_IO},                                                     // 7
        {"D8", 8, Config::FUNC_D_IO},                                                     // 8
        {"D9", 9, Config::FUNC_D_IO | Config::FUNC_PWM},                                  // 9
        {"D10", 10, Config::FUNC_D_IO | Config::FUNC_PWM | Config::FUNC_SPI_SS},          // 10
        {"D11", 11, Config::FUNC_D_IO | Config::FUNC_PWM | Config::FUNC_SPI_MOSI},        // 11
        {"D12", 12, Config::FUNC_D_IO | Config::FUNC_SPI_MISO},                           // 12
        {"D13", 13, Config::FUNC_D_IO | Config::FUNC_SPI_SCK | Config::FUNC_LED_BUILTIN}, // 13
        {"A0", 14, Config::FUNC_A_IO},                                                    // 14
        {"A1", 15, Config::FUNC_A_IO},                                                    // 15
        {"A2", 16, Config::FUNC_A_IO},                                                    // 16
        {"A3", 17, Config::FUNC_A_IO},                                                    // 17
        {"A4", 18, Config::FUNC_A_IO | Config::FUNC_I2C_SDA},                             // 18
        {"A5", 19, Config::FUNC_A_IO | Config::FUNC_I2C_SCL},                             // 19
        {"VIN", -1, Config::FUNC_VIN},                                                    // 20
        {"5V", -1, Config::FUNC_5V},                                                      // 21
        {"3V3", -1, Config::FUNC_3V3},                                                    // 22
        {"GND", -1, Config::FUNC_GND},                                                    // 23
        {"AREF", -1, Config::FUNC_AREF},                                                  // 24
        {"RESET", -1, Config::FUNC_RESET}                                                 // 25
    };

    // --- Referencias simbólicas (estilo enum) ---
    const Config::PinConf &ArduinoUnoPins::D0 = arduinoUnoPins[0];
    const Config::PinConf &ArduinoUnoPins::D1 = arduinoUnoPins[1];
    const Config::PinConf &ArduinoUnoPins::D2 = arduinoUnoPins[2];
    const Config::PinConf &ArduinoUnoPins::D3 = arduinoUnoPins[3];
    const Config::PinConf &ArduinoUnoPins::D4 = arduinoUnoPins[4];
    const Config::PinConf &ArduinoUnoPins::D5 = arduinoUnoPins[5];
    const Config::PinConf &ArduinoUnoPins::D6 = arduinoUnoPins[6];
    const Config::PinConf &ArduinoUnoPins::D7 = arduinoUnoPins[7];
    const Config::PinConf &ArduinoUnoPins::D8 = arduinoUnoPins[8];
    const Config::PinConf &ArduinoUnoPins::D9 = arduinoUnoPins[9];
    const Config::PinConf &ArduinoUnoPins::D10 = arduinoUnoPins[10];
    const Config::PinConf &ArduinoUnoPins::D11 = arduinoUnoPins[11];
    const Config::PinConf &ArduinoUnoPins::D12 = arduinoUnoPins[12];
    const Config::PinConf &ArduinoUnoPins::D13 = arduinoUnoPins[13];

    const Config::PinConf &ArduinoUnoPins::A0 = arduinoUnoPins[14];
    const Config::PinConf &ArduinoUnoPins::A1 = arduinoUnoPins[15];
    const Config::PinConf &ArduinoUnoPins::A2 = arduinoUnoPins[16];
    const Config::PinConf &ArduinoUnoPins::A3 = arduinoUnoPins[17];
    const Config::PinConf &ArduinoUnoPins::A4 = arduinoUnoPins[18];
    const Config::PinConf &ArduinoUnoPins::A5 = arduinoUnoPins[19];

    const Config::PinConf &ArduinoUnoPins::VIN = arduinoUnoPins[20];
    const Config::PinConf &ArduinoUnoPins::V5 = arduinoUnoPins[21];
    const Config::PinConf &ArduinoUnoPins::V3_3 = arduinoUnoPins[22];
    const Config::PinConf &ArduinoUnoPins::GND = arduinoUnoPins[23];
    const Config::PinConf &ArduinoUnoPins::AREF = arduinoUnoPins[24];
    const Config::PinConf &ArduinoUnoPins::RESET = arduinoUnoPins[25];

  } // namespace Boards
} // namespace ArduinoManager
