# ArduinoManager API Reference

## Tabla de Contenidos

- [Instalación](#instalación)
- [Uso Básico](#uso-básico)
- [Namespaces](#namespaces)
- [Core Classes](#core-classes)
  - [Pin](#pin)
  - [DigitalPin](#digitalpin)
  - [AnalogicPin](#analogicpin)
  - [Component](#component)
  - [BoardBuilder](#boardbuilder)
- [Configuration](#configuration)
  - [PinConf](#pinconf)
  - [PinFunction](#pinfunction)
- [Utilities](#utilities)
  - [List](#list)
- [Ejemplos](#ejemplos)

## Instalación

```cpp
#include "ArduinoManager.hpp"

// Los siguientes namespaces están disponibles automáticamente:
// AM = ArduinoManager
// using namespace ArduinoManager::Core;
// using namespace ArduinoManager::Components;
// using namespace ArduinoManager::Boards;
// using namespace ArduinoManager::Config;
// using namespace ArduinoManager::Utils;
```

## Uso Básico

```cpp
void setup() {
    // Crear un pin digital
    DigitalPin led(13, OUTPUT);
    led.setValue(1.0); // HIGH
    led.perform();
    
    // Crear un pin analógico
    AnalogicPin sensor(A0, INPUT);
    sensor.perform();
    float valor = sensor.getValue();
}
```

## Namespaces

La librería está organizada en los siguientes namespaces:

- `ArduinoManager::Core` - Clases principales
- `ArduinoManager::Components` - Componentes específicos
- `ArduinoManager::Boards` - Configuraciones de placas
- `ArduinoManager::Config` - Configuraciones y enumeraciones
- `ArduinoManager::Utils` - Utilidades auxiliares

## Core Classes

### Pin

Clase base abstracta para representar un pin genérico de Arduino.

#### Constructor

```cpp
Pin(int pin, int mode)
```

**Parámetros:**
- `pin`: Número físico del pin
- `mode`: Modo del pin (INPUT, OUTPUT, etc.)

#### Métodos Públicos

##### `void setMode(int mode)`
Establece el modo del pin.

**Parámetros:**
- `mode`: Nuevo modo (INPUT, OUTPUT, etc.)

##### `int getMode() const`
Obtiene el modo actual del pin.

**Retorna:** El modo actual del pin

##### `int getPin() const`
Obtiene el número físico del pin.

**Retorna:** El número de pin

##### `float getValue() const`
Obtiene el último valor leído o asignado al pin.

**Retorna:** Valor flotante asociado al pin

##### `void setDescription(const String& description)`
Establece la descripción del pin.

**Parámetros:**
- `description`: Texto descriptivo del pin

##### `String getDescription() const`
Obtiene la descripción del pin.

**Retorna:** Descripción actual del pin

##### `virtual boolean setValue(float value)`
Establece el valor del pin.

**Parámetros:**
- `value`: Nuevo valor para el pin

**Retorna:** `true` si el valor fue aceptado, `false` en caso contrario

##### `virtual boolean perform() = 0`
Ejecuta una acción asociada al pin (método abstracto).

**Retorna:** `true` si la acción fue realizada correctamente

##### `virtual String toString() = 0`
Devuelve una representación en cadena del estado del pin (método abstracto).

**Retorna:** Cadena descriptiva del pin

---

### DigitalPin

Representa un pin digital de Arduino. Hereda de `Pin`.

#### Constructor

```cpp
DigitalPin(int pin, int mode)
```

**Parámetros:**
- `pin`: Número físico del pin
- `mode`: Modo del pin (INPUT, OUTPUT, etc.)

#### Métodos

##### `boolean setValue(float value) override`
Establece el valor del pin digital.

**Parámetros:**
- `value`: 0.0 para LOW, 1.0 para HIGH

**Retorna:** `true` si el valor es válido, `false` en caso contrario

**Nota:** Solo acepta valores 0.0 o 1.0

##### `boolean perform() override`
Ejecuta la acción correspondiente al pin según su modo:
- INPUT: Lee el valor digital y lo almacena internamente
- OUTPUT: Escribe el valor almacenado al pin

**Retorna:** `true` si la operación fue exitosa

##### `String toString() override`
Devuelve una representación en cadena del estado del pin digital.

---

### AnalogicPin

Representa un pin analógico de Arduino. Hereda de `Pin`.

#### Constructor

```cpp
AnalogicPin(int pin, int mode)
```

**Parámetros:**
- `pin`: Número físico del pin
- `mode`: Modo del pin (INPUT, OUTPUT, etc.)

#### Métodos

##### `boolean setValue(float value) override`
Establece el valor del pin analógico.

**Parámetros:**
- `value`: Valor flotante entre 0 y 1

**Retorna:** `true` si el valor es válido, `false` si está fuera del rango [0,1]

##### `boolean perform() override`
Ejecuta la acción correspondiente al pin según su modo:
- INPUT: Lee el valor analógico y lo almacena internamente
- OUTPUT: Escribe el valor almacenado al pin analógico

**Retorna:** `true` si la operación fue exitosa

##### `String toString() override`
Devuelve una representación en cadena del estado del pin analógico.

---

### Component

Clase base para componentes de hardware que contienen múltiples pines.

#### Constructores

```cpp
Component(String name)
Component(int code, String name)
```

**Parámetros:**
- `name`: Nombre del componente
- `code`: Código identificador del componente (opcional)

#### Métodos Públicos

##### `void setName(String newName)`
Establece un nuevo nombre para el componente.

##### `String getName() const`
Obtiene el nombre del componente.

##### `int getCode()`
Obtiene el código del componente.

##### `Pin* addDigitalPin(int pin, int mode, String description, int value)`
Agrega un pin digital al componente.

**Parámetros:**
- `pin`: Número del pin en la placa Arduino
- `mode`: Modo del pin (INPUT, OUTPUT, etc.)
- `description`: Descripción del pin
- `value`: Valor inicial del pin

**Retorna:** Puntero al objeto Pin creado

##### `Pin* addAnalogicPin(int pin, int mode, String description, float value)`
Agrega un pin analógico al componente.

**Parámetros:**
- `pin`: Número del pin en la placa Arduino
- `mode`: Modo del pin (INPUT, OUTPUT, etc.)
- `description`: Descripción del pin
- `value`: Valor inicial del pin (flotante)

**Retorna:** Puntero al objeto Pin creado

##### `void performPins()`
Ejecuta acciones asociadas a todos los pines del componente.

##### `String toString()`
Retorna una representación en cadena del componente.

---

### BoardBuilder

Clase para construir una placa Arduino con componentes y distribución de pines.

#### Métodos Privados

##### `Component* addDigitalLedComponent(String name)`
Añade un componente LED digital.

##### `Component* addAnalogicalLedComponent(String name)`
Añade un componente LED analógico.

---

## Configuration

### PinConf

Representa la configuración básica de un pin de placa.

#### Constructor

```cpp
constexpr PinConf(const char* name, int number, unsigned int functions)
```

**Parámetros:**
- `name`: Nombre del pin (ej. "D0", "A1")
- `number`: Número lógico o físico del pin (puede ser negativo para VIN, GND)
- `functions`: Máscara de funciones válidas (ver PinFunction)

#### Miembros Públicos

- `const char* name` - Nombre del pin
- `const int number` - Número del pin
- `const unsigned int functions` - Máscara de funciones

---

### PinFunction

Enumeración que representa las posibles funciones de un pin mediante máscara de bits.

#### Valores

```cpp
enum PinFunction {
    FUNC_NONE = 0,              // Sin función asignada
    FUNC_RX = 1 << 0,           // UART: recepción de datos
    FUNC_TX = 1 << 1,           // UART: transmisión de datos
    FUNC_D_IO = 1 << 2,         // Entrada o salida digital
    FUNC_PWM = 1 << 3,          // Salida PWM
    FUNC_SPI_SS = 1 << 4,       // SPI: selección de esclavo
    FUNC_SPI_MOSI = 1 << 5,     // SPI: Master Out Slave In
    FUNC_SPI_MISO = 1 << 6,     // SPI: Master In Slave Out
    FUNC_SPI_SCK = 1 << 7,      // SPI: señal de reloj
    FUNC_LED_BUILTIN = 1 << 8,  // LED integrado en la placa
    FUNC_A_IO = 1 << 9,         // Entrada o salida analógica
    FUNC_I2C_SDA = 1 << 10,     // I2C: línea de datos
    FUNC_I2C_SCL = 1 << 11,     // I2C: línea de reloj
    FUNC_VIN = 1 << 12,         // Entrada de voltaje a la placa
    FUNC_5V = 1 << 13,          // Salida de 5V
    FUNC_3V3 = 1 << 14,         // Salida de 3.3V
    FUNC_GND = 1 << 15,         // Tierra (masa)
    FUNC_AREF = 1 << 16,        // Referencia analógica externa
    FUNC_RESET = 1 << 17        // Pin de reinicio del microcontrolador
};
```

#### Uso

Las funciones pueden combinarse usando operaciones bitwise:

```cpp
unsigned int funciones = FUNC_D_IO | FUNC_PWM; // Pin digital con PWM
if (pin.functions & FUNC_PWM) {
    // El pin soporta PWM
}
```

---

## Utilities

### List

Lista enlazada circular para almacenar punteros genéricos.

#### Constructor y Destructor

```cpp
List()    // Constructor
~List()   // Destructor
```

#### Métodos Públicos

##### `bool add(void* value)`
Agrega un elemento al final de la lista.

**Parámetros:**
- `value`: Puntero al valor a agregar

**Retorna:** `true` si se agregó correctamente

##### `bool add_first(void* value)`
Agrega un elemento al inicio de la lista.

**Parámetros:**
- `value`: Puntero al valor a agregar

**Retorna:** `true` si se agregó correctamente

##### `void* pop()`
Elimina y devuelve el primer elemento de la lista.

**Retorna:** Puntero al valor eliminado

##### `void* get(int index)`
Devuelve el valor en un índice específico.

**Parámetros:**
- `index`: Índice del elemento

**Retorna:** Puntero al valor en el índice

##### `void clear()`
Limpia toda la lista, eliminando todos los elementos.

##### `int get_size()`
Devuelve el tamaño actual de la lista.

**Retorna:** Número de elementos en la lista

---

## Ejemplos

### Ejemplo 1: LED Digital Simple

```cpp
#include "ArduinoManager.hpp"

void setup() {
    DigitalPin led(13, OUTPUT);
    led.setDescription("LED incorporado");
    led.setValue(1.0); // Encender
    led.perform();
    
    Serial.begin(9600);
    Serial.println(led.toString());
}

void loop() {
    // LED permanece encendido
}
```

### Ejemplo 2: Sensor Analógico

```cpp
#include "ArduinoManager.hpp"

AnalogicPin sensor(A0, INPUT);

void setup() {
    Serial.begin(9600);
    sensor.setDescription("Sensor de luz");
}

void loop() {
    sensor.perform(); // Leer valor
    float valor = sensor.getValue();
    
    Serial.print("Valor del sensor: ");
    Serial.println(valor);
    
    delay(1000);
}
```

### Ejemplo 3: Componente con Múltiples Pines

```cpp
#include "ArduinoManager.hpp"

void setup() {
    Component rgb("LED RGB");
    
    // Agregar pines para los colores
    Pin* rojo = rgb.addDigitalPin(9, OUTPUT, "Canal Rojo", 0);
    Pin* verde = rgb.addDigitalPin(10, OUTPUT, "Canal Verde", 0);
    Pin* azul = rgb.addDigitalPin(11, OUTPUT, "Canal Azul", 0);
    
    // Establecer color púrpura
    rojo->setValue(1.0);
    azul->setValue(1.0);
    
    // Activar todos los pines
    rgb.performPins();
    
    Serial.begin(9600);
    Serial.println(rgb.toString());
}
```

### Ejemplo 4: Uso de Lista

```cpp
#include "ArduinoManager.hpp"

void setup() {
    List listaPines;
    
    // Crear pines
    DigitalPin* led1 = new DigitalPin(12, OUTPUT);
    DigitalPin* led2 = new DigitalPin(13, OUTPUT);
    
    // Agregar a la lista
    listaPines.add(led1);
    listaPines.add(led2);
    
    // Usar los pines
    for(int i = 0; i < listaPines.get_size(); i++) {
        DigitalPin* pin = (DigitalPin*)listaPines.get(i);
        pin->setValue(1.0);
        pin->perform();
    }
}
```

### Ejemplo 5: Configuración de Pin con Funciones

```cpp
#include "ArduinoManager.hpp"

void setup() {
    // Definir configuración de pin con múltiples funciones
    PinConf pinConfig("D9", 9, FUNC_D_IO | FUNC_PWM);
    
    // Verificar si el pin soporta PWM
    if(pinConfig.functions & FUNC_PWM) {
        AnalogicPin pwmPin(pinConfig.number, OUTPUT);
        pwmPin.setValue(0.5); // 50% duty cycle
        pwmPin.perform();
    }
}
```

---

## Notas Importantes

1. **Gestión de Memoria**: Los objetos Pin creados con `addDigitalPin()` y `addAnalogicPin()` son gestionados internamente por el Component.

2. **Valores Válidos**: 
   - DigitalPin acepta solo 0.0 (LOW) o 1.0 (HIGH)
   - AnalogicPin acepta valores en el rango [0.0, 1.0]

3. **Herencia**: Pin es una clase abstracta. Usa DigitalPin o AnalogicPin según tus necesidades.

4. **Lista Circular**: La clase List implementa una lista enlazada circular para optimizar ciertas operaciones.

5. **Namespace**: Todos los elementos están organizados en namespaces específicos para evitar conflictos de nombres.