#ifndef mServer_h
#define mServer_h

#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include "LittleFS.h"
#include <ESPAsyncWebServer.h>


class mServer {
  public:
    void begin();
    void start();
    String processor(const String path);
};

#endif
