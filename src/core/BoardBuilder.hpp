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
        };

    } // namespace Core
} // namespace ArduinoManager

#endif // BOARD_BUILDER_HOME