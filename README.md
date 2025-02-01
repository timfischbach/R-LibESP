# R-LibESP
A library for easy HTTP OTA updates with 3 sources (stable, beta, dev) and WIFI connection, used on Espressif devices (ESP8266, ESP32) by Tim Fischbach.
FEATURES:
- User based Wifi config
- Sys intern Wifi config
- Update Functions over 3 ways! (Stable, Beta or Dev)
- Data Transmission to Update-server for device status.
- Detecting unwanted Update loops
- HTTP and HTTPS Update possibilities
- Overwrite function for expired SSL root certificate

IMPORTANT: WHEN USING THIS LIBRARY, MAKE SURE NOT TO USE EEPROM BYTES 4084 - 4095. THOSE ARE RESERVED FOR THIS LIBRARY. OVERWRITING THOSE WITH DATA CAN CAUSE YOUR PROGRAM TO BRICK!

CHECK THE WIKI FOR MORE INFOS.

# CURRENTLY ONLY WORKING WITH PLATFORMIO. Arduino IDE Release halted...

TO DO:
- Github Integration
- MD5 Check instead of Version check -> ULProtection deprecation


TO EVENTUALLY DO:
- (WebMGMTServer)
- (Config MGMT)
- (Rebranding into R-ESPOTA Library)
- (Arduino IDE Release)
