#ifndef BOARD_BUILDER_HOME
#define BOARD_BUILDER_HOME

#include "Board.hpp"

namespace ArduinoManager
{
    namespace Core
    {

        /**
         * @class BoardBuilder
         * @brief Fábrica de instancias de Board configuradas por tipo de placa.
         */
        class BoardBuilder
        {
        public:
            /**
             * @brief Crea una instancia de Board configurada como Arduino Uno.
             * @return Instancia lista para usar.
             */
            static Board* initArduinoUnoBoard();

            /**
             * @brief Crea una instancia configurada para la placa LOLIN D1 mini (ESP8266).
             * @return Instancia lista para usar.
             */
            static Board* initD1MiniBoard();
        };

    } // namespace Core
} // namespace ArduinoManager

#endif // BOARD_BUILDER_HOME
