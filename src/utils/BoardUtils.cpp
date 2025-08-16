#include "BoardUtils.hpp"

// Librería de Arduino
#include <Arduino.h>

namespace ArduinoManager
{
  namespace Utils
  {
    long BoardUtils::generateUniqueCode() {
     // Inicializa la semilla solo una vez (puedes mejorar controlando que se llame una vez)
     static bool seeded = false;
     if (!seeded) {
         randomSeed(micros()); // Semilla con microsegundos desde el arranque
         seeded = true;
     }

     // Genera un número aleatorio positivo de 31 bits
     long code = random(0x7FFFFFFF); 
     return code;
    }
  }
}