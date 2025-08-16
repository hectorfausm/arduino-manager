#ifndef PIN_HOME
#define PIN_HOME

#include <Arduino.h>

namespace ArduinoManager
{
  namespace Core
  {

    /**
     * @class Pin
     * @brief Clase base para representar un pin genérico en Arduino.
     * 
     * Esta clase permite configurar el número, modo y valor de un pin,
     * y sirve como clase base para tipos específicos de pines (digital o analógico).
     * Proporciona métodos comunes para manipular un pin de forma abstracta.
     */
    class Pin
    {
    private:
      /**
       * @brief Número físico del pin.
       */
      int pin;

      /**
       * @brief Modo del pin (por ejemplo: INPUT, OUTPUT).
       */
      int mode;

      /**
       * @brief Descripción opcional del pin.
       */
      String description;

      /**
       * @brief Valor actual asociado al pin.
       */
      float value;

    public:
      /**
       * @brief Constructor de la clase Pin.
       * @param pin Número físico del pin.
       * @param mode Modo del pin (INPUT, OUTPUT, etc.).
       */
      Pin(int pin, int mode);

      /**
       * @brief Establece el modo del pin.
       * @param mode Nuevo modo del pin. Debe ser INPUT o OUTPUT.
       * @note Si se pasa un modo inválido, el comportamiento no está definido.
       */
      void setMode(int mode);

      /**
       * @brief Obtiene el modo actual del pin.
       * @return El modo actual (INPUT, OUTPUT, etc.).
       */
      int getMode();

      /**
       * @brief Obtiene el número físico del pin.
       * @return El número de pin.
       */
      int getPin();

      /**
       * @brief Obtiene el último valor leído o asignado al pin.
       * @return Valor flotante asociado al pin.
       */
      float getValue();

      /**
       * @brief Establece la descripción del pin.
       * @param description Texto descriptivo del pin.
       */
      void setDescription(String description);

      /**
       * @brief Obtiene la descripción del pin.
       * @return Descripción actual del pin.
       */
      String getDescription();

      /**
       * @brief Establece el valor del pin.
       * @param value Nuevo valor para el pin.
       * @return true si el valor fue aceptado, false en caso contrario.
       * @note La interpretación del valor depende de la clase derivada.
       */
      virtual boolean setValue(float value);

      /**
       * @brief Ejecuta una acción asociada al pin.
       * @return true si la acción fue realizada correctamente, false en caso contrario.
       * @note Este método es abstracto y debe implementarse en clases derivadas.
       */
      virtual boolean perform() = 0;

      /**
       * @brief Devuelve una representación en cadena del estado del pin.
       * @return Cadena descriptiva del pin.
       * @note Método abstracto que debe implementarse en clases derivadas.
       */
      virtual String toString() = 0;

      /**
       * @brief Destructor virtual para permitir herencia segura.
       */
      virtual ~Pin() = default;
    };

  } // namespace Core
} // namespace ArduinoManager

#endif // PIN_HOME
