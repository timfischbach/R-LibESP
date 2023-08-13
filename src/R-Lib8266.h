#ifndef R_Lib8266_h
#define R_Lib8266_h

#include <Arduino.h>

class R_Lib8266
{
public:
    R_Lib8266();
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
    String dataTransmission();
    void setAttemptsBeforeInsecureSSL(int sattempts);
    int getAttemptsBeforeInsecureSSL();
    void executeAttemptsBeforeInsecureSSL();

private:

};

#endif