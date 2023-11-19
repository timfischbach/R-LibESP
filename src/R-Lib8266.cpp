/*
TO DO:
- Create a Coding Structure for verifying signed binaries.
- Redirection (Log In WIFI)
*/

/*
R-Lib8266 by Tim Fischbach
Functions:
- User based Wifi config
- Sys intern Wifi config
- Update Functions over 3 ways! (Stable, Beta or Dev)
- Data Transmission to Update-server for stats.
- Detecting unwanted Update loops
- HTTP and HTTPS Update possibilities
- Overwrite function for expired Root Certificate
*/
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <time.h>
#include "R-Lib8266.h"

R_Lib8266::R_Lib8266()
{
}

const String LIBVERSION = "v2.0.0";

String strInit, initLink, binLink, content, st, deviceName, version, dlLink;
int serverStatus, statusCode;
unsigned long prohibitUpdateMillis = 0;
bool betaState = false;
bool updateLock = false;
bool lockPassed = false;
bool timeAndCertificateLoaded = false;
bool devState = false;
bool sslState = false;
bool insecureState = false;
int attemptsBeforeInsecureSSL = 0;

char rootCACertificate[] = R"EOF(
-----BEGIN CERTIFICATE-----
MIIFYDCCBEigAwIBAgIQQAF3ITfU6UK47naqPGQKtzANBgkqhkiG9w0BAQsFADA/
MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT
DkRTVCBSb290IENBIFgzMB4XDTIxMDEyMDE5MTQwM1oXDTI0MDkzMDE4MTQwM1ow
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwggIiMA0GCSqGSIb3DQEB
AQUAA4ICDwAwggIKAoICAQCt6CRz9BQ385ueK1coHIe+3LffOJCMbjzmV6B493XC
ov71am72AE8o295ohmxEk7axY/0UEmu/H9LqMZshftEzPLpI9d1537O4/xLxIZpL
wYqGcWlKZmZsj348cL+tKSIG8+TA5oCu4kuPt5l+lAOf00eXfJlII1PoOK5PCm+D
LtFJV4yAdLbaL9A4jXsDcCEbdfIwPPqPrt3aY6vrFk/CjhFLfs8L6P+1dy70sntK
4EwSJQxwjQMpoOFTJOwT2e4ZvxCzSow/iaNhUd6shweU9GNx7C7ib1uYgeGJXDR5
bHbvO5BieebbpJovJsXQEOEO3tkQjhb7t/eo98flAgeYjzYIlefiN5YNNnWe+w5y
sR2bvAP5SQXYgd0FtCrWQemsAXaVCg/Y39W9Eh81LygXbNKYwagJZHduRze6zqxZ
Xmidf3LWicUGQSk+WT7dJvUkyRGnWqNMQB9GoZm1pzpRboY7nn1ypxIFeFntPlF4
FQsDj43QLwWyPntKHEtzBRL8xurgUBN8Q5N0s8p0544fAQjQMNRbcTa0B7rBMDBc
SLeCO5imfWCKoqMpgsy6vYMEG6KDA0Gh1gXxG8K28Kh8hjtGqEgqiNx2mna/H2ql
PRmP6zjzZN7IKw0KKP/32+IVQtQi0Cdd4Xn+GOdwiK1O5tmLOsbdJ1Fu/7xk9TND
TwIDAQABo4IBRjCCAUIwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMCAQYw
SwYIKwYBBQUHAQEEPzA9MDsGCCsGAQUFBzAChi9odHRwOi8vYXBwcy5pZGVudHJ1
c3QuY29tL3Jvb3RzL2RzdHJvb3RjYXgzLnA3YzAfBgNVHSMEGDAWgBTEp7Gkeyxx
+tvhS5B1/8QVYIWJEDBUBgNVHSAETTBLMAgGBmeBDAECATA/BgsrBgEEAYLfEwEB
ATAwMC4GCCsGAQUFBwIBFiJodHRwOi8vY3BzLnJvb3QteDEubGV0c2VuY3J5cHQu
b3JnMDwGA1UdHwQ1MDMwMaAvoC2GK2h0dHA6Ly9jcmwuaWRlbnRydXN0LmNvbS9E
U1RST09UQ0FYM0NSTC5jcmwwHQYDVR0OBBYEFHm0WeZ7tuXkAXOACIjIGlj26Ztu
MA0GCSqGSIb3DQEBCwUAA4IBAQAKcwBslm7/DlLQrt2M51oGrS+o44+/yQoDFVDC
5WxCu2+b9LRPwkSICHXM6webFGJueN7sJ7o5XPWioW5WlHAQU7G75K/QosMrAdSW
9MUgNTP52GE24HGNtLi1qoJFlcDyqSMo59ahy2cI2qBDLKobkx/J3vWraV0T9VuG
WCLKTVXkcGdtwlfFRjlBz4pYg1htmf5X6DYO8A4jqv2Il9DjXA6USbW1FzXSLr9O
he8Y4IWS6wY7bCkjCWDcRQJMEhg76fsO3txE+FiYruq9RUWhiF1myv4Q6W+CyBFC
Dfvp7OOGAN6dEOM4+qR9sdjoSYKEBpsr6GtPAQw4dy753ec5
-----END CERTIFICATE-----
)EOF";

