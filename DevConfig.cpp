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