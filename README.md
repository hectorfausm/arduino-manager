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
- 📡 **WiFi ESP8266 opcional**: Provisión de credenciales y registro REST para D1 mini

## 🚀 Instalación Rápida

### Opción 1: Arduino IDE
1. Descarga el archivo `.zip` del repositorio
2. Ve a **Sketch** → **Include Library** → **Add .ZIP Library**
3. Selecciona el archivo descargado

### Opción 2: PlatformIO

Añade la biblioteca al proyecto en `platformio.ini` (consulta la
[referencia de `lib_deps` de PlatformIO](https://docs.platformio.org/en/latest/projectconf/sections/env/options/library/lib_deps.html)):

```ini
[env:d1_mini]
platform = espressif8266
board = d1_mini
framework = arduino
lib_deps =
    https://github.com/hectorfausm/arduino-manager.git
```

### Opción 3: Git Clone
```bash
git clone https://github.com/hectorfausm/arduino-manager.git
```

## 💡 Uso Básico

### Incluir la librería
```cpp
#include "ArduinoManager.hpp"
```

### Crear una placa D1 mini
```cpp
Board* board;
DigitalLedComponent* led;

void setup() {
    board = BoardBuilder::initD1MiniBoard();
    led = board->addDigitalLedComponent(D1MiniPins::D5);
}
```

Las etiquetas `D0`–`D8` usan los números GPIO del ESP8266 en la configuración
de la placa. La D1 mini trabaja con lógica de 3,3 V y su entrada `A0` admite
hasta 3,2 V. Consulta la [referencia de pines y capacidades](src/extras/docs/API.md#boardbuilder)
antes de conectar señales externas.

## Conectividad WiFi en ESP8266

El módulo de provisión y registro es opcional y está dirigido a ESP8266, incluida
la D1 mini. Incluye `ArduinoManagerESP8266.hpp` explícitamente; `ArduinoManager.hpp`
no importa dependencias ESP8266. El core ESP8266 aporta `ESP8266WiFi`,
`ESP8266WebServer`, EEPROM y el cliente HTTP.

Consulta el sketch [`d1mini-wifi-resource.ino`](src/examples/d1mini-wifi-resource.ino)
y la [guía de API y configuración WiFi](src/extras/docs/API.md#esp8266-wi-fi-resource)
para configurar el AP de provisión, el host y puerto REST y el puerto local del
recurso. No incluyas contraseñas ni tokens en el código fuente.

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
- `ArduinoManager::Network` - Conectividad opcional para ESP8266

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
Pin* p2 = comp.addAnalogicPin(A1, INPUT, "Component", 0.0);
comp.performPins();    // Ejecutar todos los pines
```

## 📖 Documentación Completa

Para documentación detallada de la API, consulta:
- 📘 [Referencia de la API](src/extras/docs/API.md)
- 🔍 [Sketches de ejemplo](src/examples/)

## 🤝 Contribuir

¡Las contribuciones son bienvenidas! Por favor:

1. Haz fork del proyecto
2. Crea una rama para tu feature (`git checkout -b feature/nueva-funcionalidad`)
3. Commit tus cambios (`git commit -am 'Agregar nueva funcionalidad'`)
4. Push a la rama (`git push origin feature/nueva-funcionalidad`)
5. Abre un Pull Request en [hectorfausm/arduino-manager](https://github.com/hectorfausm/arduino-manager)

### Pautas de Contribución

- Seguir el estilo de código existente
- Incluir documentación para nuevas funcionalidades
- Agregar tests cuando sea posible
- Mantener compatibilidad con Arduino IDE

## Buenas prácticas

Estas pautas aplican al desarrollar la biblioteca y al integrarla en un sketch:

- **Trabaja desde la raíz de la biblioteca.** Los metadatos de Arduino están en `library.properties`; el código público está en `src/` y los sketches de ejemplo en `src/examples/`. Mantén cada clase en su área (`core/`, `components/`, `boards/`, `config/` o `utils/`) y usa `ArduinoManager.hpp` como cabecera agregadora.
- **Sigue el estilo y la compatibilidad existentes.** La biblioteca declara C++11. Usa nombres y namespaces coherentes con el módulo, incluye las dependencias que cada cabecera necesita y evita depender de detalles internos cuando la API pública sea suficiente.
- **Respeta la semántica de los pines.** Comprueba que el pin elegido admite la función requerida en la placa objetivo. `DigitalPin` acepta `0.0` o `1.0`; `AnalogicPin` representa valores de salida normalizados entre `0.0` y `1.0`, mientras que las lecturas analógicas también se guardan normalizadas. En Arduino Uno, las salidas PWM solo están disponibles en los pines compatibles con PWM.
- **Configura el hardware una vez y actualízalo de forma explícita.** Crea la placa y los componentes en `setup()`. En `loop()`, llama a los métodos del componente cuando necesites leer o cambiar el hardware; para tareas periódicas, usa `millis()` en vez de bloquear el programa con `delay()`.
- **Comprueba los resultados y los límites del hardware.** Valida los punteros que devuelven los métodos de alta de componentes y los valores de componentes antes de usarlos. Configura correctamente la tensión de referencia al calcular medidas analógicas y confirma el tipo de LED RGB (ánodo o cátodo común) y el orden de sus pines.
- **Cuida la memoria y el tiempo de ejecución.** Las placas AVR suelen tener RAM limitada: evita asignaciones dinámicas repetidas y `String` de gran tamaño en bucles frecuentes. Las lecturas del DHT11 tienen un intervalo mínimo de dos segundos y pueden bloquear mientras se comunica con el componente.
- **Mantén sincronizados código, ejemplos y documentación.** Si cambias una firma pública o el comportamiento de un componente, actualiza `src/extras/docs/API.md` y un ejemplo pertinente. No presentes como implementada una capacidad que el código todavía no ofrece.
- **Verifica los cambios en la placa adecuada.** Compila los ejemplos afectados para la placa objetivo antes de integrar el cambio y, cuando sea posible, comprueba su comportamiento con el hardware. Documenta la placa y las conexiones necesarias para reproducir la prueba.

## 📋 Requisitos

- **Arduino IDE**: 1.8.x o superior
- **PlatformIO**: Compatible
- **C++ Standard**: C++11 o superior
- **Perfiles de placa incluidos**: Arduino Uno y LOLIN/WEMOS D1 mini (ESP8266).
  Las clases genéricas de pines pueden usarse con otras placas, pero este proyecto
  no incluye mapas de pines para Nano o ESP32.

## 🐛 Reporte de Bugs

¿Encontraste un bug? [Abre un issue](https://github.com/hectorfausm/arduino-manager/issues) con:

- Descripción del problema
- Código para reproducir el error
- Placa y versión de Arduino IDE
- Comportamiento esperado vs. actual

## 📊 Roadmap

- [x] 🎯 v1.0.1: Implementación de componentes principales (LED, RGB, LM35 y DHT11)

## 📄 Licencia

Este proyecto está bajo la Licencia MIT. El texto completo de la licencia se incluye a continuación.

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


## Estado de componentes y telemetría ESP8266

Los componentes físicos se publican con sus valores agrupados. Para un DHT11:

```cpp
ArduinoManager::Components::DHT11Component climate(1, 4); // D2/GPIO4
// En setup():
climate.begin();
bool added = wifiResource.addDHT11Component("climate", "temperature", "humidity",
    ComponentUpdatePolicy::PERIODIC, 10, climate);
// Comprueba added antes de wifiResource.begin().
```

Temperatura y humedad proceden de una adquisición común y siempre viajan juntas:

```json
{"components":[{"attr":"climate","values":[{"attr":"temperature","value":24,"healthy":true,"updatePolicy":"PERIODIC","expectedIntervalSeconds":10},{"attr":"humidity","value":60,"healthy":true,"updatePolicy":"PERIODIC","expectedIntervalSeconds":10}]}]}
```

`GET /status` incluye todos los componentes. La telemetría POST incluye los pendientes,
siempre completos; un fallo HTTP conserva el conjunto para reintentarlo cada 3 s.
Un fallo de adquisición del DHT11 devuelve ambos valores con `healthy:false` y
`value:null`; los getters devuelven `NAN` durante la caché de ese intento fallido.

La firma escalar `addComponent(attr, policy, intervalSeconds, reader, writer)` sigue
funcionando para LED, LM35 y otros componentes: su salida también usa `values`.
`addCompositeComponent` permite otros lectores conjuntos con escritores por canal.
Cada componente tiene una única política y cadencia. Para migrar un DHT11, sustituye
sus dos altas escalares por una llamada `addDHT11Component`; utiliza un intervalo
común de 2–86400 segundos para `PERIODIC`, o 0 para `ON_CHANGE` (muestreo mínimo
2000 ms). No registres sus canales por separado. Los nombres de valores deben
ser únicos en la placa y coincidir con el alta administrativa en domo.

Se conservan provisión, token, HTTP Basic y PATCH de `/localUrl`. Los actuadores
aceptan `POST /ATRIBUTO?value=NUMERO` validado; los lectores sin escritor responden
405. El [ejemplo WiFi](src/examples/d1mini-wifi-resource.ino) incluye DHT11 en
D2/GPIO4, LED y una alternativa LM35. Comprueba siempre las altas antes de `begin()`.

Consulta la [API](src/extras/docs/API.md#estado-de-componentes-y-telemetría-esp8266)
para firmas, validación, planificación y comandos de pruebas. Tras sincronizar
la biblioteca instalada, ejecuta `python3 tests/test_components.py` (C++ real con HAL
y HTTP controlados) y `python3 tests/compile_esp8266.py` (objetos Xtensa con core
3.1.2, sin enlace ni carga). Si la instalación es de solo lectura, ambos aceptan
`ARDUINO_MANAGER_LIBRARY` apuntando a una copia sincronizada alternativa.