ESP8266WebServer server(80);
WiFiClient updatewificlient;
HTTPClient http;
BearSSL::WiFiClientSecure secureupdatewificlient;

void R_Lib8266::SSLSetup()
{
  if (timeAndCertificateLoaded == false)
  {
    Serial.println("[SSL Setup] Started SSL Setup!");
    configTime(2 * 3600, 0, "pool.ntp.org", "time.nist.gov");

    Serial.print("[SSL Setup] Waiting for NTP time sync...");
    time_t now = time(nullptr);
    while (now < 8 * 3600 * 2)
    {
      delay(500);
      Serial.print(".");
      now = time(nullptr);
    }
    Serial.println("");
    struct tm timeinfo;
    gmtime_r(&now, &timeinfo);
    Serial.print("[SSL Setup] Current time: ");
    Serial.print(asctime(&timeinfo));
    Serial.println("[SSL Setup] Loading X509 Certificate...");
    secureupdatewificlient.setTrustAnchors(new BearSSL::X509List(rootCACertificate));
    timeAndCertificateLoaded = true;
    Serial.println("[SSL Setup] Finished SSL Setup!");
  }
  else
  {
    Serial.println("[SSL Setup] Time and Certificate already set!");
  }
}

void R_Lib8266::updateStarted()
{
  Serial.println("[Update] HTTP update process started");
}

void R_Lib8266::updateFinished()
{
  Serial.print("\n");
  Serial.println("[Update] HTTP update process finished");
  EEPROM.begin(4096);
  Serial.println("[Update] Update OK!");
  Serial.println("[Update] Saving old version to prevent Update loop...");
  Serial.println(getVersion());
  String ver = getVersion();
  saveOV(ver);
  Serial.println("[Update] Done writing old version to EEPROM.");
}

void R_Lib8266::updateProgress(int cur, int total)
{
  int progress = cur * 100 / total;
  Serial.printf("[Update] HTTP update process at %d of %d bytes. Total Progress: %d % \r", cur, total, progress);
}

void R_Lib8266::updateError(int err)
{
  Serial.printf("[Update] HTTP update fatal error code %d\r", err);
}

String R_Lib8266::split(String inputString, char splitChar, int index)
{
  String rs = "";
  int parserIndex = index;
  int parserCnt = 0;
  int rFromIndex = 0, rToIndex = -1;
  while (parserIndex >= parserCnt)
  {
    rFromIndex = rToIndex + 1;
    rToIndex = inputString.indexOf(splitChar, rFromIndex);
    if (parserIndex == parserCnt)
    {
      if (rToIndex == 0 || rToIndex == -1)
        return "";
      return inputString.substring(rFromIndex, rToIndex);
    }
    else
      parserCnt++;
  }
  return rs;
}

