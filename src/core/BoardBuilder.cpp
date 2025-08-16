#include "BoardBuilder.hpp"
#include "../boards/ArduinoUnoPins.hpp" // Para acceder a arduinoUnoPins

namespace ArduinoManager
{
  namespace Core
  {
    Board* BoardBuilder::initArduinoUnoBoard()
    {
      Board* board = new Board();
      board->setPinDistribution(Boards::arduinoUnoPins, 26); // 26 pines definidos
      board->setDefaultVcc(Boards::ArduinoUnoPins::DEFAULT_VCC);
      return board;
    }
  } // namespace Core
} // namespace ArduinoManager