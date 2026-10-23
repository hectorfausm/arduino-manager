#ifndef RGB_LED_COMPONENT_HOME
#define RGB_LED_COMPONENT_HOME

#include "../core/Component.hpp"
#include "../core/AnalogicPin.hpp"
#include "../utils/RGB.hpp"

namespace ArduinoManager
{
  namespace Components
  {
    /**
     * @class RGBLedComponent
     * @brief Representa un LED RGB controlado mediante tres pines analógicos.
     * 
     * Esta clase hereda de Component y utiliza tres pines (R, G, B) 
     * para controlar la intensidad de cada canal de color de un LED RGB 
     * conectado a una placa Arduino.
     */
    class RGBLedComponent : public Core::Component {
      public:

        /**
         * @brief Constructor de un LED RGB.
         * 
         * @param code Código identificativo del componente.
         * @param pinR Pin analógico para el canal rojo.
         * @param pinG Pin analógico para el canal verde.
         * @param pinB Pin analógico para el canal azul.
         * @param cathode Modo cátodo (Los valores máximos a 255) o modo ánodo (Los valores máximos a 0)
         */
        RGBLedComponent(long code, int pinR, int pinG, int pinB, bool cathode);

        /**
         * @brief Enciende el LED RGB con un color específico.
         * 
         * @param r Intensidad del canal rojo (0-255).
         * @param g Intensidad del canal verde (0-255).
         * @param b Intensidad del canal azul (0-255).
         * @return true si la operación fue exitosa, false en caso contrario.
         */
        bool turnRGBColor(int r, int g, int b);

        /**
         * @brief Enciende el LED con el último color configurado.
         * 
         * @return true si la operación fue exitosa, false en caso contrario.
         */
        bool turnRGBColor(Utils::RGB color);

        /**
         * @brief Apaga el LED RGB (pone todos los canales en 0).
         * 
         * @return true si la operación fue exitosa, false en caso contrario.
         */
        bool turnOff();

        /**
         * @brief Obtiene el color RGB actual del LED.
         * 
         * @return Color RGB guardado por la última operación correcta; negro al apagar.
         */
        Utils::RGB  getRGBColor();

      private:
        Core::AnalogicPin* rPin; ///< Pin analógico para el canal rojo del LED RGB.
        Core::AnalogicPin* gPin; ///< Pin analógico para el canal verde del LED RGB.
        Core::AnalogicPin* bPin; ///< Pin analógico para el canal azul del LED RGB.
        bool cathode;
        Utils::RGB  ledColor;
    };

   /**
   * @brief Colores RGB predefinidos como constantes estáticas.
   */
    class RGBColors {
        public:
          static const Utils::RGB  Red;
          static const Utils::RGB  Green;
          static const Utils::RGB  Blue;
          static const Utils::RGB  Yellow;
          static const Utils::RGB  Cyan;
          static const Utils::RGB  Magenta;
          static const Utils::RGB  White;
          static const Utils::RGB  Black;
          static const Utils::RGB  Orange;
          static const Utils::RGB  Purple;
    };
  }
}

#endif // DIGITAL_LED_COMPONENT_HOME
