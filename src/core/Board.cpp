#include "Board.hpp"
#include "../config/PinConf.hpp"
#include "../components/DigitalLedComponent.hpp"
#include "../components/LM35Component.hpp"
#include "../components/RGBLedComponent.hpp"
#include "../utils/BoardUtils.hpp"

namespace ArduinoManager
{
  namespace Core
  {

    // Constructor por defecto
    Board::Board() : pinDistribution(nullptr), pinCount(0) {
      Serial.println("[INIT] Board()");
    }

    //DEstructor por defecto
    Board::~Board()
    {
      delete[] pinDistribution;
      components.clear();
    }

    void Board::setPinDistribution(const Config::PinConf *sourcePins, int count)
    {
      if (pinDistribution)
      {
        delete[] pinDistribution;
      }

      pinCount = count;
      pinDistribution = new Config::PinConf[count];

      for (int i = 0; i < count; ++i)
      {
        pinDistribution[i] = sourcePins[i]; // Copia por valor
      }
    }

    Components::DigitalLedComponent* Board::addDigitalLedComponent(Config::PinConf pin)
    {
      Config::PinConf* internal = getInternalPinConf(pin);
      if (internal != NULL) {
        long code = Utils::BoardUtils::generateUniqueCode();
        Components::DigitalLedComponent *that = new Components::DigitalLedComponent(code, internal->number);
        if(components.add(that)){
          internal->used = true;
          return that;
        }
      }
      return NULL;
    }

    Components::RGBLedComponent* Board::addRGBLedComponent(Config::PinConf rPin, Config::PinConf gPin, Config::PinConf bPin, bool cathode)
    {
      Config::PinConf* internalRpin = getInternalPinConf(rPin);
      Config::PinConf* internalGPin = getInternalPinConf(gPin);
      Config::PinConf* internalBPin = getInternalPinConf(bPin);
      if (internalRpin != NULL && internalGPin != NULL && internalBPin != NULL) {
        long code = Utils::BoardUtils::generateUniqueCode();
        Components::RGBLedComponent *that = new Components::RGBLedComponent(code, internalRpin->number, internalGPin->number, internalBPin->number, cathode);
        if(components.add(that)){
          internalRpin->used = true;
          internalGPin->used = true;
          internalBPin->used = true;
          return that;
        }
      }
      return NULL;
    }

    Components::LM35Component* Board::addLM35Component(Config::PinConf pin){
      Config::PinConf* internal = getInternalPinConf(pin);
      if (internal != NULL) {
        long code = Utils::BoardUtils::generateUniqueCode();
        Components::LM35Component *that = new Components::LM35Component(code, this->getDefaultVcc(), internal->number);
        if(components.add(that)){
          internal->used = true;
          return that;
        }
      }
      return NULL;
    }

    Config::PinConf* Board::getInternalPinConf(const Config::PinConf& pin) {
      for (int i = 0; i < pinCount; ++i) {
          Config::PinConf thisConf = pinDistribution[i];
          if (!thisConf.used && thisConf.number == pin.number) {
              return &pinDistribution[i];
          }
      }
      return NULL; // No encontrado
    }

    float Board::getDefaultVcc(){
      return defaultVcc;
    }

    void Board::setDefaultVcc(float vcc){
      defaultVcc = vcc;
    }

    Utils::List* Board::getComponents(){
      return &this->components;
    }

  } // namespace Core
} // namespace ArduinoManager