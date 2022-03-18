#ifndef R-Lib8266
#define R-Lib8266

extern const String LIBVERSION;

extern int progress;

String CheckUpdate();
String split(String s, char parser, int index);
String UpdateLoop();
void dataTransmission();
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

#endif