# Verificación de publicación por componente físico

Fecha: 2026-10-23. Implementación del handoff `arduino-manager-componentes.md`.

## Archivos modificados o añadidos

- `src/components/DHT11Component.hpp` y `.cpp`: lectura conjunta y resultado del
  último intento separado de los bytes de la última trama correcta.
- `src/network/esp8266/ESP8266ComponentService.hpp` y `.cpp`: registro compuesto,
  adaptador DHT11, programación y pendientes por componente físico, JSON `values`,
  rutas de escritura por canal y conservación de cadencia al reconectar.
- `src/network/esp8266/ESP8266WifiResource.hpp`: exposición de ambas APIs nuevas.
- `src/examples/d1mini-wifi-resource.ino`: DHT11 físico `climate`, LED escalar y
  alternativa comentada LM35 con un nombre de valor diferente.
- `README.md`, `src/extras/docs/API.md` y `AGENTS.structure.md`: contrato,
  migración, firmas, procedimientos de prueba y mapa.
- `tests/compile_esp8266.py`: amplía la compilación a componentes y todos los
  sketches existentes.
- `tests/test_components.py`, `tests/native/*` y
  `tests/fixtures/dht11-telemetry.json`: pruebas del C++ de producción con
  dependencias controladas y copia del contrato aportado por domo.
- `tests/VERIFICATION.md`: este informe.

No se modifica el proyecto Java. `ESP8266WifiResourceClient.cpp` y el coordinador
`ESP8266WifiResource.cpp` conservan su implementación; el primero se ejecuta en
la prueba nativa y ambos se compilan con Xtensa.

## Resultados

Prueba nativa con `g++ -std=c++11`: **PASS**.

- El JSON del primer DHT11 coincide, una vez parseado, con el contrato de domo:
  `climate` contiene exactamente `temperature=24` y `humidity=60`, con política
  periódica e intervalo común de 10 segundos.
- Dos DHT11 y un LED conservan identidades independientes. Los nombres duplicados,
  políticas desconocidas, intervalo ON_CHANGE distinto de cero, PERIODIC fuera
  de rango y DHT11 periódico de un segundo se rechazan.
- Ausencia de pulsos inicial, checksum incorrecto tras una muestra correcta,
  timeout posterior y recuperación con una trama válida. Ambos getters fallan
  después de cada fallo, sin recuperar datos antiguos de la caché.
- El driver no adquiere a los 1999 ms y permite el siguiente intento a los
  2000 ms. Varias consultas del mismo intento no generan nuevos pulsos.
- HTTP 400 y 500 conservan pendiente el conjunto completo. Los reintentos,
  consultas `/status`, cambios del LED, cambios de ambos DHT11 y reconexiones
  nunca generan un objeto de DHT11 con un solo valor.
- Consultas de estado intercaladas no posponen la publicación periódica.
  Una reconexión inmediata no provoca una nueva adquisición del DHT11.
- Una adquisición con tamaño de vector incorrecto publica todos los canales
  como no saludables.
- POST a los cuatro canales de ambos DHT11 devuelve 405, sin adquirir ni escribir.
  El LED acepta 0/1; rechaza vacío, texto sobrante, NAN, infinito y fuera de rango.
- El cliente real conserva URL y escape del token, HTTP Basic y PATCH `/localUrl`.

Compilación a objetos con **core ESP8266 3.1.2, Xtensa, variante d1_mini: PASS**.
Incluye los cinco `.cpp` del módulo WiFi, los cuatro componentes y los tres
sketches, incluidos los dos ejemplos escalares anteriores. Se observan avisos
preexistentes de literales asignados a `char*` en `PinConf.hpp`; la HAL nativa
también produce avisos de comparación con signo por su implementación de String.

## Reproducción y límites

La sincronización requerida a `/home/hector/Arduino/libraries/ArduinoManager`
se intentó y falló con `Read-only file system`. Esa instalación sigue sin
actualizarse. Se verificó una copia sincronizada alternativa en `/tmp`:

```bash
mkdir -p /tmp/arduino-manager-verification
cp -r src /tmp/arduino-manager-verification/
cp README.md library.properties /tmp/arduino-manager-verification/
ARDUINO_MANAGER_LIBRARY=/tmp/arduino-manager-verification python3 tests/test_components.py
ARDUINO_MANAGER_LIBRARY=/tmp/arduino-manager-verification python3 tests/compile_esp8266.py
```

Ejecutar desde la raíz de ArduinoManager. Ambos scripts usan por defecto la
instalación habitual; hay que sincronizarla cuando se disponga de escritura,
o proporcionar explícitamente una copia actualizada con la variable indicada.

La prueba nativa ejecuta el decodificador de pulsos, servicio y cliente C++ reales.
La HAL, las peticiones al servidor local y las respuestas HTTP son simuladas:
no se abre un socket ni se consulta domo real. La reconexión se prueba mediante
`stop()/begin()` del servicio, no mediante una radio WiFi física. El cliente real
se prueba contra respuestas controladas; no se ejecuta el simulador Java.

La compilación ESP8266 produce objetos; no hay enlace completo de firmware,
carga a placa ni verificación eléctrica o de temporización con DHT11 físico.
No se encontró `arduino-cli` ni `arduino-builder` en PATH. No se considera validada
la integración física ni la entrega al servidor real.
