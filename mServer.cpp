#include "coredecls.h"
#include "mServer.h"

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

void mServer::begin(){
  if(!LittleFS.begin()){
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }
  server.begin();  
  esp_delay(1);
  this->start();
}

void mServer::start(){
  server.on("/",HTTP_GET, [this](AsyncWebServerRequest *request){
      request->send(200, "text/html", processor("/index.html"));
  });

  server.on("/wlancfg", HTTP_GET, [this](AsyncWebServerRequest *request){
    request->send(200, "text/html", processor("/wlan/cfg.html"));
  });

  server.on("/favicon.ico", HTTP_GET, [this](AsyncWebServerRequest *request){
    request->send(LittleFS, "/favicon.ico", "image/x-icon");
  });

  server.on("/style.css", HTTP_GET, [this](AsyncWebServerRequest *request){    
    request->send(LittleFS, "/style.css", "text/css");
  });

  server.on("/script.js", HTTP_GET, [this](AsyncWebServerRequest *request){
    request->send(LittleFS, "/script.js", "text/javascript");
  });

}


String mServer::processor(const String path){
  Serial.println("Function Processor OK");
  File file = LittleFS.open(path, "r");
  if(!file){
    Serial.println("Failed to open file for reading");
    return "";
  }
  String html;
  while(file.available()){
    html = file.readString();
  }
  file.close();
  return html;  
}
