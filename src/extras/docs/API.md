# ArduinoManager API Reference

## Tabla de Contenidos

- [Instalación](#instalación)
- [Uso Básico](#uso-básico)
- [Namespaces](#namespaces)
- [ESP8266 Wi-Fi resource](#esp8266-wi-fi-resource)
- [Core Classes](#core-classes)
  - [Pin](#pin)
  - [DigitalPin](#digitalpin)
  - [AnalogicPin](#analogicpin)
  - [DHT11Component](#dht11component)
  - [Component](#componente)
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
    AnalogicPin component(A0, INPUT);
    component.perform();
    float valor = component.getValue();
}
```

## Namespaces

La librería está organizada en los siguientes namespaces:

- `ArduinoManager::Core` - Clases principales
- `ArduinoManager::Components` - Componentes específicos
- `ArduinoManager::Boards` - Configuraciones de placas
- `ArduinoManager::Config` - Configuraciones y enumeraciones
- `ArduinoManager::Utils` - Utilidades auxiliares
- `ArduinoManager::Network` - Provisión y registro WiFi opcionales en ESP8266; requiere `ArduinoManagerESP8266.hpp`

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

### DHT11Component

Componente para leer temperatura y humedad de un DHT11 mediante un pin digital.
Se crea desde la placa con `addDHT11Component()` y debe inicializarse una vez
en `setup()` con `begin()`.

```cpp
Board* board;
DHT11Component* dht;

void setup() {
    board = BoardBuilder::initArduinoUnoBoard();
    dht = board->addDHT11Component(ArduinoUnoPins::D2);
    if (dht != nullptr) dht->begin();
}

void loop() {
    if (dht != nullptr) {
        float temperature = dht->readTemperature();
        float humidity = dht->readHumidity(); // Reutiliza la misma muestra
    }
}
```

Las muestras se limitan a una cada dos segundos. Si todavía no hay una muestra
válida, los métodos devuelven `NAN`; comprueba `isnan()` antes de usar los
valores. `perform()` intenta actualizar la muestra y devuelve si hay datos
válidos disponibles. El protocolo ocupa el microcontrolador durante la lectura.
El componente se incluye en `ArduinoManager.hpp`.

La línea de datos debe tener una resistencia pull-up (algunos módulos ya la
incluyen); `INPUT_PULLUP` se activa como apoyo. Comprueba el voltaje compatible
con el módulo DHT11 que estás usando antes de conectarlo.

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

Para LOLIN/WEMOS D1 mini (ESP8266), usa `initD1MiniBoard()` y las referencias
simbólicas de `D1MiniPins`:

```cpp
Board* board;
DigitalLedComponent* led;

void setup() {
    board = BoardBuilder::initD1MiniBoard();
    led = board->addDigitalLedComponent(D1MiniPins::D5);
}
```

| Etiqueta | GPIO | Capacidades y observaciones |
| --- | ---: | --- |
| D0 | 16 | Digital; sin PWM ni interrupciones |
| D1 | 5 | Digital, PWM, I2C SCL |
| D2 | 4 | Digital, PWM, I2C SDA |
| D3 | 0 | Digital, PWM; pin de arranque |
| D4 | 2 | Digital, PWM; LED integrado activo en bajo y pin de arranque |
| D5 | 14 | Digital, PWM, SPI SCK |
| D6 | 12 | Digital, PWM, SPI MISO |
| D7 | 13 | Digital, PWM, SPI MOSI |
| D8 | 15 | Digital, PWM, SPI SS; pin de arranque |
| RX / TX | 3 / 1 | Digital y PWM; UART comparte pines con el puerto serie |
| A0 | A0 del core | Única entrada analógica; máximo 3,2 V en D1 mini |

La placa trabaja con lógica de 3,3 V. Evita aplicar señales de 5 V a los GPIO.
GPIO0, GPIO2 y GPIO15 participan en la selección del modo de arranque; las
cargas conectadas a D3, D4 y D8 no deben forzar niveles incompatibles durante
el reinicio. D0/GPIO16 tampoco admite interrupciones externas. Estas capacidades
se basan en la documentación y el pinout oficial de [LOLIN D1 mini](https://docs.wemos.cc/en/latest/d1/d1_mini.html)
y el [ESP8266 Arduino Core](https://arduino-esp8266.readthedocs.io/en/latest/reference.html).

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

### Ejemplo 2: Component Analógico

```cpp
#include "ArduinoManager.hpp"

AnalogicPin component(A0, INPUT);

void setup() {
    Serial.begin(9600);
    component.setDescription("Component de luz");
}

void loop() {
    component.perform(); // Leer valor
    float valor = component.getValue();
    
    Serial.print("Valor del componente: ");
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

## ESP8266 Wi-Fi resource

Este módulo es opcional y solo debe incluirse en sketches ESP8266 mediante
`ArduinoManagerESP8266.hpp`. No se incluye desde `ArduinoManager.hpp`, para
mantener el agregador principal independiente de las cabeceras del core ESP8266.
El core de ESP8266 proporciona `ESP8266WiFi`, `ESP8266WebServer`, EEPROM y
`ESP8266HTTPClient`.

El ejemplo completo está en
[`src/examples/d1mini-wifi-resource.ino`](../../examples/d1mini-wifi-resource.ino).
Configura `provisioningSsid`, una contraseña AP de 8 a 63 caracteres (o cadena
vacía para un AP abierto), `serverBaseUrl`, `serverPathPrefix`, `serverPort`, `localResourcePort` y
`httpTimeoutMs`. `serverPathPrefix` es opcional y permite configurar la ruta base
de la API (por ejemplo, `/domo/rest`); vacío conserva la ruta raíz actual.
`authUsername` y `authPassword` son opcionales. No guardes
credenciales reales ni tokens en el sketch.

`serverBaseUrl` debe contener el esquema y el nombre de host del servidor, sin
ruta ni puerto; el cliente añade `serverPort`, `serverPathPrefix` y la ruta del
recurso.

`begin()` inicializa el almacén y elige `PROVISIONING` o `CONNECTING`.
`update()` debe llamarse regularmente para atender el AP, observar la conexión y
reintentar el registro. Los estados posibles son `PROVISIONING`, `CONNECTING`,
`REGISTERING`, `READY` y `ERROR`; los errores de PATCH mantienen al controlador
en `REGISTERING` para reintentar. `resetProvisioning()` devuelve `true` si pudo
borrar las credenciales e iniciar de nuevo el AP; no reinicia la placa.

```cpp
#include <ArduinoManagerESP8266.hpp>

using namespace ArduinoManager::Network;

WifiResourceSettings makeSettings() {
  WifiResourceSettings value;
  value.provisioningSsid = "D1Mini-Setup";
  value.provisioningPassword = ""; // Empty means an open AP; configure a password for deployment.
  value.serverBaseUrl = "http://192.168.1.10"; // Replace with the REST server host.
  value.serverPathPrefix = "/domo/rest"; // Base path for this Java REST server.
  value.serverPort = 9024;
  value.localResourcePort = 80;
  return value;
}

WifiResourceSettings settings = makeSettings();
ESP8266EEPROMCredentialStore store;
ESP8266WifiResource resource(settings, store);

void setup() {
  Serial.begin(115200);
  resource.begin();
}

void loop() {
  resource.update();
  WifiResourceState state = resource.getState();
}
```

Cuando el almacenamiento no contiene credenciales válidas, la placa abre el AP
configurado y atiende `POST /getCredentials`. El cuerpo debe ser formulario
URL-encoded con `essid`, `password` y `token`; los tres campos son obligatorios y
no pueden exceder 32, 64 y 128 bytes respectivamente. Un `200` confirma que el
almacenamiento guardó los datos; solicitudes incompletas reciben `400`, campos
demasiado largos `413` y fallos de almacenamiento `500`.

El AP queda abierto si `provisioningPassword` está vacío, y el endpoint local no
usa autenticación HTTP; configura una contraseña AP para despliegues reales. Las
credenciales y el token se guardan en EEPROM sin cifrado. El módulo no inicia un
servidor para el recurso local: el sketch u otro servicio debe escuchar en
`localResourcePort`; el PATCH solo publica esa dirección.

Tras conectarse a la red, el cliente envía un PATCH al recurso existente:

```http
PATCH http://<serverBaseUrl>:<serverPort><serverPathPrefix>/wifiresource/<token>
Content-Type: application/json-patch+json

[{"op":"replace","path":"/localUrl","value":"<IP>:<localResourcePort>"}]
```

Solo una respuesta HTTP `200` lleva el estado a `READY`. Fallos de Wi-Fi o REST
se reintentan con espera creciente hasta 30 segundos, sin borrar credenciales ni
reiniciar la placa. Si la IP cambia, se vuelve a publicar. `resetProvisioning()`
borra las credenciales, cierra la conexión actual y abre de nuevo el AP.

La llamada HTTP es síncrona y puede ocupar `update()` hasta el valor configurado
en `httpTimeoutMs` (3 segundos en el ejemplo); los reintentos sí se programan sin
esperas activas entre llamadas. El sketch debe llamar a `update()` regularmente.


## Estado de componentes y telemetría ESP8266

El registro se realiza antes de `begin()`. Cada componente físico tiene una identidad,
una política y un intervalo común para todos sus valores. Comprueba siempre el
`bool` devuelto por los métodos de alta; `false` no añade ni modifica componentes.
Los nombres deben coincidir con el alta administrativa existente en domo.
La placa no crea recursos administrativos y conserva la provisión del token,
HTTP Basic configurable y PATCH de `/localUrl` descritos arriba.

### DHT11: lectura y publicación conjunta

```cpp
ArduinoManager::Components::DHT11Component climate(1, 4); // D2/GPIO4
// En setup(), antes de wifiResource.begin():
climate.begin();
bool added = wifiResource.addDHT11Component(
    "climate", "temperature", "humidity",
    ComponentUpdatePolicy::PERIODIC, 10, climate);
```

Firma pública en `ESP8266WifiResource` y `ESP8266ComponentService`:

```cpp
bool addDHT11Component(const String &attr, const String &temperatureAttr,
    const String &humidityAttr, ComponentUpdatePolicy policy,
    uint32_t intervalSeconds, Components::DHT11Component &dht);
```

`dht` debe seguir existiendo mientras el servicio esté en uso. Este adaptador
invoca una sola vez `dht.readSample(float &temperature, float &humidity)` y publica
ambos valores juntos. La lectura conjunta devuelve `false` y ambos resultados
`NAN` si falla la adquisición. Los getters individuales comparten el resultado
del último intento durante los 2000 ms mínimos: después de un timeout o checksum
incorrecto ninguno devuelve la última muestra correcta como lectura válida.
Una trama correcta posterior recupera ambos canales.

`PERIODIC` admite un intervalo entero 2–86400 segundos para DHT11. `ON_CHANGE`
requiere intervalo 0 y adquiere como máximo cada 2000 ms. El resto de componentes
periódicos admite 1–86400 segundos, sujeto a su mínimo de adquisición.

Para dos DHT11 registra, por ejemplo, `climate` con `temperature`/`humidity`
y `climate2` con `temperature2`/`humidity2`. Las identidades exteriores son únicas;
los nombres de valores son únicos en toda la placa. No se agrupa por tipo.

### API compuesta general y compatibilidad escalar

```cpp
using Writer = std::function<bool(double)>;
struct Channel { String attr; Writer write; };
using JointReader = std::function<bool(std::vector<double> &)>;

bool addCompositeComponent(const String &attr, const std::vector<Channel> &channels,
    ComponentUpdatePolicy policy, uint32_t intervalSeconds,
    JointReader read, uint32_t minimumReadIntervalMs = 0);

bool addComponent(const String &attr, ComponentUpdatePolicy policy,
    uint32_t intervalSeconds, Reader read, Writer write = Writer());
```

Los alias `Reader`, `Writer`, `Channel` y `JointReader` están en
`ESP8266ComponentService`. El lector conjunto recibe un vector del tamaño de los
canales, inicializado a `NAN`, y rellena sus posiciones en el mismo orden.
Un retorno `false`, tamaño incorrecto o cualquier valor no finito invalida todos
los valores de esa adquisición. Cada canal puede tener su propio escritor;
un `Writer` vacío significa solo lectura. Se admiten hasta 16 componentes físicos
y 16 canales por componente. Los nombres tienen 1–64 caracteres alfanuméricos,
guion o guion bajo; `status` y `getCredentials` están reservados.

`addComponent` conserva su firma y su lector `double()` para LM35, LED y otros
componentes escalares. Crea un componente con un valor, ambos con el mismo `attr`.
`NAN` indica fallo. Su JSON también pasa a usar el array `values`.

No existe configuración de política o intervalo por canal dentro de un componente
compuesto, por lo que no pueden arrancarse canales contradictorios. Para migrar
las dos altas escalares de un DHT11, elimínalas y usa una sola llamada
`addDHT11Component`, acordando un intervalo común de al menos 2 segundos. No añadas
los canales por separado. Las altas duplicadas y las políticas o intervalos
inválidos se rechazan antes de modificar el registro.

### Contrato HTTP y programación

`GET /status` devuelve todos los componentes físicos y todos sus valores:

```json
{"components":[{"attr":"climate","values":[{"attr":"temperature","value":24,"healthy":true,"updatePolicy":"PERIODIC","expectedIntervalSeconds":10},{"attr":"humidity","value":60,"healthy":true,"updatePolicy":"PERIODIC","expectedIntervalSeconds":10}]}]}
```

Cada valor declara `updatePolicy`; `expectedIntervalSeconds` es un entero para
`PERIODIC` y `null` para `ON_CHANGE`. Si falla el DHT11, se incluyen los dos valores
con `healthy:false` y `value:null`. Domo puede conservar su última lectura correcta.

`update()` y `/status` evalúan las adquisiciones que correspondan según la cadencia
común; `/status` no fuerza nuevas adquisiciones ni retrasa el próximo vencimiento.
La cadencia se cuenta desde el final de la adquisición para respetar también el
mínimo del driver. La respuesta local incluye los valores disponibles de todos
los componentes. Los periódicos publican aunque sus valores no cambien; los demás,
cuando cambia algún valor o la salud del conjunto. Al conectar se publica el
estado inicial de todos los componentes.

La telemetría usa el mismo contrato en
`POST /domo/rest/wifiresource/{token}/telemetry`, con HTTP Basic configurable.
Puede incluir solo los componentes pendientes, pero siempre con todos sus valores.
Un fallo de transporte conserva pendiente el componente físico completo y reintenta
cada 3 segundos. Nuevas adquisiciones sustituyen su estado pendiente completo;
no existe cola histórica. Una consulta `/status` nunca consume lo pendiente.
El bucle debe llamar a `update()` regularmente; HTTP puede bloquear hasta
`httpTimeoutMs`.

Se conserva `POST /ATRIBUTO?value=NUMERO` para cada canal con escritor. Se rechazan
valores no finitos, texto sobrante o valores rechazados por el escritor con 400.
Un canal sin escritor devuelve 405: temperatura, humedad y LM35 son de solo lectura.
Estas rutas locales mantienen el contrato sin autenticación del recurso y deben
permanecer en la red doméstica.

Domo asigna la fecha de recepción; la placa no necesita un reloj UTC.
Una lectura periódica caduca tras tres intervalos. La comprobación del portal
consulta `/status` y distingue fallo de componente de ausencia de respuesta de la placa.
No se implementa reinicio remoto.

### Verificación

Sincroniza primero `README.md`, `library.properties` y `src/` con la biblioteca
instalada como indica AGENTS.md. Después, desde la raíz de ArduinoManager:

```bash
python3 tests/test_components.py
python3 tests/compile_esp8266.py
```

El primer comando compila y ejecuta los `.cpp` reales de driver, servicio y cliente
con `g++ -std=c++11`, reloj y pulsos digitales controlados, servidor HTTP local
simulado y transporte HTTP simulado. Python comprueba el JSON producido por ese
C++; no reimplementa el servicio. Incluye checksum, timeout, caché, recuperación,
dos DHT11, actuador, validación, reintentos, `/status`, lectura sola y reconexión
mediante el ciclo `stop()/begin()` del servicio.

El segundo compila a objetos los fuentes WiFi, componentes y ejemplos con el
core ESP8266 3.1.2 y Xtensa para D1 mini. No enlaza ni carga firmware.
Ambos admiten `ARDUINO_MANAGER_LIBRARY` para seleccionar explícitamente otra
instalación sincronizada, por ejemplo una copia temporal cuando la instalación
habitual es de solo lectura. Esa alternativa no actualiza la biblioteca habitual.
Estas pruebas no sustituyen la validación de temporización eléctrica con hardware
ni una entrega HTTP al servidor domo real.
