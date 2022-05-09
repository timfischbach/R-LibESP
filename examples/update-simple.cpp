#include <Arduino.h>
#include "R-Lib8266.h"

void setup()
{
    Serial.begin(115200);
    Serial.println("Simple Update example R-Lib8266. Lib version: " + getLibVersion());
    // R_LIB SETUP
    setDeviceName("YourDeviceName"); //Sets the Device Name for the Webserver Logfile.
    setVersion("1.0.0"); //Sets current program version to 1.0.0
    setDlLink("http://YourWebServer.com/yourFolder/"); //Sets Webserver adress to the Update files / structure.
    // R_LIB SETUP END

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
    }                                      // Then enter SSID and Password on the Website and send it. The ESP automatically saves it to EEPROM and reboots.

    while (checkWIFI() == false) // While WiFi is still not connected
    {
        connectWIFIUserHandle(); // Handle Webserver every 50ms.
        delay(50);
    }
}

void loop()
{
     dataTransmission(); //Send MAC, Version, DeviceName and Status to the Webservers Logfile
      if (checkUpdate() == "UPDATE_AVAILABLE") //Checks for Update. If Update is available
      {
         performUpdate(); // execute Update
      }
      delay(200000); // Check for Update every 200 seconds.
}