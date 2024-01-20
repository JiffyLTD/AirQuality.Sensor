#include "GetJsonData.h"
#include <ArduinoJson.h>

String GetJsonData(float t, float h, float pm_1, float pm2_5, float pm_10, int co, float pressure, String gpsData){
  String jsonToPost;
  StaticJsonDocument<1500> jsonDocument;

  JsonObject createStationDto = jsonDocument.createNestedObject("createStationDto");
  createStationDto["sensorId"] = sensorId;
  createStationDto["location"] = gpsData;

  JsonObject createStationDataDto = jsonDocument.createNestedObject("createStationDataDto");
  createStationDataDto["temperature"] = t;
  createStationDataDto["humidity"] = h;
  createStationDataDto["pm_1"] = pm_1;
  createStationDataDto["pm2_5"] = pm2_5;
  createStationDataDto["pm_10"] = pm_10;
  createStationDataDto["co"] = co;
  createStationDataDto["pressure"] = pressure;

  serializeJsonPretty(jsonDocument, jsonToPost);

  return jsonToPost;
}