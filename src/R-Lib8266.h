#ifndef RLib8266
#define RLib8266

#include <Arduino.h>

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