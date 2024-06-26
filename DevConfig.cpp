#include "DevConfig.h"

//ตั้งค่า ช่วงอุณหภูมิ
void DevConfig::handleAlarmCfg(AsyncWebServerRequest *request) {
  if (!request->hasParam("ssr1low")) return request->send(400, "application/json", "{\"error\":\"No parameters ssr1low\"}");
  if (!request->hasParam("ssr1high")) return request->send(400, "application/json", "{\"error\":\"No parameters ssr1high\"}");
  if (!request->hasParam("ssr2low")) return request->send(400, "application/json", "{\"error\":\"No parameters ssr2low\"}");
  if (!request->hasParam("ssr2high")) return request->send(400, "application/json", "{\"error\":\"No parameters ssr2high\"}");

  String ssr1low = request->getParam("ssr1low")->value();
  String ssr1high = request->getParam("ssr1high")->value();
  String ssr2low = request->getParam("ssr2low")->value();
  String ssr2high = request->getParam("ssr2high")->value();



  File file = LittleFS.open("/config/config.json", "r");
  if (!file) {
    return request->send(400, "application/json", "{\"error\":\"Failed to open config.json for update\"}");
  }
  size_t size = file.size();
  std::unique_ptr<char[]> buf(new char[size]);
  file.readBytes(buf.get(), size);
  file.close();

  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, buf.get());
  if (error) return request->send(400, "application/json", "{\"error\":\"Failed to parse config.json for update\"}");

  //  หากไม่พบ key alarm temperature ให้สร้างใหม่
  if (!doc.containsKey("alarm")) {
    // Add new data to the JSON object
    JsonObject alarm = doc.createNestedObject("alarm");
    JsonObject sensor1 = alarm.createNestedObject("sensor_1");
    sensor1["low"] = ssr1low.toInt();
    sensor1["high"] = ssr1high.toInt();
    JsonObject sensor2 = alarm.createNestedObject("sensor_2");
    sensor2["low"] = ssr2low.toInt();
    sensor2["high"] = ssr2high.toInt();
    // Open file for writing
    File newFile = LittleFS.open("/config/config.json", "w");
    if (!newFile) {
      return request->send(400, "application/json", "{\"error\":\"Failed to create new config file\"}");
    }
    // Serialize JSON to file
    if (serializeJson(doc, newFile) == 0) return request->send(400, "application/json", "{\"error\":\"Failed to write to new config file\"}");
    // Close the file
    newFile.close();
    return request->send(200, "application/json", "{\"ok\":\"New config file created\"}");
  } else {
    //หากพบ key alarm temperature ให้อัพเดทแทน
    File file = LittleFS.open("/config/config.json", "r+");
    if (!file) return request->send(400, "application/json", "{\"error\":\"Failed to open config.json for update\"}");
    // Parse the JSON object
    size_t size = file.size();
    std::unique_ptr<char[]> buf(new char[size]);
    file.readBytes(buf.get(), size);
    file.close();

    DynamicJsonDocument doc(1024);  // Adjust the size if necessary
    DeserializationError error = deserializeJson(doc, buf.get());
    if (error) return request->send(400, "application/json", "{\"error\":\"Failed to parse config file\"}");
    // Update the values in the JSON object
    JsonObject sensor1 = doc["alarm"]["sensor_1"];
    sensor1["low"] = ssr1low.toInt();
    sensor1["high"] = ssr1high.toInt();

    JsonObject sensor2 = doc["alarm"]["sensor_2"];
    sensor2["low"] = ssr2low.toInt();
    sensor2["high"] = ssr2high.toInt();

    file = LittleFS.open("/config/config.json", "w");
    if (!file) return request->send(400, "application/json", "{\"error\":\"Failed to create new config file\"}");
    // Serialize JSON to file
    if (serializeJson(doc, file) == 0) return request->send(400, "application/json", "{\"error\":\"Failed to write to config file\"}");
    // Close the file
    file.close();
    return request->send(200, "application/json", "{\"ok\":\"Config.json updated successfully\"}");
  }
}


