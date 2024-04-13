
#include <Arduino.h>
#include "WlanManager.h"
#include "Router.h"
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <LittleFS.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <TridentTD_LineNotify.h>

#include "display.h"
#include "variable.h"

// สร้างออปเจ็กต์ WlanManager
WlanManager wlan;
//สร้างออบเจ็กต์ mServer
Router router;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "time.google.com", 3600, 60000);

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress ds_one, ds_two;

 

void setup() {
  Serial.begin(115200);
  pinMode(button_Pin, INPUT_PULLUP);
  pinMode(button_test, INPUT_PULLUP);
  // สร้าง SSID ขึ้นมาจาก Chip ID
  String apSsid = "ESP_" + wlan.chipID();

  // กำหนดรหัส WiFi สำหรับโหมด Access Point
  const char* apPassword = "password";
 if (!LittleFS.begin()) {
    Serial.println("LittleFS Mount Failed");
    return;
  }
  
  if (digitalRead(button_Pin) == HIGH) {    
    if(wlan.WlanSetup()){
      Serial.println("Wifi Connect OK.");
    }
  } else if (digitalRead(button_Pin) == LOW) {
    // เริ่มโหมด Access Point
    wlan.startAP(apSsid.c_str(), apPassword);
  }  
  String ListSsid = wlan.scanNetwork();
  delay(100);
  router.setScanNetwork(ListSsid);
//  ################
  router.begin();
// ############## ตั้งค่า NTP Client ####################
timeClient.begin();
  File ntpCfg = LittleFS.open("/config/ntp.txt", "r");
  if (!ntpCfg) {
    ntpCfg.close();
    Serial.println("Failed to open file NTP");
  } else {
    String data;
    while (ntpCfg.available()) {
      char c = ntpCfg.read();
      data += c;
    }
    ntpCfg.close();
    int str_len = data.length() + 1;
    char ntp[str_len];
    data.toCharArray(ntp, str_len);
    timeClient.setPoolServerName(ntp);
  }
// ######################################################
// ########## Sensor ####################################
sensors.begin();
sensors.setWaitForConversion(false);
// ########## LCD Disply ##################
Wire.begin(SDA, SCL);
lcd.init(); //Windows
lcd.backlight();  //เปิดไฟ backlight
//lcd.noBacklight(); //ปิดไฟ backlight

LINE.setToken(TOKEN);

}
// Show data to LCD
void printTemperature(DeviceAddress deviceAddress, String number){
  float tempC = sensors.getTempC(deviceAddress);
  float tempF = DallasTemperature::toFahrenheit(tempC);
  Serial.print("Temp C: ");
  Serial.print(tempC);
  Serial.print(" Temp F: ");
  Serial.print(tempF);
  Serial.println("  Number " + number);

  if (number == "T1") {
    tempOneC = tempC;
    tempOneF = tempF;
  } else if (number == "T2") {
    tempTwoC = tempC;
    tempTwoF = tempF;
  }else{
    tempTwoC = DEVICE_DISCONNECTED;
    tempTwoF = DEVICE_DISCONNECTED;
  }
}
// Check Alarm
void checkAlarm(DeviceAddress deviceAddress, String number){
  if (sensors.hasAlarm(deviceAddress)) {
    String tempC = "-127";
    if (number == "T1") {
      tempC = String(tempOneC);
    } else if (number == "T2") {
      tempC = String(tempTwoC);
    }
    Serial.print("เซ็นนเซอร์ " + number + " อุณหภูมิ เกินกำหนด "); Serial.print(tempC); Serial.println(" องศา");
    if (TOKEN.length() > 30) LINE.notify(location + " เซ็นนเซอร์ " + number + "อุณหภูมิิิเกินกำหนด " + tempC + " องศา");
  }
}

