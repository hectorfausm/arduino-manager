#ifndef ARDUINO_MANAGER_HPP
#define ARDUINO_MANAGER_HPP

// Incluir todas las clases principales

#include "core/AnalogicPin.hpp"
#include "core/Board.hpp"
#include "core/BoardBuilder.hpp"
#include "core/Component.hpp"
#include "core/DigitalPin.hpp"
#include "core/Pin.hpp"

// Incluir componentes
#include "components/DigitalLedComponent.hpp"
#include "components/DHT11Component.hpp"

// Incluir configuraciones
#include "config/PinConf.hpp"
#include "config/PinFunction.hpp"

// Incluir targetas
#include "boards/ArduinoUnoPins.hpp"
#include "boards/D1MiniPins.hpp"

// Incluir utilidades
#include "utils/List.hpp"
#include "utils/BoardUtils.hpp"

namespace AM = ArduinoManager;
using namespace ArduinoManager::Core;
using namespace ArduinoManager::Components;
using namespace ArduinoManager::Boards;
using namespace ArduinoManager::Config;
using namespace ArduinoManager::Utils;

#endif // ARDUINO_MANAGER_HPP