// ############### NTP Server config ############################
bool DevConfig::handleNtp(AsyncWebServerRequest *request) {
  if (!request->hasParam("ntpserver")) {
    request->send(400, "application/json", "{\"status\":\"ntpserver not found\"}");
    return false;
  }

  File file = LittleFS.open("/config/config.json", "r");
  if (!file) {
    Serial.println("Failed to data open config.json at function NTP for reading");
    return false;
  }

  //  Parse JSON
  size_t size = file.size();
  String ntpsrv = request->getParam("ntpserver")->value();
  Serial.println(file.size());

  if (file.size() <= 0) {  //หากไฟล์ว่างเปล่าให้เพิ่มค่าไปใหม่
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
    request->send(200, "application/json", "{\"status\":\"new config ntp\"}");
    return true;
  }

  std::unique_ptr<char[]> buf(new char[size]);
  file.readBytes(buf.get(), size);
  file.close();

  // Adjust the size if necessary
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, buf.get());
  if (error) {
    request->send(400, "application/json", "{\"status\":\"Failed to parse config.json\"}");
    return false;
  }
  doc["ntp"] = ntpsrv;
  // Write back to data.json
  File outFile = LittleFS.open("/config/config.json", "w");
  if (!outFile) {
    request->send(400, "application/json", "{\"status\":\"Failed to open config.json for writing\"}");
    return false;
  }
  if (serializeJson(doc, outFile) == 0) {
    Serial.println("Failed to write to data.json");
    outFile.close();
    request->send(400, "application/json", "{\"status\":\"Failed to write to config.json\"}");
    return false;
  }
  outFile.close();
  request->send(200, "application/json", "{\"status\":\"update ntp success\"}");

  return true;
}

// ตั้งเวลาแจ้งเตือนสถานะ
void DevConfig::handleAlarmRange(AsyncWebServerRequest *request) {
  if (!request->hasParam("alH1")) return request->send(400, "application/json", "{\"error\":\"No parameters ช่วงเวลาที่1\"}");
  if (!request->hasParam("alH2")) return request->send(400, "application/json", "{\"error\":\"No parameters ช่วงเวลาที่2\"}");
  if (!request->hasParam("alH3")) return request->send(400, "application/json", "{\"error\":\"No parameters ช่วงเวลาที่3\"}");

  String alH1 = request->getParam("alH1")->value();
  String alH2 = request->getParam("alH2")->value();
  String alH3 = request->getParam("alH3")->value();

  File file = LittleFS.open("/config/config.json", "r");
  if (!file) {
    return request->send(400, "application/json", "{\"error\":\"Failed to open config.json for update\"}");
  }
  size_t size = file.size();
  std::unique_ptr<char[]> buf(new char[size]);
  file.readBytes(buf.get(), size);
  file.close();

  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, buf.get());
  if (error) return request->send(400, "application/json", "{\"error\":\"Failed to parse config.json for update\"}");

  //  หากไม่พบ key timeRange ให้สร้างใหม่
  if (!doc.containsKey("alarm")) {
    // Add new data to the JSON object
    JsonObject timeRange = doc.createNestedObject("timeRang");
    timeRange["range1"] = alH1.toInt();
    timeRange["range2"] = alH2.toInt();
    timeRange["range3"] = alH3.toInt();

    // Open file for writing
    File newFile = LittleFS.open("/config/config.json", "w");
    if (!newFile) {
      return request->send(400, "application/json", "{\"error\":\"Failed to create new config file\"}");
    }
    // Serialize JSON to file
    if (serializeJson(doc, newFile) == 0) return request->send(400, "application/json", "{\"error\":\"Failed to write to new config file\"}");
    // Close the file
    newFile.close();
    return request->send(200, "application/json", "{\"ok\":\"New config file created\"}");
  } else {
    //หากพบ key timeRang ให้อัพเดทแทน
    File file = LittleFS.open("/config/config.json", "r+");
    if (!file) return request->send(400, "application/json", "{\"error\":\"Failed to open config.json for update\"}");
    // Parse the JSON object
    size_t size = file.size();
    std::unique_ptr<char[]> buf(new char[size]);
    file.readBytes(buf.get(), size);
    file.close();

    DynamicJsonDocument doc(1024);  // Adjust the size if necessary
    DeserializationError error = deserializeJson(doc, buf.get());
    if (error) return request->send(400, "application/json", "{\"error\":\"Failed to parse config file\"}");
    // Update the values in the JSON object
    JsonObject timeRange = doc["timeRange"];
    timeRange["range1"] = alH1.toInt();
    timeRange["range2"] = alH2.toInt();
    timeRange["range3"] = alH3.toInt();


    file = LittleFS.open("/config/config.json", "w");
    if (!file) return request->send(400, "application/json", "{\"error\":\"Failed to create new config file\"}");
    // Serialize JSON to file
    if (serializeJson(doc, file) == 0) return request->send(400, "application/json", "{\"error\":\"Failed to write to config file\"}");
    // Close the file
    file.close();
    return request->send(200, "application/json", "{\"ok\":\"Config.json updated successfully\"}");
  }
}