void R_Lib8266::connectWIFI()
{
  Serial.println("[WIFI] Connecting to WIFI...");
  String hostname = getDeviceName() + "-" + split(WiFi.macAddress() + ':', ':', 4) + split(WiFi.macAddress() + ':', ':', 5);
  WiFi.hostname();
  WiFi.persistent(false);
  WiFi.begin();
}

bool R_Lib8266::checkWIFI()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    return false;
  }
  else
  {
    return true;
  }
}

void R_Lib8266::resetWIFI()
{
  WiFi.persistent(true);
  WiFi.disconnect();
  WiFi.persistent(false);
  Serial.println("[WIFI] WIFI reset successful.");
}

void R_Lib8266::saveWIFI(String ssid, String password)
{
  Serial.println("[WIFI] Writing SSID to EEPROM...");
  WiFi.persistent(true);
  WiFi.begin(ssid, password);
  delay(5000);
  WiFi.persistent(false);
  WiFi.disconnect();
  Serial.println("[WIFI] Done writing SSID to EEPROM.");
}

void R_Lib8266::connectWIFIUser(String ssid, String password)
{
  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  int n = WiFi.scanNetworks();
  Serial.println("[WIFI] Scan done");
  if (n == 0)
  {
    Serial.println("[WIFI] No networks found");
  }
  else
  {
    Serial.print("[WIFI] ");
    Serial.print(n);
    Serial.println(" networks found!");
  }
  Serial.println("");
  st = "<ol>";
  for (int i = 0; i < n; ++i)
  {
    // Print SSID and RSSI for each network found
    st += "<li>";
    st += WiFi.SSID(i);
    st += " (";
    st += WiFi.RSSI(i);

    st += ")";
    st += (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*";
    st += "</li>";
  }
  st += "</ol>";
  delay(100);
  ssid = deviceName + "-" + split(WiFi.macAddress() + ':', ':', 4) + split(WiFi.macAddress() + ':', ':', 5);
  WiFi.softAP(ssid, password);
  Serial.println("[WIFI] Starting user config mode...");
  Serial.println("");
  if (WiFi.status() == WL_CONNECTED)
    Serial.println("[WIFI] Accesspoint created!");
  Serial.print("[WIFI] Local IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("[WIFI] SoftAP IP: ");
  Serial.println(WiFi.softAPIP());
  createWebServer();
  // Start the server
  server.begin();
  Serial.println("[WIFI] Server started");
  Serial.println("[WIFI] User config mode successfully started!");
}

void R_Lib8266::createWebServer()
{
  {
    server.on("/", []()
              {
                IPAddress ip = WiFi.softAPIP();
                String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
                content = "<!DOCTYPE HTML>\r\n<html>R-Lib8266 " + LIBVERSION + " by Tim Fischbach at ";
                content += ipStr;
                content += "<form action=\"/scan\" method=\"POST\"><input type=\"submit\" value=\"scan\"></form>";
                content += "<p>";
                content += st;
                content += "</p><form method='get' action='setting'><label>SSID: </label><input name='ssid' length=32><label> Password: </label><input name='pass' length=64><input type='submit'></form>";
                content += "</html>";
                server.send(200, "text/html", content); });
    server.on("/scan", []()
              {
                //setupAP();
                IPAddress ip = WiFi.softAPIP();
                String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);

                content = "<!DOCTYPE HTML>\r\n<html>Scan success! Please go back!";
                content += "<form action=\"/\" method=\"POST\"><input type=\"submit\" value=\"back\"></form>";
                server.send(200, "text/html", content); });

    server.on("/setting", [=]()
              {
                String qsid = server.arg("ssid");
                String qpass = server.arg("pass");
                if (qsid.length() > 0 && qpass.length() > 0)
                {
                  saveWIFI(qsid, qpass);

                  content = "{\"Success\":\"saved to eeprom... reset to boot into new wifi\"}";
                  statusCode = 200;
                  ESP.reset();
                }
                else
                {
                  content = "{\"Error\":\"404 not found\"}";
                  statusCode = 404;
                  Serial.println("Sending 404");
                }
                server.sendHeader("Access-Control-Allow-Origin", "*");
                server.send(statusCode, "application/json", content); });
  }
}

String R_Lib8266::getLibVersion()
{
  return LIBVERSION;
}

void R_Lib8266::connectWIFIUserHandle()
{
  server.handleClient();
}

void R_Lib8266::endWIFIUser()
{
  server.stop();
}

void R_Lib8266::setDeviceName(String DevName)
{
  deviceName = DevName;
}

String R_Lib8266::getDeviceName()
{
  return deviceName;
}

void R_Lib8266::setVersion(String ver)
{
  version = ver;
}

String R_Lib8266::getVersion()
{
  return version;
}

void R_Lib8266::setDlLink(String DLL)
{
  dlLink = DLL;
}

String R_Lib8266::getDlLink()
{
  return dlLink;
}

void R_Lib8266::setBetaState(bool sbeta)
{
  betaState = sbeta;
}

bool R_Lib8266::getBetaState()
{
  return betaState;
}

void R_Lib8266::setDevState(bool sdev)
{
  devState = sdev;
}

bool R_Lib8266::getDevState()
{
  return devState;
}

void R_Lib8266::setSSLState(bool sssl)
{
  sslState = sssl;
}

bool R_Lib8266::getSSLState()
{
  return sslState;
}

bool R_Lib8266::varCheck()
{
  if (devState == false)
  {
    if (deviceName.length() == 0 || version.length() == 0 || dlLink.length() == 0)
    {
      return false;
    }
    else
    {
      return true;
    }
  }
  else
  {
    if (deviceName.length() == 0 || version.length() == 0 || dlLink.length() == 0)
    {
      return false;
    }
    else
    {
      return true;
    }
  }
}

void R_Lib8266::saveOV(String oldversion)
{
  EEPROM.begin(4096);
  Serial.println("[ULProtection] Saving old version number to EEPROM...");
  if (oldversion.length() > 10)
  {
    Serial.println("[ULProtection] ERROR! version STRING TOO LONG TO SAVE! REPORTING IT TO THE SERVER!");
    dataTransmission("ERROR:%20VERSION%20STRING%20TOO%20LONG!%20PLEASE%20SET%20YOUR%20VERSION%20STRING%20TO%20MAX.%2010%20CHARACTERS%20LONG!");
  }
  else
  {
    for (int i = 0; i < oldversion.length(); i++)
    {
      EEPROM.write(i + 4085, oldversion[i]);
    }
    EEPROM.commit();
    EEPROM.end();
    Serial.println("[ULProtection] Saved old version number to EEPROM.");
  }
}

String R_Lib8266::loadOV()
{
  String oldVersion;
  EEPROM.begin(4096);
  Serial.println("[ULProtection] Loading old version number from EEPROM...");
  String ver = getVersion();
  String esid;
  for (int i = 0; i < ver.length(); i++)
  {
    char output = EEPROM.read(i + 4085);
    esid += output;
  }
  oldVersion = esid.c_str();
  EEPROM.end();
  Serial.print("[ULProtection] Loaded old version number from EEPROM: ");
  Serial.println(oldVersion);
  return oldVersion;
}

String R_Lib8266::checkUpdate()
{
  const int maxFiles = 100;
  String fileNames[maxFiles];
  int fileCount = 0;
  String url;

  // Make HTTPS request to server directory
  if (varCheck() == false)
  {
    Serial.println("[Update] ERROR: VARIABLES NOT SET IN R-LIB");
    return "NO_VARIABLES_SET";
  }
  if (betaState == true)
  {
    url = dlLink + "beta/";
  }
  else
  {
    url = dlLink;
  }
  if (sslState == true)
  {
    R_Lib8266::SSLSetup();
    if (insecureState == true)
    {
      secureupdatewificlient.setInsecure();
      Serial.println("[Secure Update] WARNING! Insecure mode activated! Certificate will not be checked!");
    }
    http.begin(secureupdatewificlient, url);
    Serial.println("[Secure Update] SSL enabled!");
  }
  else
  {
    http.begin(updatewificlient, url);
  }

  int httpResponseCode = http.GET();
  if (httpResponseCode == 301)
  {
    // Follow the redirect to the new URL
    String newUrl = http.getString().substring(9);
    http.end();
    if (sslState == true)
    {
      if (insecureState == true)
      {
        secureupdatewificlient.setInsecure();
        Serial.println("[Secure Update] WARNING! Insecure mode activated! Certificate will not be checked!");
      }
      http.begin(secureupdatewificlient, newUrl);
    }
    else
    {
      http.begin(updatewificlient, newUrl);
    }
    httpResponseCode = http.GET();
  }
  if (httpResponseCode == 200)
  {
    String response = http.getString();
    int linkStart = response.indexOf("<a href=");
    while (linkStart != -1)
    {
      int linkEnd = response.indexOf("</a>", linkStart);
      // Extract file name from HTML link
      int nameStart = response.indexOf("\">", linkStart) + 2;
      int nameEnd = response.indexOf("</a>", nameStart);
      String fileName = response.substring(nameStart, nameEnd);
      // Check if file name contains "." and doesn't contain "alt="
      if (fileName.indexOf(".") != -1 && fileName.indexOf("alt=") == -1)
      {
        if (fileCount < maxFiles)
        {
          fileNames[fileCount] = fileName;
          fileCount++;
        }
        else
        {
          Serial.println("Max files reached.");
          break;
        }
      }
      // Find the next link
      linkStart = response.indexOf("<a href=", linkEnd);
    }
    http.end(); // Close HTTP connection
    if (sslState == true)
    {
      secureupdatewificlient.stop();
    }
    else
    {
      updatewificlient.stop();
    }
    // Print the saved file names
    Serial.println("[Update] Files found:");
    for (int i = 0; i < fileCount; i++)
    {
      Serial.println("[Update] " + fileNames[i]);
    }
    String DEVICENAMEFileName = split(fileNames[0], '_', 0);
    for (int i = 0; i < fileCount; i++)
    {
      fileNames[i] = split(fileNames[i] + '_', '_', 1);
      fileNames[i] = split(fileNames[i], '.', 0) + '.' + split(fileNames[i], '.', 1) + '.' + split(fileNames[i], '.', 2);
    }
    Serial.println("[Update] Extracting Versions:");
    for (int i = 0; i < fileCount; i++)
    {
      Serial.println("[Update] " + fileNames[i]);
    }
    //------------------------------version COMPARING----------------------------------------

    int numVersions = sizeof(fileNames) / sizeof(fileNames[0]);

    int currentMajor, currentMinor, currentPatch;
    sscanf(version.c_str(), "v%d.%d.%d", &currentMajor, &currentMinor, &currentPatch);

    int latestVersionIndex = -1;
    int latestMajor = currentMajor, latestMinor = currentMinor, latestPatch = currentPatch;
    char latestVersion[11] = "";
    for (int i = 0; i < numVersions; i++)
    {
      String versionStringPre = fileNames[i];
      const char *versionString = versionStringPre.c_str();
      int major, minor, patch;
      sscanf(versionString, "v%d.%d.%d", &major, &minor, &patch);
      if (major > currentMajor || (major == currentMajor && minor > currentMinor) || (major == currentMajor && minor == currentMinor && patch > currentPatch))
      {
        if (latestVersionIndex == -1 || major > latestMajor || (major == latestMajor && minor > latestMinor) || (major == latestMajor && minor == latestMinor && patch > latestPatch))
        {
          latestVersionIndex = i;
          latestMajor = major;
          latestMinor = minor;
          latestPatch = patch;
          strncpy(latestVersion, versionString, sizeof(latestVersion));
        }
      }
    }

    if (latestVersionIndex == -1)
    {
      if (devState == false)
      {
        Serial.println("[Update] No update available!\r");
        return "NO_UPDATE_AVAILABLE";
      }
      else
      {
        Serial.println("[Update] No update available, but dev mode is active. Creating DLLink anyways: \r");
        binLink = dlLink + DEVICENAMEFileName + "_" + latestVersion + ".bin";
        Serial.println("[Update] " + binLink);
        return "UPDATE_AVAILABLE";
      }
    }
    else
    {
      Serial.println("[Update] Successfully connected to the update server!\r");
      Serial.println("[Update] ***READING UPDATE INFOS***");
      Serial.println("[Update] - Name of the device: " + DEVICENAMEFileName);
      Serial.print("[Update] - Update Version: ");
      Serial.println(latestVersion);
      Serial.println("[Update] ***READING SYSTEM INFOS***");
      Serial.println("[Update] - Device Name: " + deviceName);
      Serial.println("[Update] - Installed Version: " + version);
      Serial.println("[Update] - Beta enabled: " + String(betaState));
      Serial.println("[Update] - SSL enabled: " + sslState);
      Serial.println("[Update] Update available!");
      Serial.println("[Update] Creating DLLink: ");
      binLink = dlLink + DEVICENAMEFileName + "_" + latestVersion + ".bin";
      Serial.println("[Update] " + binLink);
      return "UPDATE_AVAILABLE";
    }
  }
  else
  {
    Serial.print("[Update] Error connecting to the server: ");
    Serial.println(httpResponseCode);
    executeAttemptsBeforeInsecureSSL();
    return "UPDATE_SERVER_DOWN";
  }
}

String R_Lib8266::performUpdate()
{
  if (sslState == true)
  {
    R_Lib8266::SSLSetup();
    if (insecureState == true)
    {
      secureupdatewificlient.setInsecure();
      Serial.println("[Secure Update] WARNING! Insecure mode activated! Certificate will not be checked!");
    }
  }
  if (updateLock == true and prohibitUpdateMillis + 86400000 > millis())
  {
    long timeleft = prohibitUpdateMillis + 86400000 - millis();
    timeleft = timeleft / 1000;
    Serial.println("[Update] ERROR: UPDATE BLOCKED BY UPDATE LOOP PROTECTION");
    Serial.println("[ULProtection] Time till updating is possible: " + timeleft);
    return "UPDATE_LOOP_BLOCK";
  }
  else if (updateLock == true and prohibitUpdateMillis + 86400000 < millis())
  {
    Serial.println("[ULProtection] 24 hours passed. Trying to update again!");
    lockPassed = true;
  }
  Serial.println("[ULProtection] Checking old version number...");
  String oldver;
  String ver = getVersion();
  oldver = loadOV();
  if (oldver == ver)
  {
    EEPROM.begin(4096);
    int stat = EEPROM.read(4095);
    EEPROM.end();
    stat++;
    if (stat == 1 or lockPassed == true)
    {
      Serial.println("[ULProtection] Downgrade or possible Update loop detected. Continuing with update...");
      EEPROM.begin(4096);
      EEPROM.write(4095, stat);
      EEPROM.commit();
      EEPROM.end();
    }
    else
    {
      Serial.println("[ULProtection] ERROR: UPDATE LOOP DETECTED!!!");
      Serial.println("[ULProtection] Disabling updates for the next 24 hours!");
      updateLock = true;
      lockPassed = false;
      prohibitUpdateMillis = millis();
      dataTransmission("ERROR:%20UPDATE%20LOOP%20DETECTED!%20PLEASE%20MATCH%20THE%20VERSION%20IN%20THE%20SOURCECODE%20WITH%20THE%20FILENAME%20VERSION%20AND%20PUSH%20AN%20UPDATE!");
      EEPROM.begin(4096);
      EEPROM.write(4095, 1);
      EEPROM.commit();
      EEPROM.end();
      return "UPDATE_LOOP_DETECTED";
    }
  }
  else
  {
    Serial.println("[ULProtection] Done! No Downgrade or Update Loop detected!");
    Serial.println("[ULProtection] Continuing with update...");
    EEPROM.begin(4096);
    EEPROM.write(4095, 0);
    EEPROM.commit();
    EEPROM.end();
  }

  t_httpUpdate_return ret;
  while (true)
  {
    ESPhttpUpdate.onStart([this]()
                          { updateStarted(); });
    ESPhttpUpdate.onEnd([this]()
                        { updateFinished(); });
    ESPhttpUpdate.onProgress([this](int cur, int total)
                             { updateProgress(cur, total); });
    ESPhttpUpdate.onError([this](int err)
                          { updateError(err); });
    if (sslState == true)
    {
      ret = ESPhttpUpdate.update(secureupdatewificlient, binLink);
    }
    else
    {
      ret = ESPhttpUpdate.update(updatewificlient, binLink);
    }
    switch (ret)
    {
    case HTTP_UPDATE_FAILED:
      Serial.print("[Update] HTTP_UPDATE_FAILED Error ");
      Serial.printf("(%d): %s\r", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
      return "HTTP_ERROR";
      break;

    case HTTP_UPDATE_NO_UPDATES:
      return "HTTP_UPDATE_NO_UPDATES";
      break;

    case HTTP_UPDATE_OK:
      return "HTTP_UPDATE_OK";
      break;
    }
    // delay(10); // DDOS Protection maybe slowing update down?
  }
}

void R_Lib8266::setSSLRootCertificate(char certificate[])
{
  strcpy(rootCACertificate, certificate);
}

void R_Lib8266::setSSLInsecureState(bool sinsecure)
{
  insecureState = sinsecure;
}

bool R_Lib8266::getSSLInsecureState()
{
  return insecureState;
}

String R_Lib8266::dataTransmission(String status)
{

  String postData = dlLink + "datareceive.php?mac=" + WiFi.macAddress() + "&devicename=" + deviceName + "&fwver=" + version + "&status=" + status;
  if (sslState == true)
  {
    if (insecureState == true)
    {
      secureupdatewificlient.setInsecure();
      Serial.println("[Secure Update] WARNING! Insecure mode activated! Certificate will not be checked!");
    }
    http.begin(secureupdatewificlient, postData);
  }
  else
  {
    http.begin(updatewificlient, postData);
  }
  delay(10);
  int httpCode = http.GET();
  if (httpCode == 200)
  {
    Serial.println("[Update] UserData successfully transmitted!");
    return "SUCCESS";
  }
  else
  {
    Serial.println("[Update] Error transmitting UserData: " + httpCode);
    return "ERROR";
  }
}

void R_Lib8266::setAttemptsBeforeInsecureSSL(int sattempts)
{
  attemptsBeforeInsecureSSL = sattempts;
}

int R_Lib8266::getAttemptsBeforeInsecureSSL()
{
  return attemptsBeforeInsecureSSL;
}

void R_Lib8266::executeAttemptsBeforeInsecureSSL()
{
  if (attemptsBeforeInsecureSSL > 0)
  {
    Serial.println("[SSL Setup] Insecure SSL after X Attempts enabled!");
    EEPROM.begin(4096);
    Serial.println("[SSL Setup] Loading Insecure SSL after X Attempts value...");
    int output = EEPROM.read(4084);
    Serial.println(output);
    if (output > 9 || output < 0)
    {
      output = 0;
    }
    output++;
    if (output > attemptsBeforeInsecureSSL)
    {
      Serial.println("[SSL Setup] Max Attempts reached! Enabling Insecure SSL...");
      setSSLInsecureState(true);
      EEPROM.write(4084, 0);
    }
    else
    {
      Serial.println("[SSL Setup] Max Attempts not reached. Continuing with Secure SSL...");
      EEPROM.write(4084, output);
    }
    EEPROM.commit();
    EEPROM.end();
  }
  else
  {
    Serial.println("[SSL Setup] Insecure SSL after X Attempts disabled. Continuing with normal SSL operation...");
  }
}