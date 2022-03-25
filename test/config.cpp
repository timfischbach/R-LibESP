// WARNING! THIS CONFIG FILE IS ESSENTIAL FOR THE LIBARY TO WORK!
#include "config.h"
#include <Arduino.h>
const String DEVICENAME = "TestDevice";
const String VERSION = "v1.0.0a";
const String dllink = "http://dl.timfischbach.com/firmware/testdevice/";
const String devlink = "http://dl.timfischbach.com/dev/";

bool beta = false;

// DEV SETTINGS

bool dev = true;
String DEVTAG = "00000000"; //HEXCODE UPDATE-SERVER 8-DIGIT
