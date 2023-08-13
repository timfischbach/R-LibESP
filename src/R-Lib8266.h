#ifndef R_Lib8266_h
#define R_Lib8266_h

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <ESP8266mDNS.h>

class R_Lib8266
{
public:
    R_Lib8266();


    String checkUpdate();
    String secureCheckUpdate();
    String split(String s, char parser, int index);
    String performUpdate();
    String securePerformUpdate();
    String dataTransmission();
    void connectWIFI();
    bool checkWIFI();
    void resetWIFI();
    void saveWIFI(String ssid, String password);
    void connectWIFIUser(String ssid, String password);
    void createWebServer();
    String getLibVersion();
    void connectWIFIUserHandle();
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
    void update_started();
    void update_finished();
    void update_progress(int cur, int total);
    void update_error(int err);

private:


    String libVersion;
    int progress;
    String deviceName;
    String version;
    String dlLink;
    bool betaState;
    bool devState;
};

#endif