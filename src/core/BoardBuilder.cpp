#include "BoardBuilder.hpp"
#include "../boards/ArduinoUnoPins.hpp" // Para acceder a arduinoUnoPins
#include "../boards/D1MiniPins.hpp"

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

    Board* BoardBuilder::initD1MiniBoard()
    {
      Board* board = new Board();
      board->setPinDistribution(Boards::d1MiniPins, Boards::D1MiniPins::PIN_COUNT);
      board->setDefaultVcc(Boards::D1MiniPins::DEFAULT_VCC);
      return board;
    }
  } // namespace Core
} // namespace ArduinoManager
