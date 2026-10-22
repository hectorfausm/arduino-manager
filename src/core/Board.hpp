#ifndef BOARD_HOME
#define BOARD_HOME

#include "../config/PinConf.hpp"
#include "../utils/List.hpp"
#include "../components/DigitalLedComponent.hpp"
#include "../components/LM35Component.hpp"
#include "../components/RGBLedComponent.hpp"
#include "../components/DHT11Component.hpp"

namespace ArduinoManager {
namespace Core {

/**
 * @class Board
 * @brief Representa una placa con pines y componentes conectados.
 */
class Board {
private:
    Config::PinConf* pinDistribution; // Array dinámico de pines
    int pinCount;
    Utils::List components; // Lista de componentes
    Config::PinConf* getInternalPinConf(const Config::PinConf& pin);
    float defaultVcc;

public:
    Board();
    ~Board();

    void setPinDistribution(const Config::PinConf* sourcePins, int count);
    const Config::PinConf* getPinDistribution() const { return pinDistribution; }
    int getPinCount() const { return pinCount; }
    float getDefaultVcc();
    void setDefaultVcc(float);
    Components::DigitalLedComponent* addDigitalLedComponent(Config::PinConf pin);
    Components::LM35Component* addLM35Component(Config::PinConf pin);
    Components::RGBLedComponent* addRGBLedComponent(Config::PinConf rPin, Config::PinConf gPin, Config::PinConf bPin, bool cathode);
    Components::DHT11Component* addDHT11Component(Config::PinConf pin);
    Utils::List* getComponents();

    // Podrías añadir aquí más métodos como addComponent(), getComponentList(), etc.
};

} // namespace Core
} // namespace ArduinoManager

#endif // BOARD_HOME
