#include <ESP8266WiFi.h>          //ESP8266 Core WiFi Library (you most likely already have this in your sketch)
#include <ArduinoOTA.h>
#include "ota.h"

/*
Example of ota, using an IP address:

In platformio.ini:

[env:esp12e_over_IP]
platform = espressif8266
framework = arduino
board = d1_mini
upload_protocol = espota
upload_port = <ip address of the esp8266>

upload_flags = --auth=<ota password>
monitor_speed = 115200
*/

void setupArduinoOTA () {
  // Port defaults to 8266
  ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname("<host name>");

  // No authentication by default
  ArduinoOTA.setPassword((const char *)"<ota password>");

  ArduinoOTA.onStart([]() {
    Serial.println("Start");

  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("\nReady via air!");
  
  if (WiFi.getMode() == SOFTAP_MODE) {
    Serial.print("SoftAP SSID: ");
    Serial.println(WiFi.softAPSSID());
    Serial.print("SoftAP IP: ");
    Serial.println(WiFi.softAPIP());
    Serial.print("SoftAP Mac Address: ");
    Serial.println(WiFi.softAPmacAddress());
  }
  if (WiFi.getMode() == STATION_MODE) {
    Serial.print("Connecting");
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print(".");
    }
    Serial.println();
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("Mac Address: ");
    Serial.println(WiFi.macAddress());
  }
}

void otaHandle() 
{
      ArduinoOTA.handle();
}