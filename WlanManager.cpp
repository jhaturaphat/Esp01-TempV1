#include "HardwareSerial.h"
#include "WlanManager.h"


void WlanManager::startAP(const char* ssid, const char* password) {
  MDNS.addService("http","tcp",80);
  WiFi.softAP(ssid, password);
  Serial.println("Access Point started");
  
}

//ดึงข้อมูล ChipID
String WlanManager::chipID(){
  String chipID = "ESP_123456";
  #ifdef ARDUINO_ARCH_ESP32  //ถ้าเป็น ESP32
    uint32_t chipId = 0;
      for(int i=0; i<17; i=i+8) {
        chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
      }
      chipID = String(chipId);
  #elif defined(ARDUINO_ARCH_ESP8266) //ถ้าเป็น ESP8266
    chipID = String(ESP.getChipId());
  #endif
  return chipID;
}
//รับข้อมูลมาเก็บไว้
void WlanManager::handleWlanConfig(AsyncWebServerRequest *request) {
  if(!request->hasParam("ssid")){
    request->send(400, "application/json","{\"status\":\"ssid not found\"}");
    return;
  }
    String ssid = request->getParam("ssid")->value();    
    String password = request->getParam("password")->value();    
    String ip = request->getParam("ipaddress")->value();    
    String sn = request->getParam("subnetmask")->value();    
    String gw = request->getParam("gateway")->value();    
    String dns = request->getParam("dns")->value();    
    String fixip = request->getParam("fixip")->value();
    String wifichk = request->getParam("wifichk")->value();

  if(ssid.isEmpty()){
    request->send(400, "application/json", "{\"result\":\"SSID เป็นค่าว่าง\"}");
    return;
  }
    StaticJsonDocument<300> doc;
    doc["ssid"] = ssid;
    doc["password"] = password;
    doc["ip"] = ip;
    doc["sn"] = sn;
    doc["gw"] = gw;
    doc["dns"] = dns;
    doc["fixip"] = fixip;
    doc["wifichk"] = wifichk;

    File configFile = LittleFS.open("/config/wifi.json", "w");
    if(serializeJson(doc, configFile) == 0) {
      request->send(400, "application/json", "{\"result\":\"Failed to write to config file\"}");
    }
     configFile.close();
    request->send(200, "application/json", "{\"result\":\"Config file updated successfully\"}");
    
}

bool WlanManager::WlanSetup() {
  File wfconfig = LittleFS.open("/config/wifi.json", "r");
  if (!wfconfig) {
    Serial.println("Failed to open config file at WlanManager");
    return false;
  }
  // อ่านข้อมูลจากไฟล์
  size_t size = wfconfig.size();
  std::unique_ptr<char[]> buf(new char[size]);
  wfconfig.readBytes(buf.get(), size);
  wfconfig.close();

  // แปลง JSON เป็นโครงสร้างข้อมูล
  StaticJsonDocument<1024> doc;
  DeserializationError error = deserializeJson(doc, buf.get());
  if (error) {
    Serial.println("Failed to parse config file");
    return false;
  }

  // ดึงค่าจาก JSON
  const char* ssid = doc["ssid"].as<const char*>();
  const char* password = doc["password"].as<const char*>();
  const String wifichk = doc["wifichk"].as<String>();
  const String fixip = doc["fixip"].as<String>();
  const char* ip = doc["ip"].as<const char*>();
  const char* subnet = doc["sn"].as<const char*>();
  const char* gateway = doc["gw"].as<const char*>();
  const char* dns = doc["dns"].as<const char*>();
  

  // กำหนดค่า IP address ในลักษณะของ IPAddress object
  IPAddress ipAddr;
  ipAddr.fromString(ip);
  IPAddress subnetMask;
  subnetMask.fromString(subnet);
  IPAddress gatewayAddr;
  gatewayAddr.fromString(gateway);
  IPAddress dnsAddr;
  dnsAddr.fromString(dns);

  // กำหนดค่าให้ ESP8266 ถ้าต้องการ Fixip
  if (fixip == "true") WiFi.config(ipAddr, dnsAddr, gatewayAddr, subnetMask);
  
  // โค้ดการเชื่อมต่อ Wi-Fi
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Failed to connect to WiFi");
    return false;
  }
  this->wifichk = wifichk;
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.subnetMask());
  Serial.println(WiFi.gatewayIP());
  return true;
}

void WlanManager::updateWlanConfig(const char* fixIP) {
  // เปิดไฟล์ config.json เพื่ออ่านข้อมูลเดิม
  File configFile = LittleFS.open("/config/wifi.json", "r");
  if (!configFile) {
    Serial.println("Failed to open config file");
    return;
  }

  // อ่านข้อมูลจากไฟล์
  size_t size = configFile.size();
  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);
  configFile.close();

  // แปลง JSON เป็นโครงสร้างข้อมูล
  StaticJsonDocument<256> doc;
  DeserializationError error = deserializeJson(doc, buf.get());
  if (error) {
    Serial.println("Failed to parse config file");
    return;
  }

  // อัพเดทค่าทั้งหมด
  doc["fixip"] = fixIP;

  // เปิดไฟล์ config.json อีกครั้งเพื่อเขียนข้อมูลอัพเดทลงไป
  configFile = LittleFS.open("/config/wifi.json", "w");
  if (!configFile) {
    Serial.println("Failed to open config file for writing");
    return;
  }

  // ส่งข้อมูลอัพเดทลงไปในไฟล์
  if (serializeJson(doc, configFile) == 0) {
    Serial.println("Failed to write to config file");
  }

  // ปิดไฟล์
  configFile.close();

  Serial.println("Config file updated successfully");
}

void WlanManager::updateWlanConfig(const char* newSSID, const char* newPassword) {
  // เปิดไฟล์ config.json เพื่ออ่านข้อมูลเดิม
  File configFile = LittleFS.open("/config/wifi.json", "r");
  if (!configFile) {
    Serial.println("Failed to open config file");
    return;
  }

  // อ่านข้อมูลจากไฟล์
  size_t size = configFile.size();
  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);
  configFile.close();

  // แปลง JSON เป็นโครงสร้างข้อมูล
  StaticJsonDocument<256> doc;
  DeserializationError error = deserializeJson(doc, buf.get());
  if (error) {
    Serial.println("Failed to parse config file");
    return;
  }

  // อัพเดทค่าทั้งหมด
  doc["ssid"] = newSSID;
  doc["password"] = newPassword;
  // doc["ip"] = newIP;
  // doc["subnet"] = newSubnet;
  // doc["gateway"] = newGateway;

  // เปิดไฟล์ config.json อีกครั้งเพื่อเขียนข้อมูลอัพเดทลงไป
  configFile = LittleFS.open("/config/wifi.json", "w");
  if (!configFile) {
    Serial.println("Failed to open config file for writing");
    return;
  }

  // ส่งข้อมูลอัพเดทลงไปในไฟล์
  if (serializeJson(doc, configFile) == 0) {
    Serial.println("Failed to write to config file");
  }

  // ปิดไฟล์
  configFile.close();

  Serial.println("Config file updated successfully");
}

bool WifiHealthy(){
  if(WiFi.status() == WL_CONNECTED){
    return true;
  }else{
    return false;
  }
}
