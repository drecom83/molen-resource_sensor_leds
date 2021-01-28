#ifndef HANDLEHTTPCLIENT_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define HANDLEHTTPCLIENT_H
#include <ESP8266WiFi.h>          //ESP8266 Core WiFi Library (you most likely already have this in your sketch)
#include <ESP8266HTTPClient.h> 
#include "settings.h"

const String HANDLEHTTPCLIENT_FAILED = "Handling client failed";

/* send data to target server using ESP8266HTTPClient */
String handleHTTPClient(WiFiClient wifiClient, Settings * pSettings, String macAddress, uint32_t revolutions, uint32_t viewPulsesPerMinute);
/* contains the html that is send to the targetServer */
String getSendData(Settings * pSettings, String macddress, uint32_t revolutions, uint32_t viewPulsesPerMinute);
/* sends data to the targetServer */
String sendDataToTarget(WiFiClient wifiClient, Settings * pSettings, String macAddress, uint32_t revolutions, uint32_t viewPulsesPerMinute);

#endif