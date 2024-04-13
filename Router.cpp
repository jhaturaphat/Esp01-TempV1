#ifdef ESP32

#elif defined(ESP8266)
#include "coredecls.h"
#endif

#include "Router.h"
#include "WlanManager.h"


// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

void Router::begin(){
  if(!LittleFS.begin()){
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }
  server.begin();  
  delay(1);
  this->start();
}

void Router::notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
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
  server.on("/autocomplete.js", HTTP_GET, [this](AsyncWebServerRequest *request){
    request->send(LittleFS, "/autocomplete.js", "text/javascript");
  });
  server.on("/wifi.json", HTTP_GET, [this](AsyncWebServerRequest *request){
    request->send(LittleFS, "/config/wifi.json", "application/json");
  });
  
  server.on("/ntwcfg", HTTP_GET, [](AsyncWebServerRequest *request)   {
//    เข้าถึง Function แบบ สร้าง Object มาก่อน
        WlanManager WlanCfg;  
        WlanCfg.handleWlanConfig(request);
    });

  server.on("/querytemp",HTTP_GET,[this](AsyncWebServerRequest *request){
    request->send(200, "application/json", "{\"temperature1\":"+String(temperature1)+", \"temperature2\":"+String(temperature2)+"}");
  });
  

  server.on("/wifi.json", HTTP_GET, [this](AsyncWebServerRequest *request){
    request->send(LittleFS, "/config/wifi.json", "application/json");
  });  
  server.on("/config.json", HTTP_GET, [this](AsyncWebServerRequest *request){
    request->send(LittleFS, "/config/config.json", "application/json");
  });
  server.on("/scannetwork.json", HTTP_GET,[this](AsyncWebServerRequest *request){       
   request->send(200, "application/json", this->listSsid);
  });
  
  server.on("/ntpCfg", HTTP_GET, [this](AsyncWebServerRequest *request){
    this->handleNtp(request);
  });
  server.on("/alarmCfg",HTTP_GET, [](AsyncWebServerRequest *request){
    this->handleAlarmCfg(request);
  });
  
  server.on("/setRange", HTTP_GET, [this](AsyncWebServerRequest *request){
    this->handleRange(request);
  });
  
  
  server.on("/restart", HTTP_GET,[](AsyncWebServerRequest *request){
    ESP.restart();
  });
  
 
}

void Router::handleAlarmCfg(AsyncWebServerRequest *request){
  if(!request->hasParam("ssr1low")) return request->send(400,"application/json", "{\"error\":\"No parameters ssr1low"\}");
  if(!request->hasParam("ssr2high")) return request->send(400,"application/json", "{\"error\":\"No parameters ssr1high"\}");
  if(!request->hasParam("ssr2low")) return request->send(400,"application/json", "{\"error\":\"No parameters ssr2low"\}");
  if(!request->hasParam("ssr2high")) return request->send(400,"application/json", "{\"error\":\"No parameters ssr2high"\}");
  
}
//
void Router::setScanNetwork(String ssid){
  listSsid = ssid;
}
//######### Set data to share value
void Router::setTemperature(float temp1, float temp2){
  temperature1 = temp1; 
  temperature2 = temp2;
}

// ############### NTP Server config ############################
bool Router::handleNtp(AsyncWebServerRequest *request){
  if(!request->hasParam("ntpserver")){
    request->send(400, "application/json","{\"status\":\"ntpserver not found\"}");
    return false;
  }
  
 File file = LittleFS.open("/config/config.json", "r");
  if(!file){
    Serial.println("Failed to data open config.json at function NTP for reading"); 
    return false;
  }
 
//  Parse JSON
size_t size = file.size();
String ntpsrv = request->getParam("ntpserver")->value();
Serial.println(file.size());

if(file.size() <= 0){ //หากไฟล์ว่างเปล่าให้เพิ่มค่าไปใหม่
  file.close();
  StaticJsonDocument<64> doc;
  doc["ntp"] = ntpsrv;
  File writeFile = LittleFS.open("/config/config.json", "w");
  if (serializeJson(doc, writeFile) == 0) {
    Serial.println("Failed to write to data.json");
    writeFile.close();    
    return false;
  }
  writeFile.close();
  request->send(200, "application/json","{\"status\":\"new config ntp\"}");
  return true;
}

std::unique_ptr<char[]> buf(new char[size]);
file.readBytes(buf.get(), size);
file.close();

// Adjust the size if necessary
DynamicJsonDocument doc(1024);
DeserializationError  error = deserializeJson(doc, buf.get());
if(error){  
  request->send(400, "application/json","{\"status\":\"Failed to parse config.json\"}");
  return false;
}
doc["ntp"] = ntpsrv;
// Write back to data.json
  File outFile = LittleFS.open("/config/config.json", "w");
  if (!outFile) {    
    request->send(400, "application/json","{\"status\":\"Failed to open config.json for writing\"}");
    return false;
  }
  if (serializeJson(doc, outFile) == 0) {
    Serial.println("Failed to write to data.json");
    outFile.close();
    request->send(400, "application/json","{\"status\":\"Failed to write to config.json\"}");
    return false;
  }
  outFile.close();
  request->send(200, "application/json","{\"status\":\"update ntp success\"}");
  
  return true;
  
}
//#################################################

void Router::handleRange(AsyncWebServerRequest *request){
  if(!request->hasParam("ntpserver")){
    request->send(400, "application/json","{\"status\":\"ntpserver not found\"}");
    return false;
  }
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
