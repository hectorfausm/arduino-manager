# ArduinoManager

Punto central de instrucciones para modelos de lenguaje y referencias a la documentación del proyecto.

## Uso

- La fuente de desarrollo es este directorio `ArduinoManager/`. Los tests importan la copia instalada en `/home/hector/Arduino/libraries/ArduinoManager`; después de cualquier cambio en esta biblioteca, sincroniza allí `README.md`, `library.properties` y el contenido de `src/` antes de compilar o ejecutar tests. No edites esa copia como fuente principal.
- Antes de ejecutar comandos, consulta el entorno local; si falta, usa su plantilla para identificar datos pendientes y confirma las rutas necesarias.
- Localiza el área de trabajo en el [mapa lógico](AGENTS.structure.md) y carga solo la documentación pertinente.
- El README contiene los procedimientos operativos de la librería; consulta también la referencia API cuando el cambio afecte a su interfaz pública.

## Permisos de agentes

### Permisos permanentes

- Aceptados: ninguno especificado.
- Rechazados: ninguno especificado.

Resto de operaciones: configuración de desarrollador y permisos efectivos del entorno.

## Referencias de configuración

- [Entorno local](AGENTS.env.md)
- [Plantilla de entorno](AGENTS.env.example.md)
- [Mapa lógico](AGENTS.structure.md)

## Mantenimiento

- Mantén una fuente por tema: reglas del agente en AGENTS y procedimientos operativos en el README. Actualiza las referencias al mover documentación o instrucciones.
- Actualiza el mapa ante cambios de áreas, responsabilidades o entradas. Mantén el árbol selectivo y deja las descripciones en la tabla.
- Mantén la plantilla al día con los datos de entorno que el proyecto necesita y no incluyas rutas personales ni credenciales.
- Registra permisos permanentes solo cuando el usuario indique que deben mantenerse.
