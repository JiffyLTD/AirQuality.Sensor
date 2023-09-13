#include "GetJsonData.h"
#include <ArduinoJson.h>

String GetJsonData(float t, float h){
  String jsonToPost;
  StaticJsonDocument<100> jsonDocument;

  jsonDocument["temperature"] = t;
  jsonDocument["humidity"] = h;
  serializeJsonPretty(jsonDocument, jsonToPost);

  Serial.print("---JSON START---");
  Serial.print(jsonToPost);
  Serial.println("---JSON END---");

  return jsonToPost;
}