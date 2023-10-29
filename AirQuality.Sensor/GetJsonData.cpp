#include "GetJsonData.h"
#include <ArduinoJson.h>

String GetJsonData(float t, float h,float pm_1,float pm2_5,float pm_10,int co){
  String jsonToPost;
  StaticJsonDocument<100> jsonDocument;

  jsonDocument["temperature"] = t;
  jsonDocument["humidity"] = h;
  jsonDocument["pm_1"] = pm_1;
  jsonDocument["pm2_5"] = pm2_5;
  jsonDocument["pm_10"] = pm_10;
  jsonDocument["co"] = co;
  serializeJsonPretty(jsonDocument, jsonToPost);

  Serial.println("---JSON START---");
  Serial.print(jsonToPost);
  Serial.println("---JSON END---");

  return jsonToPost;
}