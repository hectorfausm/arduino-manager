#ifndef PIN_FUNCTION_HOME
#define PIN_FUNCTION_HOME

namespace ArduinoManager
{
  namespace Config
  {

    /**
     * @enum PinFunction
     * @brief Representa las posibles funciones de un pin mediante máscara de bits.
     *
     * Cada valor corresponde a una capacidad funcional del pin.
     * Las constantes pueden combinarse con operaciones bitwise (OR) para representar múltiples funciones.
     */
    enum PinFunction
    {
      FUNC_NONE        = 0,          //< Sin función asignada.
      FUNC_RX          = 1 << 0,     //< UART: recepción de datos.
      FUNC_TX          = 1 << 1,     //< UART: transmisión de datos.
      FUNC_D_IO        = 1 << 2,     //< Entrada o salida digital.
      FUNC_PWM         = 1 << 3,     //< Salida PWM.
      FUNC_SPI_SS      = 1 << 4,     //< SPI: selección de esclavo.
      FUNC_SPI_MOSI    = 1 << 5,     //< SPI: Master Out Slave In.
      FUNC_SPI_MISO    = 1 << 6,     //< SPI: Master In Slave Out.
      FUNC_SPI_SCK     = 1 << 7,     //< SPI: señal de reloj.
      FUNC_LED_BUILTIN = 1 << 8,     //< LED integrado en la placa.
      FUNC_A_IO        = 1 << 9,     //< Entrada o salida analógica.
      FUNC_I2C_SDA     = 1 << 10,    //< I2C: línea de datos.
      FUNC_I2C_SCL     = 1 << 11,    //< I2C: línea de reloj.
      FUNC_VIN         = 1 << 12,    //< Entrada de voltaje a la placa.
      FUNC_5V          = 1 << 13,    //< Salida de 5V.
      FUNC_3V3         = 1 << 14,    //< Salida de 3.3V.
      FUNC_GND         = 1 << 15,    //< Tierra (masa).
      FUNC_AREF        = 1 << 16,    //< Referencia analógica externa.
      FUNC_RESET       = 1 << 17     //< Pin de reinicio del microcontrolador.
    };

  }
}
#endif // PIN_FUNCTION_HOME