#ifdef ESP32

#elif defined(ESP8266)
#include "coredecls.h"
#endif

#include "Router.h"
#include "WlanManager.h"


// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void Router::begin(){
  if(!LittleFS.begin()){
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }
  server.begin();  
  delay(1);
  this->start();
}

void Router::start(){
  server.on("/",HTTP_GET, [this](AsyncWebServerRequest *request){
      request->send(200, "text/html", processor("/index.html"));
  });

// หน้าตั้งค่า
  server.on("/setting.css", HTTP_GET, [this](AsyncWebServerRequest *request){
    request->send(200, "text/css", processor("/setting.css"));
  });
  server.on("/setting.html", HTTP_GET, [this](AsyncWebServerRequest *request){
    request->send(200, "text/html", processor("/setting.html"));
  });
  server.on("/setting.js", HTTP_GET, [this](AsyncWebServerRequest *request){
    request->send(LittleFS, "/setting.js", "text/javascript");
  });
  

// Icon
  server.on("/favicon.ico", HTTP_GET, [this](AsyncWebServerRequest *request){
    request->send(LittleFS, "/favicon.ico", "image/x-icon");
  });
// Image
  server.on("/img/line_notify.png", HTTP_GET, [this](AsyncWebServerRequest *request){
    request->send(LittleFS, "/img/line_notify.png", "image/png");
  });  
  // Css
  server.on("/style.css", HTTP_GET, [this](AsyncWebServerRequest *request){    
    request->send(LittleFS, "/style.css", "text/css");
  });
//javaacript
  server.on("/js/highcharts.js", HTTP_GET, [this](AsyncWebServerRequest *request){
    request->send(LittleFS, "/js/highcharts.js", "text/javascript");
  });
  server.on("/js/sweetalert.js", HTTP_GET, [this](AsyncWebServerRequest *request){
    request->send(LittleFS, "/js/sweetalert.js", "text/javascript");
  });
  server.on("/script.js", HTTP_GET, [this](AsyncWebServerRequest *request){
    request->send(LittleFS, "/js/script.js", "text/javascript");
  });
  server.on("/wifi.json", HTTP_GET, [this](AsyncWebServerRequest *request){
    request->send(LittleFS, "/config/wifi.json", "application/json");
  });
  
  server.on("/ntwcfg", HTTP_GET, [](AsyncWebServerRequest *request)   {
        WlanManager WlanCfg;  
        WlanCfg.handleWlanConfig(request);
    });

  server.on("/temp",HTTP_GET,[](AsyncWebServerRequest *request){
    
  });
  

  server.on("/wifi.json", HTTP_GET, [this](AsyncWebServerRequest *request){
    request->send(LittleFS, "/config/wifi.json", "application/json");
  });  
  server.on("/location",HTTP_POST, [](AsyncWebServerRequest *request){
    
  });
  server.on("/restart", HTTP_GET,[](AsyncWebServerRequest *request){
    ESP.restart();
  });
  
  server.onNotFound(notFound);  
}

void Router::setTemperature(float temp1, float temp2){
  temperature1 = temp1; temperature2 = temp2;
}

String Router::processor(const String path){
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
