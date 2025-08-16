# ArduinoManager 🚀

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Arduino](https://img.shields.io/badge/Arduino-Compatible-blue.svg)](https://www.arduino.cc/)
[![C++](https://img.shields.io/badge/C++-11-red.svg)](https://isocpp.org/)

Una librería moderna y elegante para gestionar pines y componentes de Arduino de forma orientada a objetos.

## ✨ Características

- 🎯 **Orientada a Objetos**: Diseño limpio y modular con clases especializadas
- 🔌 **Gestión Inteligente de Pines**: Soporte completo para pines digitales y analógicos
- 🧩 **Sistema de Componentes**: Agrupa pines relacionados en componentes lógicos
- ⚙️ **Configuración Flexible**: Sistema de configuración basado en máscaras de bits
- 📊 **Tipo Seguro**: Validación automática de valores y rangos
- 🎨 **API Intuitiva**: Sintaxis clara y fácil de usar
- 📋 **Lista Circular**: Estructura de datos optimizada incluida

## 🚀 Instalación Rápida

### Opción 1: Arduino IDE
1. Descarga el archivo `.zip` del repositorio
2. Ve a **Sketch** → **Include Library** → **Add .ZIP Library**
3. Selecciona el archivo descargado

### Opción 2: PlatformIO
```bash
pio lib install "ArduinoManager"
```

### Opción 3: Git Clone
```bash
git clone https://github.com/tu-usuario/ArduinoManager.git
```

## 💡 Uso Básico

### Incluir la librería
```cpp
#include "ArduinoManager.hpp"
```

### LED Digital Simple
```cpp
#include "ArduinoManager.hpp"

DigitalPin led(13, OUTPUT);

void setup() {
    led.setValue(1.0); // Encender LED
    led.perform();
}

void loop() {
    // LED permanece encendido
}
```

### Sensor Analógico
```cpp
#include "ArduinoManager.hpp"

AnalogicPin sensor(A0, INPUT);

void setup() {
    Serial.begin(9600);
}

void loop() {
    sensor.perform();
    float valor = sensor.getValue();
    
    Serial.print("Sensor: ");
    Serial.println(valor);
    delay(1000);
}
```

## 🔧 Ejemplos Avanzados

### Componente LED RGB
```cpp
#include "ArduinoManager.hpp"

void setup() {
    Component ledRGB("LED RGB");
    
    // Agregar pines de colores
    Pin* rojo = ledRGB.addDigitalPin(9, OUTPUT, "Rojo", 0);
    Pin* verde = ledRGB.addDigitalPin(10, OUTPUT, "Verde", 0);
    Pin* azul = ledRGB.addDigitalPin(11, OUTPUT, "Azul", 0);
    
    // Color púrpura (rojo + azul)
    rojo->setValue(1.0);
    azul->setValue(1.0);
    
    ledRGB.performPins();
    
    Serial.begin(9600);
    Serial.println(ledRGB.toString());
}
```

### Control PWM Suave
```cpp
#include "ArduinoManager.hpp"

AnalogicPin pwmPin(6, OUTPUT);

void setup() {
    // Configuración inicial
}

void loop() {
    // Fade in
    for(float i = 0.0; i <= 1.0; i += 0.01) {
        pwmPin.setValue(i);
        pwmPin.perform();
        delay(10);
    }
    
    // Fade out
    for(float i = 1.0; i >= 0.0; i -= 0.01) {
        pwmPin.setValue(i);
        pwmPin.perform();
        delay(10);
    }
}
```

### Sistema Multi-Sensor
```cpp
#include "ArduinoManager.hpp"

void setup() {
    List sensores;
    
    // Crear sensores
    AnalogicPin* luz = new AnalogicPin(A0, INPUT);
    AnalogicPin* temperatura = new AnalogicPin(A1, INPUT);
    AnalogicPin* humedad = new AnalogicPin(A2, INPUT);
    
    luz->setDescription("Sensor de Luz");
    temperatura->setDescription("Sensor de Temperatura");
    humedad->setDescription("Sensor de Humedad");
    
    // Agregar a la lista
    sensores.add(luz);
    sensores.add(temperatura);
    sensores.add(humedad);
    
    Serial.begin(9600);
    
    // Leer todos los sensores
    for(int i = 0; i < sensores.get_size(); i++) {
        AnalogicPin* sensor = (AnalogicPin*)sensores.get(i);
        sensor->perform();
        
        Serial.print(sensor->getDescription());
        Serial.print(": ");
        Serial.println(sensor->getValue());
    }
}
```

## 📚 Arquitectura

### Jerarquía de Clases

```
Pin (abstracta)
├── DigitalPin
└── AnalogicPin

Component
├── Lista de Pines
└── Métodos de gestión

BoardBuilder
├── Configuración de Pines
└── Lista de Componentes
```

### Namespaces

- `ArduinoManager::Core` - Clases principales
- `ArduinoManager::Components` - Componentes específicos  
- `ArduinoManager::Config` - Configuraciones
- `ArduinoManager::Utils` - Utilidades
- `ArduinoManager::Boards` - Definiciones de placas

## 🎯 Funciones de Pin Soportadas

| Función | Descripción | Valor |
|---------|-------------|-------|
| `FUNC_D_IO` | E/S Digital | `1 << 2` |
| `FUNC_A_IO` | E/S Analógica | `1 << 9` |
| `FUNC_PWM` | Salida PWM | `1 << 3` |
| `FUNC_UART_RX/TX` | Comunicación serie | `1 << 0/1` |
| `FUNC_SPI_*` | Comunicación SPI | `1 << 4-7` |
| `FUNC_I2C_*` | Comunicación I2C | `1 << 10-11` |
| `FUNC_POWER` | Alimentación (5V, 3V3, GND) | `1 << 13-15` |

## 🛠️ API Rápida

### Pin Digital
```cpp
DigitalPin pin(13, OUTPUT);
pin.setValue(1.0);     // HIGH
pin.setValue(0.0);     // LOW
pin.perform();         // Ejecutar
```

### Pin Analógico
```cpp
AnalogicPin pin(A0, INPUT);
pin.setValue(0.5);     // 50% (solo OUTPUT)
pin.perform();         // Leer/Escribir
float valor = pin.getValue();
```

### Componente
```cpp
Component comp("Mi Componente");
Pin* p1 = comp.addDigitalPin(12, OUTPUT, "LED", 0);
Pin* p2 = comp.addAnalogicPin(A1, INPUT, "Sensor", 0.0);
comp.performPins();    // Ejecutar todos los pines
```

## 📖 Documentación Completa

Para documentación detallada de la API, consulta:
- 📘 [Documentación Completa](docs/extras/API.md)
- 🔍 [Ejemplos Avanzados](examples/)
- 🎓 [Guía de Inicio](docs/getting-started.md)

## 🤝 Contribuir

¡Las contribuciones son bienvenidas! Por favor:

1. Haz fork del proyecto
2. Crea una rama para tu feature (`git checkout -b feature/nueva-funcionalidad`)
3. Commit tus cambios (`git commit -am 'Agregar nueva funcionalidad'`)
4. Push a la rama (`git push origin feature/nueva-funcionalidad`)
5. Abre un Pull Request

### Pautas de Contribución

- Seguir el estilo de código existente
- Incluir documentación para nuevas funcionalidades
- Agregar tests cuando sea posible
- Mantener compatibilidad con Arduino IDE

## 📋 Requisitos

- **Arduino IDE**: 1.8.x o superior
- **PlatformIO**: Compatible
- **C++ Standard**: C++11 o superior
- **Placas soportadas**: Arduino Uno, Nano, ESP32, ESP8266, y más

## 🐛 Reporte de Bugs

¿Encontraste un bug? [Abre un issue](https://github.com/tu-usuario/ArduinoManager/issues) con:

- Descripción del problema
- Código para reproducir el error
- Placa y versión de Arduino IDE
- Comportamiento esperado vs. actual

## 📊 Roadmap

- [ ] 🎯 v1.0.1: Implementación de compomentes

## 📄 Licencia

Este proyecto está bajo la Licencia MIT. Ver [LICENSE](LICENSE) para más detalles.

```
MIT License

Copyright (c) 2025 ArduinoManager

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
```

## 🌟 Agradecimientos

- Comunidad Arduino por la inspiración
- Contribuidores del proyecto
- Usuarios que reportan bugs y sugieren mejoras

## 📞 Contacto

- 📧 Email: tu-email@ejemplo.com

---

<div align="center">

**⭐ Si te gusta el proyecto, dale una estrella ⭐**

*Hecho con ❤️ para la comunidad Arduino*

</div>