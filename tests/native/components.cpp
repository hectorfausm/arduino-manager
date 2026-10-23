#include "network/esp8266/ESP8266ComponentService.hpp"
#include "ESP8266HTTPClient.h"
#include "components/RGBLedComponent.hpp"
#include <cassert>
#include <deque>
#include <iostream>
#include <map>
using namespace ArduinoManager::Network;
using ArduinoManager::Components::DHT11Component;
SerialStub Serial;
ESP8266WebServer *ESP8266WebServer::active=nullptr;
int HTTPClient::result=200;
std::vector<Request> HTTPClient::requests;
uint64_t tick=0;
uint32_t millis() { return tick/1000; }
uint32_t micros() { return tick++; }
void delay(uint32_t ms) { tick+=ms*1000ULL; }
void delayMicroseconds(uint32_t us) { tick+=us; }
using Wave=std::vector<std::pair<int,int>>;
struct PinState { std::deque<Wave> frames; Wave wave; uint64_t start=0; bool armed=false; unsigned attempts=0; };
std::map<int,PinState> pins;
void digitalWrite(int p,int v) { if(v==LOW) { pins[p].armed=true; ++pins[p].attempts; } }
void pinMode(int p,int m) {
  auto &s=pins[p];
  if(m==INPUT_PULLUP && s.armed) {
    s.armed=false; s.start=tick; s.wave.clear();
    if(!s.frames.empty()) { s.wave=s.frames.front(); s.frames.pop_front(); }
  }
}
int digitalRead(int p) {
  auto &s=pins[p]; uint64_t elapsed=tick-s.start;
  for(auto pulse:s.wave) { if(elapsed<static_cast<unsigned>(pulse.second)) return pulse.first; elapsed-=pulse.second; }
  return HIGH;
}
void frame(int pin,int t,int h,bool checksum=true) {
  unsigned char bytes[]={static_cast<unsigned char>(h),0,static_cast<unsigned char>(t),0,static_cast<unsigned char>(h+t+(checksum?0:1))};
  Wave w={{LOW,80},{HIGH,80}};
  for(auto b:bytes) for(int bit=7;bit>=0;--bit) { w.push_back({LOW,50}); w.push_back({HIGH,(b&(1<<bit))?70:28}); }
  w.push_back({LOW,50}); pins[pin].frames.push_back(w);
}
void emit(const char *label,const String &s) { std::cout<<label<<"\t"<<s<<"\n"; }
int main() {
  for (bool cathode : {false, true}) {
    ArduinoManager::Components::RGBLedComponent rgb(10, 12, 13, 14, cathode);
    auto color = rgb.getRGBColor();
    assert(color.r == 0 && color.g == 0 && color.b == 0);
    assert(rgb.turnRGBColor(17, 128, 240));
    color = rgb.getRGBColor();
    assert(color.r == 17 && color.g == 128 && color.b == 240);
    assert(rgb.turnRGBColor(ArduinoManager::Utils::RGB(255, 64, 0)));
    color = rgb.getRGBColor();
    assert(color.r == 255 && color.g == 64 && color.b == 0);
    assert(rgb.turnOff());
    color = rgb.getRGBColor();
    assert(color.r == 0 && color.g == 0 && color.b == 0);
  }
  // Exercise actual pulse decoder, initial failure, stale cache, checksum, timeout, recovery.
  DHT11Component dht(1,4); dht.begin();
  float t,h;
  assert(!dht.readSample(t,h) && std::isnan(t) && std::isnan(h));
  unsigned attempts=pins[4].attempts;
  assert(std::isnan(dht.readHumidity()) && pins[4].attempts==attempts);
  delay(2000); frame(4,22,50);
  const uint32_t validAttemptAt=millis();
  assert(dht.readSample(t,h) && t==22 && h==50);
  attempts=pins[4].attempts;
  assert(dht.readTemperature()==22 && dht.readHumidity()==50 && pins[4].attempts==attempts);
  tick=(static_cast<uint64_t>(validAttemptAt)+1999)*1000;
  assert(dht.readHumidity()==50 && pins[4].attempts==attempts);
  delay(1); frame(4,23,51,false);
  assert(std::isnan(dht.readTemperature()) && std::isnan(dht.readHumidity()));
  delay(2000); // No pulses, after a valid sample.
  assert(!dht.readSample(t,h) && std::isnan(t) && std::isnan(h));
  assert(std::isnan(dht.readHumidity()));
  delay(2000); frame(4,24,60); assert(dht.readSample(t,h) && t==24 && h==60);

  WifiResourceSettings settings; settings.serverBaseUrl="http://domo/";
  settings.serverPort=9024; settings.serverPathPrefix="domo/rest/";
  settings.authUsername="test"; settings.authPassword="test-secret";
  ESP8266ComponentService service(settings);
  DHT11Component second(2,5); second.begin();
  assert(!service.addDHT11Component("climate","temperature","humidity",ComponentUpdatePolicy::PERIODIC,1,dht));
  assert(service.addDHT11Component("climate","temperature","humidity",ComponentUpdatePolicy::PERIODIC,10,dht));
  assert(!service.addComponent("temperature",ComponentUpdatePolicy::PERIODIC,5,[]{return 0.;}));
  assert(!service.addDHT11Component("climate","t2","h2",ComponentUpdatePolicy::PERIODIC,10,second));
  assert(!service.addCompositeComponent("bad",{{"a",{}},{"a",{}}},ComponentUpdatePolicy::ON_CHANGE,0,[](std::vector<double>&){return true;}));
  assert(!service.addComponent("bad",ComponentUpdatePolicy::ON_CHANGE,10,[]{return 0.;}));
  assert(!service.addComponent("bad",static_cast<ComponentUpdatePolicy>(9),0,[]{return 0.;}));
  assert(!service.addComponent("bad",ComponentUpdatePolicy::PERIODIC,86401,[]{return 0.;}));
  assert(service.addDHT11Component("climate2","temperature2","humidity2",ComponentUpdatePolicy::ON_CHANGE,0,second));
  double led=0;
  assert(service.addComponent("led",ComponentUpdatePolicy::ON_CHANGE,0,[&]{return led;},[&](double v){if(v!=0 && v!=1)return false; led=v; return true;}));
  service.begin();
  assert(!service.addComponent("late",ComponentUpdatePolicy::ON_CHANGE,0,[]{return 0.;}));
  delay(2000); frame(4,24,60); frame(5,25,61);
  HTTPClient::result=400; service.update("token/a");
  assert(HTTPClient::requests.size()==1);
  emit("initial",HTTPClient::requests.back().body);
  assert(HTTPClient::requests.back().url=="http://domo:9024/domo/rest/wifiresource/token%2Fa/telemetry");
  assert(HTTPClient::requests.back().user=="test" && HTTPClient::requests.back().password=="test-secret");
  auto &server=*ESP8266WebServer::active;
  attempts=pins[4].attempts;
  server.request("/status"); emit("status",server.response);
  assert(server.code==200 && pins[4].attempts==attempts);
  for(auto attr:{"temperature","humidity","temperature2","humidity2"}) {
    server.request(String("/")+attr,HTTP_POST,"30"); assert(server.code==405);
  }
  assert(pins[4].attempts==attempts);
  for(auto v:{"2","nan","inf","1x",""}) { server.request("/led",HTTP_POST,v); assert(server.code==400 && led==0); }
  server.request("/led",HTTP_POST,"1"); assert(server.code==200 && led==1);
  delay(3000); frame(5,26,62); HTTPClient::result=200; service.update("token/a");
  emit("retry",HTTPClient::requests.back().body);
  auto count=HTTPClient::requests.size(); service.update("token/a"); assert(count==HTTPClient::requests.size());
  // Status requests must not postpone periodic acquisition/publication.
  for(int i=0;i<6;++i) { delay(1000); frame(5,26,62); server.request("/status"); }
  delay(1000); frame(4,27,63); frame(5,26,62); service.update("token/a");
  emit("periodic",HTTPClient::requests.back().body);
  // Failed acquisition invalidates both channels; failed delivery keeps the group.
  pins[5].frames.clear();
  delay(10000); HTTPClient::result=500; service.update("token/a");
  emit("failure",HTTPClient::requests.back().body);
  server.request("/status"); emit("failure_status",server.response);
  delay(3000); service.update("token/a"); emit("failure_retry",HTTPClient::requests.back().body);
  // Reconnect lifecycle re-publishes all complete physical components.
  service.stop(); pins[5].frames.clear(); delay(10000); frame(4,28,64); frame(5,29,65);
  service.begin(); HTTPClient::result=200; service.update("token/a");
  emit("reconnect",HTTPClient::requests.back().body);
  // A quick reconnect must not treat cached data as a fresh acquisition.
  attempts=pins[4].attempts;
  service.stop(); service.begin(); service.update("token/a");
  assert(pins[4].attempts==attempts);
  emit("quick_reconnect",HTTPClient::requests.back().body);
  ESP8266WifiResourceClient client(settings);
  assert(client.patchResourceUrl("token/a",IPAddress(192,168,1,5),80));
  assert(HTTPClient::requests.back().method=="PATCH");
  assert(HTTPClient::requests.back().body=="[{\"op\":\"replace\",\"path\":\"/localUrl\",\"value\":\"192.168.1.5:80\"}]");
  service.stop();
  ESP8266ComponentService initialFailure(settings);
  DHT11Component absent(3,12); absent.begin();
  assert(initialFailure.addDHT11Component("climate","temperature","humidity",ComponentUpdatePolicy::PERIODIC,10,absent));
  initialFailure.begin(); initialFailure.update("token/a");
  emit("initial_failure",HTTPClient::requests.back().body);
  auto initialAttempts=pins[12].attempts;
  auto &failedServer=*ESP8266WebServer::active;
  failedServer.request("/status");
  assert(pins[12].attempts==initialAttempts);
  initialFailure.stop();
  ESP8266ComponentService malformed(settings);
  assert(malformed.addCompositeComponent("climate",{{"temperature",{}},{"humidity",{}}},
      ComponentUpdatePolicy::PERIODIC,10,[](std::vector<double> &v) { v.resize(1); v[0]=24; return true; }));
  malformed.begin(); malformed.update("token/a");
  emit("malformed",HTTPClient::requests.back().body);

}
