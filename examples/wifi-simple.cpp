#include <Arduino.h>
#include "R-Lib8266.h"

void setup()
{
    Serial.begin(115200);
    Serial.println("Simple WIFI example R-Lib8266. Lib version: " + getLibVersion());

    connectWIFI(loadWIFI("SSID"), loadWIFI("PASS")); // Connect to WIFI using stored SSID and Password in EEPROM

    // connectWIFI("TESTWIFI", "WIFIPASSWORD"); //Connect to WIFI using SSID and PASSWORD you coded here. (Would not recommend)

    int c = 0;
    while (checkWIFI() == false and c < 100) // While trying to connect to Wifi, wait.
    {
        delay(100);
        c++;
    }

    if (checkWIFI() == false) // If WiFi is still not connected
    {
        connectWIFIUser("TestDevice", ""); // Launch Hotspot with SSID "TestDevice" and Password "". Connect to this WIFI with your phone/pc and enter the following ip: 192.168.4.1
    }                                      //Then enter SSID and Password on the Website and send it. The ESP automatically saves it to EEPROM and reboots.

     while (checkWIFI() == false) // While WiFi is still not connected
    {
        connectWIFIUserHandle(); //Handle Webserver every 50ms.
        delay(50);
    }
}

void loop()
{
}