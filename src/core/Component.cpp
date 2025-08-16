#include "Component.hpp"
#include "Pin.hpp"
#include "DigitalPin.hpp"
#include "AnalogicPin.hpp"

namespace ArduinoManager
{
  namespace Core
  {

    // Constructor que inicializa el componente con un nombre.
    // También genera un código aleatorio único.
    Component::Component(String name) : name(name)
    {
      randomSeed(analogRead(A0));
      this->code = random(100);
    }

    // Constructor que inicializa el componente con un código específico.
    Component::Component(long code, String name) : name(name), code(code)
    {
    }

    // Destructor: libera la memoria de los pines asociados.
    Component::~Component()
    {
      pins.clear(); // Limpiar la lista para evitar memory leaks
    }

    // Asigna un nuevo nombre al componente.
    void Component::setName(String newName)
    {
      name = newName;
    }

    // Retorna el nombre actual del componente.
    String Component::getName() const
    {
      return name;
    }

    // Retorna el código único del componente.
    long Component::getCode()
    {
      return this->code;
    }

    // Agrega un pin digital al componente con sus metadatos.
    DigitalPin* Component::addDigitalPin(int pin, int mode, String description, int value)
    {
      DigitalPin *pinPtr = new DigitalPin(pin, mode);
      pinPtr->setDescription(description);
      pinPtr->setValue(value);
      pins.add(pinPtr);
      return pinPtr;
    }

    // Agrega un pin analógico al componente con sus metadatos.
    AnalogicPin *Component::addAnalogicPin(int pin, int mode, String description, float value)
    {
      AnalogicPin *pinPtr = new AnalogicPin(pin, mode);
      pinPtr->setDescription(description);
      pinPtr->setValue(value);
      pins.add(pinPtr);
      return pinPtr;
    }

    // Ejecuta la lógica asociada a todos los pines del componente.
    bool Component::performPins()
    {
      bool result = true;
      int count = this->pins.get_size();
      for (int i = 0; i < count; i++)
      {
        Pin *pinptr = (Pin *)this->pins.get(i);
        result &= pinptr->perform();
      }
      return result;
    }

    // Devuelve una representación visual del componente en forma de cuadro con los pines.
    String Component::toString()
    {
      String output = "";
      String content = "";

      // Construir contenido
      content += " Component: " + name + "\n";
      content += " Pins: [";

      int n = pins.get_size();
      for (int i = 0; i < n; i++)
      {
        Pin *pinptr = (Pin *)pins.get(i);
        String valPtr = pinptr->toString();
        content += String(valPtr);
        if (i < n - 1)
          content += ", ";
      }
      content += "]\n";

      // Calcular el ancho máximo de línea
      int maxWidth = 0;
      int lineStart = 0;
      for (int i = 0; i < content.length(); i++)
      {
        if (content[i] == '\n')
        {
          int lineWidth = i - lineStart;
          if (lineWidth > maxWidth)
            maxWidth = lineWidth;
          lineStart = i + 1;
        }
      }

      // Construir bordes
      output += "+"; // esquina superior izquierda
      for (int i = 0; i < maxWidth + 2; i++)
        output += "-";
      output += "+\n";

      // Añadir líneas con márgenes y bordes laterales
      lineStart = 0;
      for (int i = 0; i < content.length(); i++)
      {
        if (content[i] == '\n')
        {
          String line = content.substring(lineStart, i);
          int padding = maxWidth - line.length();
          output += "| " + line;
          for (int j = 0; j < padding; j++)
            output += " ";
          output += " |\n";
          lineStart = i + 1;
        }
      }

      // Borde inferior
      output += "+";
      for (int i = 0; i < maxWidth + 2; i++)
        output += "-";
      output += "+\n";

      return output;
    }
  } // namespace Core
} // namespace ArduinoManager