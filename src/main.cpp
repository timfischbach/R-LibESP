#include <Arduino.h>
#include "R-Lib8266.h"

void setup()
{
   // R_LIB SETUP
   setDeviceName("TestDevice");
   setVersion("1.0.0a");
   setDlLink("http://dl.timfischbach.com/prv/firmware/testdevice/");
   // R_LIB SETUP END
   delay(3000);
   Serial.begin(115200);
   Serial.println("TESTING PROGRAMM FOR R-LIB8266");
   Serial.println(getLibVersion());
   connectWIFI(loadWIFI("SSID"), loadWIFI("PASS"));
   int c = 0;
   while (checkWIFI() == false and c < 100)
   {
      delay(100);
      c++;
   }
   if (checkWIFI() == false)
   {
      connectWIFIUser("TestDevice", "");
   }
   while (checkWIFI() == false)
   {
      connectWIFIUserHandle();
      delay(50);
   }
}
   void loop()
   {
      dataTransmission();
      delay(1000);
      if (checkUpdate() == "UPDATE_AVAILABLE")
      {
         performUpdate();
      }
      delay(20000);
   }