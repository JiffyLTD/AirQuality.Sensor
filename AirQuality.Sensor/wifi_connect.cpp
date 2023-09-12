#include "wifi_connect.h"

#include <WiFiManager.h>

bool WiFiConnect(){
  WiFiManager wm;

  bool connectResult;

  connectResult = wm.autoConnect("AirQualitySensor","1111"); // password protected ap

  if(!connectResult) {
    Serial.println("---Failed to connect---");
    // ESP.restart();
  } 
  else {    
    Serial.println("---WiFi connected---");
  }

  return connectResult;
}