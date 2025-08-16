#ifndef RGB_HOME
#define RGB_HOME

namespace ArduinoManager
{
  namespace Utils
  {
    /**
     * @brief Representa un color RGB con tres componentes: rojo, verde y azul.
     */
    class RGB {
      public:
        int r; ///< Intensidad del canal rojo (0-255)
        int g; ///< Intensidad del canal verde (0-255)
        int b; ///< Intensidad del canal azul (0-255)

        RGB();                     ///< Constructor por defecto
        RGB(int red, int green, int blue); ///< Constructor con parámetros
    };
  }
}

#endif