#include "handleHTTPClient.h"
#include "base64.h"

long lastSendMillis = millis();                // part of the period for sending data to the target server


/* send data to target server using ESP8266HTTPClient */
void handleHTTPClient(asyncHTTPrequest aRequest, WiFiClient wifiClient, Settings * pSettings, String macAddress, uint32_t revolutions, uint32_t viewPulsesPerMinute)
  {
    long currentMillis = millis();

    // send data to the target server wich should show an openstreetmap
    if (currentMillis - lastSendMillis > pSettings->getSEND_PERIOD())
    {
      sendDataToTarget(aRequest, wifiClient, pSettings, macAddress,  revolutions, viewPulsesPerMinute);
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
  result += "\"firmwareVersion\":";
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

void sendDataToTarget(asyncHTTPrequest aRequest, WiFiClient wifiClient, Settings * pSettings, String macAddress, uint32_t revolutions, uint32_t viewPulsesPerMinute)
{
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


  if (aRequest.readyState() == 0 || aRequest.readyState() == 4)
  {
    aRequest.open("POST", url.c_str());
    aRequest.setReqHeader("Content-Type", "application/json");
    aRequest.setReqHeader("Cache-Control", "no-cache");
    aRequest.setReqHeader("Connection", "keep-alive");
    aRequest.setReqHeader("Pragma", "no-cache");
    aRequest.setReqHeader("Authorization", auth.c_str());
    aRequest.send(post_data);
    delay(100);  //prevents a reboot
  }
}
