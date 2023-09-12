#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include "wifi_connect.h"

#include <DHT.h>

#include <ArduinoJson.h>

bool wifi_connect = false;
HTTPClient https;
const uint8_t fingerprint[20] = {0xcc, 0x64, 0x74, 0xd1, 0x51, 0x71, 0x56, 0xc6, 0x66, 0xee, 0x53, 0x49, 0x20, 0xcf, 0x37, 0xb1, 0xe6, 0xb6, 0x3a, 0x19};

#define DHTPIN 2
DHT dht(DHTPIN, DHT11); 

const String serverPath = "https://192.168.50.170:4430/api/wsd/add";

void setup() {
  Serial.begin(115200);

  wifi_connect = WiFiConnect();

  dht.begin();
}

void loop() {
    if(wifi_connect == true){
      std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
      client->setFingerprint(fingerprint);

      float t = dht.readTemperature(); 
      float h = dht.readHumidity(); 
      if (isnan(h) || isnan(t)) {  
        Serial.println("---DHT Error Read---");
        return;
      }

      StaticJsonDocument<100> jsonDocument;

      jsonDocument["temperature"] = t;
      jsonDocument["humidity"] = h;

      char jsonToPost[100];
      serializeJsonPretty(jsonDocument, jsonToPost);
      Serial.print("---JSON START---");
      Serial.print(jsonToPost);
      Serial.println("---JSON END---");
          
      https.begin(*client, serverPath.c_str());
      https.addHeader("Content-Type", "application/json");
      int httpsResponseCode = https.POST(jsonToPost);
          
      if (httpsResponseCode > 0) {
        const String payload = https.getString();
        printHttpsResponseCode(httpsResponseCode, payload);
      }
      else {
        printHttpsError(httpsResponseCode);
      }
          
      https.end();
    }else{
      Serial.print("---WiFi error---");
    }

    delay(300000);
}

void printHttpsResponseCode(int httpsResponseCode, String payload){
  Serial.print("---HTTPS Response code: ");
  Serial.print(httpsResponseCode);
  Serial.print(payload);
  Serial.print("---");
}

void printHttpsError(int httpsResponseCode){
  Serial.print("---HTTPS ERROR START---");
  Serial.print("Error code: ");
  Serial.println(httpsResponseCode);
  Serial.printf("[HTTPS] POST... failed, error: %s\n", https.errorToString(httpsResponseCode).c_str());
  Serial.print("---HTTPS ERROR END---");
}
