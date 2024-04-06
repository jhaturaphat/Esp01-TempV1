#include "HardwareSerial.h"
#include "WiFiManager.h"

void WiFiManager::startAP(const char* ssid, const char* password) {
  WiFi.softAP(ssid, password);
  Serial.println("Access Point started");
}

void WiFiManager::handleWlanConfig(AsyncWebServerRequest *request){
  if(request->hasParam("SSID") && request->hasParam("PASS")){
    String IP = request->getParam("IP")->value();
    String SN = request->getParam("SN")->value();
    String GW = request->getParam("GW")->value();
    String SSID = request->getParam("SSID")->value();
    String PASS = request->getParam("PASS")->value();
    String FIXIP = request->getParam("FIXIP")->value();

    File wfconfig = LittleFS.open("/config/wifi.json","w");
    if(!wfconfig){
      request->send(500,"text/plain", "Failed to open config file for writing");
      return;
    }

    wfconfig.println("{");
    wfconfig.println("  \"ssid\": \"" + SSID + "\",");
    wfconfig.println("  \"password\": \"" + PASS + "\",");
    wfconfig.println("  \"ip\": \"" + IP + "\"");
    wfconfig.println("  \"subnet\": \"" + SN + "\"");
    wfconfig.println("  \"gateway\": \"" + GW + "\"");
    wfconfig.println("  \"fixip\": \"" + FIXIP + "\"");
    wfconfig.println("}");
    wfconfig.close();

    request->send(200,"text/plain", "OK");
  }else{
    request->send(200,"text/plain", "ไม่มีพารามิเตอร์ ssid และ password ส่งมาด้วย");
  }
}

bool WiFiManager::WlanSetup(){
  File wfconfig = LittleFS.open("/config/wifi.json","r");
  if(!wfconfig){
      Serial.println("Failed to open config file");
      return false;
    }
    // อ่านข้อมูลจากไฟล์
  size_t size = wfconfig.size();
  std::unique_ptr<char[]> buf(new char[size]);
  wfconfig.readBytes(buf.get(), size);
  wfconfig.close();

  // แปลง JSON เป็นโครงสร้างข้อมูล
  StaticJsonDocument<256> doc;
  DeserializationError error = deserializeJson(doc, buf.get());
  if (error) {
    Serial.println("Failed to parse config file");
    return false;
  }

  // ดึงค่าจาก JSON
  const char* ssid = doc["ssid"];
  const char* password = doc["password"];
  const char* ip = doc["ip"];
  const char* subnet = doc["subnet"];
  const char* gateway = doc["gateway"];
  const String fixip = doc["fixip"];

  // กำหนดค่า IP address ในลักษณะของ IPAddress object
  IPAddress ipAddr;
  ipAddr.fromString(ip);
  IPAddress subnetMask;
  subnetMask.fromString(subnet);
  IPAddress gatewayAddr;
  gatewayAddr.fromString(gateway);

   // กำหนดค่าให้ ESP8266
   if(fixip == "true") WiFi.config(ipAddr, gatewayAddr, subnetMask); 
  
  // โค้ดการเชื่อมต่อ Wi-Fi
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Failed to connect to WiFi");
    return false;
  }
  Serial.println(WiFi.localIP());


  return true;
}
