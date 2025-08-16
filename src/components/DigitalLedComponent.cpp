#include "DigitalLedComponent.hpp"

namespace ArduinoManager
{
  namespace Components
  {

    // Constructor de un led digital. El código es obligatorio
    DigitalLedComponent::DigitalLedComponent(long code, int pin): Core::Component(code, "Digital LED")
    {
      ledPin = addDigitalPin(pin, OUTPUT, name, 0);
    }

    bool DigitalLedComponent::turnOn()
    {
      return ledPin->setValue(1.0f) && performPins();
    }

    bool DigitalLedComponent::turnOff()
    {
      return ledPin->setValue(0.0f) && performPins();
    }

    void DigitalLedComponent::toggle()
    {
      if (isOn())
      {
        turnOff();
      }
      else
      {
        turnOn();
      }
    }

    bool DigitalLedComponent::perform()
    {
      return performPins();
    }

    bool DigitalLedComponent::isOn() const
    {
      return ledPin->getValue() > 0.5f;
    }

  } // namespace Components
} // namespace ArduinoManager