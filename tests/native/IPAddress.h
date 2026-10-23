#pragma once
#include "Arduino.h"
class IPAddress {
public:
  IPAddress(int,int,int,int) {}
  String toString() const { return "192.168.1.5"; }
};
