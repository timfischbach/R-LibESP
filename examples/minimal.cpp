#include <Arduino.h>
#include "R-LibESP.h"
R_LibESP rlib;
void setup()
{
   // R_LIB SETUP
   rlib.setDeviceName("TestDevice");
   rlib.setVersion("v1.0.2");
   rlib.setDlLink("https://your-server.com/firmware/testdevice/");
   rlib.setSSLState(true);
   rlib.setAttemptsBeforeInsecureSSL(2);
   // R_LIB SETUP END
   delay(5000);
   Serial.begin(115200);
   Serial.print("TESTING PROGRAMM FOR R-LIBESP ");
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
      rlib.connectWIFIUser("TestDevice", "rlibdev1");
   }
   while (rlib.checkWIFI() == false)
   {
      rlib.connectWIFIUserHandle();
   }
}
   void loop()
   {
      rlib.dataTransmission("This is a status report! Everything is good :)");
      delay(1000);
      if (rlib.checkUpdate() == "UPDATE_AVAILABLE")
      {
         Serial.println(rlib.performUpdate());
      }
      delay(20000);
   }