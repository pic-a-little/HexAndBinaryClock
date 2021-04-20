#pragma once

#include <Arduino.h>

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <SPIFFS.h>
#include <FS.h>

#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>


#define APMODE (1)
#define CLIENTMODE (0)

extern WebServer server;

extern void getSPIFFSSetting();
extern void setAccessPointMode();
extern void setClientMode(int *WiFiMode);

extern bool connectionSuccess;

#define MYSSID "mySSID"
#define MYSSIDPASSWD "myPassword"

void setArduinoOTA();

// //OTA関連
// extern void setupOTA();

