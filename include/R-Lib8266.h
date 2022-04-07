#ifndef RLib8266
#define RLib8266

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <ESP8266mDNS.h>
#include "R-Lib8266.h"

extern const String LIBVERSION;

extern int progress;

String CheckUpdate();
String split(String s, char parser, int index);
String UpdateLoop();
String dataTransmission();
void connectWIFI(String ssid, String passwd);
bool checkWIFI();
void resetWIFI();
void saveWIFI(String ssid, String password);
String loadWIFI(String mode);
void connectWIFIUser(String ssid, String password);
void createWebServer();
String getLibVersion();
void connectWIFIUserHandle();
void endWIFIUser();
String getLibVersion();
void connectWIFIUserHandle();
void endWIFIUser();
void setDeviceName(String DevName);
String getDeviceName();
void setVersion(String ver);
String getVersion();
void setDlLink(String DLL);
String getDlLink();
void setDevLink(String DEL);
String getDevLink();
void setDevTag(String DTAG);
String getDevTag();
void setBetaState(bool sbeta);
bool getBetaState();
void setDevState(bool sdev);
bool getDevState();
bool varCheck();

#endif