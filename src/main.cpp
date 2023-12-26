#include <Arduino.h>
#include "R-Lib8266.h"
R_Lib8266 rlib;
void setup()
{
   // R_LIB SETUP
   rlib.setDeviceName("TestDevice");
   rlib.setVersion("v1.0.1a");
   rlib.setDlLink("https://dl.timfischbach.com/prv/firmware/testdevice/");
   rlib.setSSLState(true);
   rlib.setAttemptsBeforeInsecureSSL(2);
   // R_LIB SETUP END
   delay(5000);
   Serial.begin(115200);
   Serial.print("TESTING PROGRAMM FOR R-LIB8266 ");
   Serial.println(rlib.getLibVersion());
   rlib.connectWIFI();
   int c = 0;
   while (rlib.checkWIFI() == false and c < 100)
   {
      delay(100);
      c++;
   }
   if (rlib.checkWIFI() == false)
   {
      rlib.connectWIFIUser("TestDevice", "");
   }
   while (rlib.checkWIFI() == false)
   {
      rlib.connectWIFIUserHandle();
      delay(50);
   }
}
   void loop()
   {
      rlib.dataTransmission("Hallo! Ich habe - viele ' Leerzeichen und so");
      delay(1000);
      if (rlib.checkUpdate() == "UPDATE_AVAILABLE")
      {
         Serial.println(rlib.performUpdate());
      }
      delay(20000);
   }