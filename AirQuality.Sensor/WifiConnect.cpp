#include "WifiConnect.h"
#include <WiFiManager.h>

bool WiFiConnect(){
  WiFiManager wm;

  bool connectResult;

  connectResult = wm.autoConnect("AirQualitySensor","0000");

  if(!connectResult) {
    Serial.println("---Failed to connect---");
    // ESP.restart();
  } 
  else {    
    Serial.println("---WiFi connected---");
  }

  return connectResult;
}