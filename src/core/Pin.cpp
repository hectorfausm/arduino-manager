#include "Pin.hpp"


namespace ArduinoManager
{
  namespace Core
  {

    // Cosntructor con pin y modo
    Pin::Pin(int pin, int mode) : pin(pin), mode(mode), value(0.0)
    {
      if (mode != INPUT && mode != OUTPUT)
      {
        Serial.println("Error: modo de pin inválido.");
        return;
      }
      pinMode(pin, mode);
    }

    // Constructor con pin y modo por defecto OUTPUT
    void Pin::setMode(int mode)
    {
      if (mode != INPUT && mode != OUTPUT)
      {
        Serial.println("Error: modo de pin inválido.");
        return;
      }
      this->mode = mode;
      pinMode(pin, mode);
    }

    // Obtiene el modo actual
    int Pin::getMode()
    {
      return mode;
    }

    // Obtiene el pin
    int Pin::getPin()
    {
      return pin;
    }

    // Obtiene el valor actual
    float Pin::getValue()
    {
      return value;
    }

    // Establece la descripción del pin
    void Pin::setDescription(String description)
    {
      this->description = description;
    }

    // Obtiene la descripción del pin
    String Pin::getDescription()
    {
      return this->description;
    }

    // Establece el valor del pin
    boolean Pin::setValue(float value)
    {
      this->value = value;
      return true;
    }
  }
}
