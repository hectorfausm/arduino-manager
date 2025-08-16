#ifndef PIN_CONF_HOME
#define PIN_CONF_HOME

namespace ArduinoManager
{
  namespace Config
  {

    /**
     * @class PinConf
     * @brief Representa la configuración básica de un pin de placa.
     *
     * Contiene el nombre del pin, su número físico o lógico, y una máscara de bits que indica
     * las funciones que puede desempeñar (ver @ref PinFunction).
     */
    class PinConf
    {
    public:

      // Consdtructor por defecto para poder crear una copia
      constexpr PinConf() : name(""), number(-1), functions(0), used(false) {}
    
      /// Constructor constante con nombre, número y funciones.
      constexpr PinConf(char *name, int number, unsigned int functions)
          : name(name), number(number), functions(functions), used(false) {}

      /// Nombre del pin (ej. "D0", "A1").
      char *name;

      /* Determina si la configuración del pin está siendo utilizada o no */
      bool used = false;

      /**
       * Número lógico o físico del pin.
       * Puede ser negativo si no se asigna a un I/O (ej. VIN, GND).
       */
      int number;

      /**
       * Máscara de funciones válidas para este pin (ver @ref PinFunction).
       * Permite comprobar si el pin soporta una funcionalidad específica mediante operaciones bitwise.
       */
      unsigned int functions;
    };
  }
}
#endif // PIN_CONF_HOME