#include "HardwareSerial.h"
#include "WiFiManager.h"

void WiFiManager::startAP(const char* ssid, const char* password) {
  WiFi.softAP(ssid, password);
  Serial.println("Access Point started");
}

void WiFiManager::handleNetConfig(AsyncWebServerRequest *request){
  if(request->hasParam("IP")){
    String IP = request->getParam("IP")->value();
    request->send(200,"text/plain", IP);
  }else{
    request->send(200,"text/plain", "OK");
  }
}
