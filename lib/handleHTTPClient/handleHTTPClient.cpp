#include "handleHTTPClient.h"

long lastSendMillis = millis();                // part of the period for sending data to the target server


/* send data to target server using ESP8266HTTPClient */
String handleHTTPClient(WiFiClient wifiClient, Settings * pSettings, String macAddress, uint32_t revolutions, uint32_t viewPulsesPerMinute)
  {
    String response = "";
    long currentMillis = millis();

    // send data to the target server wich should show an openstreetmap
    if (currentMillis - lastSendMillis > pSettings->getSEND_PERIOD())
    {
      response = sendDataToTarget(wifiClient, pSettings, macAddress,  revolutions, viewPulsesPerMinute);
      lastSendMillis = currentMillis;
    }
    return response;
  }

// start client to send data to the server (showing data on openstreetmap)
String getSendData(Settings * pSettings, String macAddress, uint32_t revolutions, uint32_t viewPulsesPerMinute) {
  String result = "{";
  result += "\"data\": {";
  result += "\"revolutions\":";
  result += "\"";
  result += revolutions;
  result += "\",";
  result += "\"rawCounter\":";
  result += "\"";
  result += pSettings->getCounter();
  result += "\",";
  result += "\"viewPulsesPerMinute\":";
  result += "\"";
  result += viewPulsesPerMinute;
  result += "\",";
  result += "\"deviceKey\":";
  result += "\"";
  result += pSettings->getDeviceKey();
  result += "\",";
  result += "\"macAddress\":";
  result += "\"";
  result += macAddress;
  result += "\",";
  result += "\"isOpen\":";
  result += "\"";
  result += String(pSettings->getIsOpen());
  result += "\",";
  result += "\"showData\":";
  result += "\"";
  result += String(pSettings->getShowData());
  result += "\",";
  result += "\"message\":";
  result += "\"";
  result += pSettings->getTargetServerMessage();
  result += "\"";
  result += "}";
  result += "}";
  return result;
}

String sendDataToTarget(WiFiClient wifiClient, Settings * pSettings, String macAddress, uint32_t revolutions, uint32_t viewPulsesPerMinute)
{
  HTTPClient httpClient;    //Declare object of class HTTPClient
  //String targetServer = "10.0.0.51";
  //uint16_t port = 8085;
  //String path = "/";
  String targetServer = pSettings->getTargetServer();
  uint16_t port =  pSettings->getTargetPort();
  String path =  pSettings->getTargetPath();
  String url = targetServer + ":" + String(port) + path;
  //httpClient.begin("http://192.168.1.88:8085/hello");      //Specify request destination
  httpClient.begin(wifiClient, url);      //Specify request destination
  //httpClient.begin(wifiClient, "http://10.0.0.10:9090/feed/");      //Specify request destination

  httpClient.addHeader("Content-Type", "application/json");  //Specify content-type header
  httpClient.addHeader("Cache-Control", "no-cache");
  httpClient.addHeader("Connection", "keep-alive");
  httpClient.addHeader("Pragma", "no-cache");

  String post = getSendData(pSettings, macAddress, revolutions, viewPulsesPerMinute);
  //httpClient.POST(post);   //Send the request
  int httpCode = httpClient.POST(post);   //Send the request
  //Serial.println(httpCode);
  String response = "";
  if (httpCode == 200)
  {
    response = httpClient.getString();    //Get the response payload
  }
  else {
    // something is wrong
    response = HANDLEHTTPCLIENT_FAILED;
  }
  httpClient.end();  //Close connection
  return response;
}
// end client