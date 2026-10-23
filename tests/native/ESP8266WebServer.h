#pragma once
#include "Arduino.h"
#include <functional>
#include <map>
constexpr int HTTP_GET=0, HTTP_POST=1;
class ESP8266WebServer {
public:
  static ESP8266WebServer *active;
  std::map<String,std::function<void()>> routes;
  std::function<void()> fallback;
  String path, value, response;
  int verb=0, code=0;
  explicit ESP8266WebServer(uint16_t) { active=this; }
  void on(const String &p,int,std::function<void()> f) { routes[p]=f; }
  void onNotFound(std::function<void()> f) { fallback=f; }
  void begin() {}
  void stop() {}
  void handleClient() {}
  String uri() const { return path; }
  int method() const { return verb; }
  String arg(const String &) const { return value; }
  void send(int c,const String &,const String &s) { code=c; response=s; }
  void request(const String &p,int v=HTTP_GET,const String &val="") {
    path=p; verb=v; value=val;
    if (v==HTTP_GET && routes.count(p)) routes[p](); else fallback();
  }
};
