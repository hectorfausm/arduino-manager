#ifndef COMPONENT_HOME
#define COMPONENT_HOME

#include "../utils/List.hpp"
#include "Pin.hpp"
#include "DigitalPin.hpp"
#include "AnalogicPin.hpp"
#include <Arduino.h>

namespace ArduinoManager
{
  namespace Core
  {

    /**
     * @class Component
     * @brief Clase base para componentes de hardware en el sistema ArduinoManager.
     * 
     * Esta clase representa un componente que contiene un conjunto de pines (digitales o analógicos),
     * un nombre identificador, y un código interno. Ofrece funcionalidades para agregar y manejar
     * los pines asociados al componente.
     */
    class Component
    {
    protected:
      /**
       * @brief Nombre del componente.
       */
      String name;

      /**
       * @brief Lista de pines asociados al componente.
       */
      Utils::List pins;

      /**
       * @brief Código identificador del componente.
       */
      long code;

    public:
      /**
       * @brief Constructor que inicializa el componente con un nombre.
       * 
       * @param name Nombre del componente.
       */
      Component(String name);

      /**
       * @brief Constructor que inicializa el componente con un código y un nombre.
       * 
       * @param code Código del componente.
       * @param name Nombre del componente.
       */
      Component(long code, String name);

      /**
       * @brief Destructor virtual.
       */
      virtual ~Component();

      /**
       * @brief Establece un nuevo nombre para el componente.
       * 
       * @param newName Nuevo nombre a asignar.
       */
      void setName(String newName);

      /**
       * @brief Obtiene el nombre del componente.
       * 
       * @return Nombre actual del componente.
       */
      String getName() const;

      /**
       * @brief Obtiene el código del componente.
       * 
       * @return Código del componente.
       */
      long getCode();

      /**
       * @brief Agrega un pin digital al componente.
       * 
       * @param pin Número del pin en la placa Arduino.
       * @param mode Modo del pin (INPUT, OUTPUT, etc.).
       * @param description Descripción del pin.
       * @param value Valor inicial del pin.
       * @return Puntero al objeto Pin creado y agregado.
       */
      DigitalPin *addDigitalPin(int pin, int mode, String description, int value);

      /**
       * @brief Agrega un pin analógico al componente.
       * 
       * @param pin Número del pin en la placa Arduino.
       * @param mode Modo del pin (INPUT, OUTPUT, etc.).
       * @param description Descripción del pin.
       * @param value Valor inicial del pin (como flotante).
       * @return Puntero al objeto Pin creado y agregado.
       */
      AnalogicPin *addAnalogicPin(int pin, int mode, String description, float value);

      /**
       * @brief Ejecuta acciones asociadas a los pines del componente.
       * 
       * Este método debe ser llamado para actualizar o ejecutar la lógica de los pines.
       */
      bool performPins();

      /**
       * @brief Retorna una representación en cadena del componente.
       * 
       * @return Cadena que representa al componente (nombre, código, etc.).
       */
      String toString();
    };

  } // namespace Core
} // namespace ArduinoManager

#endif // COMPONENT_HOME
