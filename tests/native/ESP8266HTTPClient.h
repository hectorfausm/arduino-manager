#pragma once
#include "Arduino.h"
#include "ESP8266WiFi.h"
#include <vector>
constexpr int HTTP_CODE_OK=200;
struct Request { String url, method, body, user, password, contentType; };
class HTTPClient {
  Request request;
public:
  static int result;
  static std::vector<Request> requests;
  void setTimeout(uint16_t) {}
  bool begin(WiFiClient &,const String &url) { request.url=url; return true; }
  void addHeader(const String &, const String &s) { request.contentType=s; }
  void setAuthorization(const char *u,const char *p) { request.user=u; request.password=p; }
  int POST(const String &s) { return sendRequest("POST",s); }
  int sendRequest(const String &method,const String &s) {
    request.method=method; request.body=s; requests.push_back(request); return result;
  }
  void end() {}
};
