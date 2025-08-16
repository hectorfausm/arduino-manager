#include <Arduino.h>
#include <ArduinoManager.h>

using namespace ArduinoManager::Core;

/**
 * @brief Get the Rgb From Temperature object
 * 
 * @param temperature Temperatura de la que obtener el RGB
 * @return RGB Colores a partir de la temperatura
 */
RGB getRgbFromTemperature(float temperature);

unsigned long previousMillis = 0;       // Tiempo de la última ejecución
const unsigned long interval = 2000;    // Intervalo de 2 segundos (2000 ms)
Board* myBoard = nullptr;               // Tarjeta Arduino
LM35Component* lm35 = NULL;             // Componente que lee la tempreatura
RGBLedComponent* rgb = NULL;            // LED RGB

void setup() {
    Serial.begin(9600);
    myBoard = BoardBuilder::initArduinoUnoBoard();
    lm35 = myBoard->addLM35Component(ArduinoUnoPins::A0);
    rgb = myBoard->addRGBLedComponent(ArduinoUnoPins::D3, ArduinoUnoPins::D5, ArduinoUnoPins::D6, false);
}

void loop() {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        float temperature = lm35->readTemperature();
        Serial.println("Temperatura: "+String(temperature)+"C");
        RGB rgbColor = getRgbFromTemperature(temperature);
        Serial.println("Color RGB("+String(rgbColor.r)+","+String(rgbColor.g)+","+String(rgbColor.b)+")");
        rgb->turnRGBColor(rgbColor);
    }
}

RGB getRgbFromTemperature(float temperature){

    // Clamp temperature to [0, 50]
    if (temperature < 0) temperature = 0;
    if (temperature > 50) temperature = 50;

    if (temperature <= 16.66f) {
        // Azul (0,0,255) → Verde (0,255,0)
        float ratio = temperature / 16.66f;
        int r = 0;
        int g = static_cast<int>(255 * ratio);
        int b = static_cast<int>(255 * (1.0f - ratio));
        return RGB(r, g, b);
    } else if (temperature <= 33.33f) {
        // Verde (0,255,0) → Amarillo (255,255,0)
        float ratio = (temperature - 16.66f) / 16.66f;
        int r = static_cast<int>(255 * ratio);
        int g = 255;
        int b = 0;
        return RGB(r, g, b);
    } else {
        // Amarillo (255,255,0) → Rojo (255,0,0)
        float ratio = (temperature - 33.33f) / 16.66f;
        int r = 255;
        int g = static_cast<int>(255 * (1.0f - ratio));
        int b = 0;
        return RGB(r, g, b);
    }
}