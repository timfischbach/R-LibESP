#ifndef R_LibESP_h
#define R_LibESP_h

#include <Arduino.h>
#include <DNSServer.h>
#include <EEPROM.h>
#include <time.h>

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <ESP8266WebServer.h>
#elif defined(ESP32)
#include <WiFi.h>
#include <HTTPClient.h>
#include <HttpUpdate.h>
#include <WebServer.h>
#include <WiFiClientSecure.h>
#include <Preferences.h>
#else
#error "This is not an Espressif board!"
#endif
#include "R-LibESP.h"

class R_LibESP
{
public:
    R_LibESP();
    String checkUpdate();
    String split(String s, char parser, int index);
    String performUpdate();
    void connectWIFI();
    bool checkWIFI();
    void resetWIFI();
    void saveWIFI(String ssid, String password);
    void connectWIFIUser(String ssid, String password);
    void createWebServer();
    String getLibVersion();
    void endWIFIUser();
    void setDeviceName(String DevName);
    String getDeviceName();
    void setVersion(String ver);
    String getVersion();
    void setDlLink(String DLL);
    String getDlLink();
    void setBetaState(bool sbeta);
    bool getBetaState();
    void setDevState(bool sdev);
    bool getDevState();
    bool varCheck();
    void saveOV(String oldversion);
    String loadOV();
    void updateStarted();
    void updateFinished();
    void updateProgress(int cur, int total);
    void updateError(int err);
    void setSSLRootCertificate(char *certificate);
    bool getSSLState();
    void setSSLState(bool sssl);
    void setSSLInsecureState(bool sinsecure);
    bool getSSLInsecureState();
    void SSLSetup();
    String dataTransmission(String status = "OK");
    void setAttemptsBeforeInsecureSSL(int sattempts);
    int getAttemptsBeforeInsecureSSL();
    void executeAttemptsBeforeInsecureSSL();
    String urlEncode(String str);
    void connectWIFIUserHandle();

private:
};

#endif