// ตั้งค่า line TOKEN
void DevConfig::handleLineConf(AsyncWebServerRequest *request) {
  if (!request->hasParam("TOKEN")) return request->send(400, "application/json", "{\"error\":\"No parameters Line TOKEN\"}");

  String token_line = request->getParam("TOKEN")->value();
  File file = LittleFS.open("/config/config.json", "r");
  if (!file) {
    return request->send(400, "application/json", "{\"error\":\"Failed to open config.json for update\"}");
  }

  size_t size = file.size();
  std::unique_ptr<char[]> buf(new char[size]);
  file.readBytes(buf.get(), size);
  file.close();

  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, buf.get());
  if (error) return request->send(400, "application/json", "{\"error\":\"Failed to parse config.json for update\"}");

  //  หากไม่พบ key token_line ให้สร้างใหม่
  if (!doc.containsKey("token_line")) {
    doc["token_line"] = token_line;
    // Open file for writing
    File newFile = LittleFS.open("/config/config.json", "w");
    if (!newFile) {
      return request->send(400, "application/json", "{\"error\":\"Failed to create new config file\"}");
    }
    // Serialize JSON to file
    if (serializeJson(doc, newFile) == 0) return request->send(400, "application/json", "{\"error\":\"Failed to write to new config file\"}");
    // Close the file
    newFile.close();
    return request->send(200, "application/json", "{\"ok\":\"New config file create token line\"}");

  } else {
    // หากพบ key token_line ให้อัพเดท
    //หากพบ key timeRang ให้อัพเดทแทน
    File file = LittleFS.open("/config/config.json", "r+");
    if (!file) return request->send(400, "application/json", "{\"error\":\"Failed to open config.json for update\"}");
    // Parse the JSON object
    size_t size = file.size();
    std::unique_ptr<char[]> buf(new char[size]);
    file.readBytes(buf.get(), size);
    file.close();

    DynamicJsonDocument doc(1024);  // Adjust the size if necessary
    DeserializationError error = deserializeJson(doc, buf.get());
    if (error) return request->send(400, "application/json", "{\"error\":\"Failed to parse config file\"}");    
    doc["token_line"] = token_line;


    file = LittleFS.open("/config/config.json", "w");
    if (!file) return request->send(400, "application/json", "{\"error\":\"Failed to create new config file\"}");
    // Serialize JSON to file
    if (serializeJson(doc, file) == 0) return request->send(400, "application/json", "{\"error\":\"Failed to write to config file\"}");
    // Close the file
    file.close();
    return request->send(200, "application/json", "{\"ok\":\"Config.json updated successfully\"}");
  }
}