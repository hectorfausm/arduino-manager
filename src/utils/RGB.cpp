#include "RGB.hpp"

namespace ArduinoManager
{
  namespace Utils
  {
    RGB::RGB() : r(0), g(0), b(0) {}

    RGB::RGB(int red, int green, int blue) : r(red), g(green), b(blue) {}
  }
}