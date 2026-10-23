# Mapa lógico

```text
tests/                  # Pruebas nativas de sensores y compilación Xtensa.
src/
├── boards/              # Distribuciones de pines para Arduino Uno y D1 mini.
├── components/
├── config/
├── core/
├── examples/
├── network/esp8266/     # Provisionamiento y registro WiFi opcionales para ESP8266.
├── extras/docs/
└── utils/
```

| Ruta | Responsabilidad / punto de entrada |
| --- | --- |
| `README.md` | Descripción, instalación y orientación de uso de la librería. |
| `library.properties` | Metadatos de la biblioteca Arduino. |
| `src/ArduinoManager.hpp` | Cabecera agregadora de la API de la librería. |
| `src/core/` | Pines, componentes base, placa y constructor de placas. |
| `src/components/` | Componentes de hardware, incluidos LM35, DHT11 y LED. |
| `src/boards/` | Definiciones de pines para Arduino Uno y LOLIN/WEMOS D1 mini. |
| `src/ArduinoManagerESP8266.hpp` | Entrada explícita del módulo WiFi ESP8266, separada del agregador principal. |
| `src/network/esp8266/` | API y componentes de provisión, conexión y registro de recursos ESP8266. |
| `tests/` | Servicio, cliente y DHT11 reales con HAL/HTTP controlados; compilación ESP8266. |
| `src/config/` | Tipos y funciones de configuración de pines. |
| `src/utils/` | Estructuras y utilidades compartidas. |
| `src/examples/` | Sketches de ejemplo, incluido `d1mini-wifi-resource.ino` para WiFi ESP8266. |
| `src/extras/docs/API.md` | Referencia detallada de la API pública. |
