#include "mServer.h"

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

void mServer::begin(){
  if(!LittleFS.begin()){
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }
  server.begin();  
}

void mServer::index(){
  server.on("/",HTTP_GET, [this](AsyncWebServerRequest *request){
//    Serial.println("Root Start server OK");
      request->send(200, "text/html", processor("/index.html"));
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
