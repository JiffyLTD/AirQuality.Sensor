#include "GetJsonData.h"
#include <ArduinoJson.h>

String GetJsonData(float t, float h, float pm_1, float pm2_5, float pm_10, int co, float pressure, String gpsData){
  String jsonToPost;
  StaticJsonDocument<3000> jsonDocument;

  jsonDocument["temperature"] = t;
  jsonDocument["humidity"] = h;
  jsonDocument["pm_1"] = pm_1;
  jsonDocument["pm2_5"] = pm2_5;
  jsonDocument["pm_10"] = pm_10;
  jsonDocument["co"] = co;
  jsonDocument["pressure"] = pressure;
  jsonDocument["gpsData"] = gpsData;

  serializeJsonPretty(jsonDocument, jsonToPost);

  return jsonToPost;
}