#include <Arduino.h>
#include <ArduinoManager.h>

using namespace ArduinoManager::Core;

unsigned long previousMillis = 0;       // Tiempo de la última ejecución
const unsigned long interval = 2000;    // Intervalo de 2 segundos (2000 ms)
Board* myBoard = nullptr;
LM35Component* lm35 = NULL;

// Temperaturas del termómetro
const float temperatureThresholds[4] = {10.0, 20.0, 30.0, 40.0};

void setup() {
    Serial.begin(9600);
    myBoard = BoardBuilder::initArduinoUnoBoard();
    myBoard->addDigitalLedComponent(ArduinoUnoPins::D12);
    myBoard->addDigitalLedComponent(ArduinoUnoPins::D11);
    myBoard->addDigitalLedComponent(ArduinoUnoPins::D10);
    myBoard->addDigitalLedComponent(ArduinoUnoPins::D9);
    lm35 = myBoard->addLM35Component(ArduinoUnoPins::A0);
}

void loop() {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        float temperature = lm35->readTemperature();

        // Encender según temperatura
        for (int i = 0; i < 3; i++) {
            DigitalLedComponent* c = (DigitalLedComponent*)myBoard->getComponents()->get(i);

            // Si la temperatura es mayor que la indicada, enciende el led
            if (temperature >= temperatureThresholds[i]) {    
                if(!c->isOn()){
                    c->turnOn();
                }
            }

            // Sino, lo apaga
            else if(c->isOn()){
                c->turnOff();
            }
        }
    }
}