void testNotify(DeviceAddress deviceAddress, String number){
  String tempC = "-127";
    if (number == "T1") {
      tempC = String(tempOneC);
    } else if (number == "T2") {
      tempC = String(tempTwoC);
    }
    Serial.print("ทดสอบเซ็นเซอร์ " + number + " อุณหภูมิ "); Serial.print(tempC); Serial.println(" องศา");
    if (TOKEN.length() > 30) LINE.notify(location + " ทดสอบเซ็นเซอร์ " + number + "อุณหภูมิ " + tempC + " องศา");
  }

void clearLCDLine(int line) {
  for (int n = 0; n < 16; n++) { // 20 indicates symbols in line. For 2x16 LCD write - 16
    lcd.setCursor(n, line);
    lcd.print(" ");
  }
  lcd.setCursor(0, line);            // set cursor in the beginning of deleted line
}

void loop() {
  sensors.requestTemperatures();
  // โค้ดใน loop ที่คุณต้องการ
  // #############################################
  timeClient.update();
  int hour = timeClient.getHours();
  int mint = timeClient.getMinutes();
  int sec = timeClient.getSeconds();

if (TOKEN.length() > 10){
  // ############### ส่งแจ้งเตื่อนเมือถึงเวลาที่ตั้งไว้ 16:00:00
    if(hour == 16 && mint == 0 && sec <= 1){
      testNotify(ds_one, "T1");
      delay(100);
      testNotify(ds_two, "T2");
      delay(900);
    }
    // ############### ส่งแจ้งเตื่อนเมือถึงเวลาที่ตั้งไว้ 23:59:00
    else if(hour == 23 && mint == 59 && sec <= 1){
      testNotify(ds_one, "T1");
      delay(100);
      testNotify(ds_two, "T2");
      delay(900);
    }
}
  // #############################################
 //เซนเซอร์ตัวที่ 1
    if (sensors.getAddress(ds_one, 0)) {
    printTemperature(ds_one, "T1");    
    clearLCDLine(0);
    lcd.print("1");
    lcd.write(2);
    if (tempOneC < 0) {
      lcd.setCursor(3, 0);      
    } else {
      lcd.setCursor(4, 0);      
    }
    lcd.print(tempOneC, 0);
    lcd.setCursor(6, 0);
    lcd.write(1);
    lcd.print("C");
    lcd.setCursor(12, 0);
    lcd.print(daysOfTheWeek[timeClient.getDay()]);
    delay(1000);
  } else {
    tempOneC = DEVICE_DISCONNECTED;
    tempOneF = DEVICE_DISCONNECTED;
    clearLCDLine(0);
    lcd.print("T1 DISCONNECTED");
    delay(500);
  }

  //เซนเซอร์ตัวที่ 2
    if (sensors.getAddress(ds_two, 1)) {
    printTemperature(ds_two, "T2");
    clearLCDLine(1);
    lcd.print("2");
    lcd.write(2);
    if (tempTwoC < 0) {
      lcd.setCursor(3, 1);
    } else {
      lcd.setCursor(4, 1);
    }
    lcd.print(tempTwoC, 0);
    lcd.setCursor(6, 1);
    lcd.write(1);
    lcd.print("C");
    lcd.setCursor(11, 1);
    lcd.print(timeClient.getFormattedTime());
    delay(1000);
  } else {
    tempTwoC = -127;
    tempTwoF = -127;
    clearLCDLine(1);
    lcd.print("T2 DISCONNECTED");
    delay(500);
  }
//ตรวจอุณหภูมิเกินกำหนด
unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    checkAlarm(ds_one, "T1");
    delay(100);
    checkAlarm(ds_two, "T2");
    //ส่งค่าอุณหภูมิไปเครื่อง SERVER
    // if (urlApi.length() > 10) postDataToServer(urlApi);    
  }

  if (previousMillis >= interval_restart) {
    LINE.notify("อุปกรณ์รีสตาร์ทเนื่งจากทำงานครบ" + String(day_time) +"วัน");
    delay(500);
    ESP.restart();
  }
  if (digitalRead(button_test) == HIGH){
      testNotify(ds_one, "T1");
      delay(100);
      testNotify(ds_two, "T2");
      delay(900);
  }
  router.setTemperature(tempOneC, tempTwoC);
}
