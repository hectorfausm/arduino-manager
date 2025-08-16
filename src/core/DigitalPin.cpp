#include "DigitalPin.hpp"

namespace ArduinoManager
{
  namespace Core
  {

    // Establece el valor del pin digital
    boolean DigitalPin::setValue(float value)
    {
      if (value == 0.0f)
      {
        Pin::setValue(0.0f);
        return true;
      }
      else if (value == 1.0f)
      {
        Pin::setValue(1.0f);
        return true;
      }
      Serial.println("[Error] DigitalPin.setValue() - Out of Range (0.0 LOW o 1.0 HIGH).");
      return false;
    }

    // Ejecuta la acción para el pin digital
    boolean DigitalPin::perform()
    {
      if (getMode() == INPUT)
      {
        int val = digitalRead(getPin());
        Pin::setValue((float)val);
        Serial.println("[INFO] Digital.perform() - Obtain value: " + String(getValue() == 0.0f ? LOW : HIGH) + " from Pin: " + String(getPin()));
        return true;
      }
      else if (getMode() == OUTPUT)
      {
        Serial.println("[INFO] Digital.perform() -  Expose: " + String(getValue() == 0.0f ? LOW : HIGH) + " to Pin: " + String(getPin()));
        digitalWrite(getPin(), getValue() == 0.0f ? LOW : HIGH);
        return true;
      }
      Serial.println("[Error] AnalogicPin.perform() - Out of Range (0.0 - 1.0).");
      return false;
    }

    // Muestra el valro del pin digital
    String DigitalPin::toString()
    {
      return String(getPin()) + ":" + String(getValue()) + String(getMode());
    }
  }
}