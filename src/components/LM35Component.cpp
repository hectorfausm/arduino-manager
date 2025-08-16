#include "../core/Component.hpp"
#include "LM35Component.hpp"

namespace ArduinoManager
{
  namespace Components
  {

    LM35Component::LM35Component(long code, float vcc, int pin): Core::Component(code, "LM35")
    {
      this->readPin = addAnalogicPin(pin, INPUT, name, 0);
      this->vcc = vcc;
    }

    float LM35Component::readTemperature(){
      if(this->readPin->perform()){
        return this->readPin->getValue() * this->vcc * 100.0;
      }
      return -1.0;
    }
  }
}