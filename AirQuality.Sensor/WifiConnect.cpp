#include "WifiConnect.h"
#include <WiFiManager.h>

bool WiFiConnect(){
  WiFiManager wm;

  bool connectResult;

  connectResult = wm.autoConnect("AirQualitySensor","12345678");

  if(!connectResult) {
    wm.resetSettings();
    ESP.restart(); 
  } 

  return connectResult;
}