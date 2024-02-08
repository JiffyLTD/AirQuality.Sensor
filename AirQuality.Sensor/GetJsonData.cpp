#include "GetJsonData.h"
#include <ArduinoJson.h>

String GetJsonData(int16_t t, int16_t h, int16_t pm_1, int16_t pm2_5, int16_t pm_10, int16_t co, int16_t pressure, String gpsData){
  String jsonToPost;
  StaticJsonDocument<1500> jsonDocument;

  JsonObject createStationDto = jsonDocument.createNestedObject(F("createStationDto"));
  createStationDto[F("sensorId")] = SensorId;
  createStationDto[F("location")] = gpsData;

  JsonObject createStationDataDto = jsonDocument.createNestedObject(F("createStationDataDto"));
  createStationDataDto[F("temperature")] = t / 10;
  createStationDataDto[F("humidity")] = h;
  createStationDataDto[F("pm_1")] = pm_1;
  createStationDataDto[F("pm2_5")] = pm2_5;
  createStationDataDto[F("pm_10")] = pm_10;
  createStationDataDto[F("co")] = co;
  createStationDataDto[F("pressure")] = pressure;

  serializeJsonPretty(jsonDocument, jsonToPost);

  return jsonToPost;
}