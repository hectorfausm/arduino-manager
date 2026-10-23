#include "../core/Component.hpp"
#include "../core/AnalogicPin.hpp"
#include "RGBLedComponent.hpp"
#include "../utils/RGB.hpp"

namespace ArduinoManager
{
  namespace Components
  {
    RGBLedComponent::RGBLedComponent(long code, int pinR, int pinG, int pinB, bool cathode): Core::Component(code, "RGB LED"){
      rPin = addAnalogicPin(pinR, OUTPUT, name + "_R", 0);
      gPin = addAnalogicPin(pinG, OUTPUT, name + "_G", 0);
      bPin = addAnalogicPin(pinB, OUTPUT, name + "_B", 0);
      this->cathode = cathode;
      turnOff();
    }

    bool RGBLedComponent::turnRGBColor(int r, int g, int b){
      bool result = false;

      // Modo cátodo
      if(cathode){
        result = rPin->setValue((float)r/255.0f) && gPin->setValue((float)g/255.0f) && bPin->setValue((float)b/255.0f);
      }

      // Modo ánodo
      else{
        result = rPin->setValue(1.0f-((float)r/255.0f)) && gPin->setValue(1.0f-((float)g/255.0f)) && bPin->setValue(1.0f-((float)b/255.0f));
      }
      
      if (result) {
        ledColor =  Utils::RGB(r, g, b);
      }
      return result && performPins();
    }
  
    bool RGBLedComponent::turnRGBColor(Utils::RGB color){
      return turnRGBColor(color.r, color.g, color.b);
    }

    bool RGBLedComponent::turnOff(){
      return turnRGBColor(RGBColors::Black);
    }

    /**
     * @brief Obtiene el color RGB actual del LED.
     * 
     * @return Color RGB en el LED.
     */
    Utils::RGB RGBLedComponent::getRGBColor(){
      return ledColor;
    }

    const Utils::RGB RGBColors::Red     = Utils::RGB(255, 0, 0);
    const Utils::RGB RGBColors::Green   = Utils::RGB(0, 255, 0);
    const Utils::RGB RGBColors::Blue    = Utils::RGB(0, 0, 255);
    const Utils::RGB RGBColors::Yellow  = Utils::RGB(255, 255, 0);
    const Utils::RGB RGBColors::Cyan    = Utils::RGB(0, 255, 255);
    const Utils::RGB RGBColors::Magenta = Utils::RGB(255, 0, 255);
    const Utils::RGB RGBColors::White   = Utils::RGB(255, 255, 255);
    const Utils::RGB RGBColors::Black   = Utils::RGB(0, 0, 0);
    const Utils::RGB RGBColors::Orange  = Utils::RGB(255, 165, 0);
    const Utils::RGB RGBColors::Purple  = Utils::RGB(128, 0, 128);
  }
}
