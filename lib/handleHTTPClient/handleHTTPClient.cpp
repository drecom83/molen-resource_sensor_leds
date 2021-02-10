#include "handleHTTPClient.h"
#include "base64.h"

long lastSendMillis = millis();                // part of the period for sending data to the target server


/* send data to target server using ESP8266HTTPClient */
void handleHTTPClient(asyncHTTPrequest* pRequest, WiFiClient wifiClient, Settings * pSettings, String macAddress, uint32_t revolutions, uint32_t viewPulsesPerMinute)
  {
    long currentMillis = millis();

    // send data to the target server wich should show an openstreetmap
    if (currentMillis - lastSendMillis > pSettings->getSEND_PERIOD())
    {
      sendDataToTarget(pRequest, wifiClient, pSettings, macAddress,  revolutions, viewPulsesPerMinute);
      lastSendMillis = currentMillis;
    }
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
  result += "\"version\":";
  result += "\"";
  result += pSettings->getFirmwareVersion();
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

void sendDataToTarget(asyncHTTPrequest* pRequest, WiFiClient wifiClient, Settings * pSettings, String macAddress, uint32_t revolutions, uint32_t viewPulsesPerMinute)
{
  //String targetServer = "10.0.0.51";
  //uint16_t port = 8085;
  //String path = "/";
  String targetServer = pSettings->getTargetServer();
  uint16_t port =  pSettings->getTargetPort();
  String path =  pSettings->getTargetPath();
  String url = targetServer + ":" + String(port) + path;
 
  // Note: BasicAuthentication does not allow any colon characters
  //       replace them with an underscore
  String key = macAddress;
  key.replace(":", "_");
  // Note: String end with \n character that has to be removed in the header
  String auth = "Basic " + base64::encode(key + ":" + pSettings->getDeviceKey());
  auth.replace("\n","");

  String post_data = getSendData(pSettings, macAddress, revolutions, viewPulsesPerMinute);


  if (pRequest->readyState() == 0 || pRequest->readyState() == 4)
  {
    pRequest->open("POST", url.c_str());
    pRequest->setReqHeader("Content-Type", "application/json");
    pRequest->setReqHeader("Cache-Control", "no-cache");
    pRequest->setReqHeader("Connection", "keep-alive");
    pRequest->setReqHeader("Pragma", "no-cache");
    pRequest->setReqHeader("Authorization", auth.c_str());
    pRequest->send(post_data);
  }
}
/*
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
  // Note: BasicAuthentication does not allow any colon characters
  //       replace them with an underscore
  String key = macAddress;
  key.replace(":", "_");
  // Note: String end with \n character that has to be removed in the header
  String auth = base64::encode(key + ":" + pSettings->getDeviceKey());
  auth.replace("\n","");

  httpClient.begin(wifiClient, url);      //Specify request destination
  //httpClient.begin(wifiClient, "http://10.0.0.10:9090/feed/");      //Specify request destination

  httpClient.addHeader("Content-Type", "application/json");  //Specify content-type header
  httpClient.addHeader("Cache-Control", "no-cache");
  httpClient.addHeader("Connection", "keep-alive");
  httpClient.addHeader("Pragma", "no-cache");
  httpClient.addHeader("Authorization", "Basic " + auth);

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
    Serial.println(httpCode);
    response = HANDLEHTTPCLIENT_FAILED;
  }
  httpClient.end();  //Close connection
  return response;
}
// end client
*/