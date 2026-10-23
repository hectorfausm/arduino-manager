#pragma once
#include <string>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <type_traits>
using boolean = bool;
class String : public std::string {
public:
  using std::string::string;
  String(const std::string &s): std::string(s) {}
  template<class T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
  String(T n): std::string(std::to_string(n)) {}
  String(double n, int precision = 2) { char b[64]; snprintf(b, sizeof b, "%.*f", precision, n); assign(b); }
  int length() const { return static_cast<int>(size()); }
  bool startsWith(const String &s) const { return compare(0, s.size(), s) == 0; }
  bool endsWith(const String &s) const { return size() >= s.size() && compare(size()-s.size(), s.size(), s) == 0; }
  void remove(size_t i) { erase(i); }
  String substring(size_t a, size_t b) const { return substr(a,b-a); }
};
constexpr int INPUT=0, OUTPUT=1, INPUT_PULLUP=2, LOW=0, HIGH=1, A0=0;
uint32_t millis();
uint32_t micros();
void delay(uint32_t);
void delayMicroseconds(uint32_t);
void pinMode(int,int);
void digitalWrite(int,int);
int digitalRead(int);
inline int analogRead(int) { return 0; }
inline void analogWrite(int,int) {}
inline void randomSeed(int) {}
inline long random(int) { return 1; }
struct SerialStub { template<class T> void println(const T &) {} };
extern SerialStub Serial;
