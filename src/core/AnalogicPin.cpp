#include "AnalogicPin.hpp"

namespace ArduinoManager
{
  namespace Core
  {

    boolean AnalogicPin::setValue(float value)
    {
      if (value < 0.0 || value > 1.0)
      {
        Serial.println("[Error] AnalogicPin.setValue() - Out of Range (0.0 - 1.0).");
        return false;
      }
      // Escalamos de [0.0, 1.0] a [0, 255] para analogWrite
      return Pin::setValue(value);
    }

    boolean AnalogicPin::perform()
    {
      if (getMode() == INPUT)
      {
        int raw = analogRead(getPin());
        float normalized = raw / 1023.0;
        Pin::setValue(normalized);
        return true;
      }
      else if (getMode() == OUTPUT)
      {
        int pwm = (int)(getValue() * 255);
        analogWrite(getPin(), pwm);
        Serial.println("[TRACE] AnalogicPin.perform(): " + toString());
        return true;
      }
      Serial.println("[Error] AnalogicPin.perform() - Not implemented mode");
      return false;
    }

    String AnalogicPin::toString()
    {
      return String(getPin()) + ":" + String(getValue()) + ":" + String(getMode());
    }
  }
}