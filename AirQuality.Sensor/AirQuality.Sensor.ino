#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include "wifi_connect.h"
#include "certs.h"

#include <DHT.h>

#include <ArduinoJson.h>

bool wifi_connect = false;
HTTPClient https;
// Отпечаток сертификат SSL сайта iocontrol.ru, так же его можно получить из браузера,
// нажав на пиктограмму замка рядом с полем ввода адреса
const uint8_t fingerprint[20] = {0xcc, 0x64, 0x74, 0xd1, 0x51, 0x71, 0x56, 0xc6, 0x66, 0xee, 0x53, 0x49, 0x20, 0xcf, 0x37, 0xb1, 0xe6, 0xb6, 0x3a, 0x19};

#define DHTPIN 2
DHT dht(DHTPIN, DHT11); //Инициация датчика

const String serverPath = "https://192.168.50.170:4430/api/wsd/add";

// интервал в миллисекундах (5 минут)
const unsigned long interval = 10000;
unsigned long previousMillis = 0;

void setup() {
  Serial.begin(115200);

  wifi_connect = WiFiConnect();

  dht.begin();
}

void loop() {

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {

    previousMillis = currentMillis;

    if(wifi_connect == true){
      std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
      // Ignore SSL certificate validation
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
      int httpResponseCode = https.POST(jsonToPost);
          
      if (httpResponseCode > 0) {
        const String payload = https.getString();
        printHttpsResponseCode(httpResponseCode, payload);
      }
      else {
        printHttpsError(httpResponseCode);
      }
          
      https.end();
    }else{
      Serial.print("---WiFi error---");
    }
  }
}

void printHttpsResponseCode(int httpResponseCode, String payload){
  Serial.print("---HTTPS Response code: ");
  Serial.print(httpResponseCode);
  Serial.print(payload);
  Serial.print("---");
}

void printHttpsError(int httpResponseCode){
  Serial.print("---HTTPS ERROR START---");
  Serial.print("Error code: ");
  Serial.println(httpResponseCode);
  Serial.printf("[HTTPS] POST... failed, error: %s\n", https.errorToString(httpResponseCode).c_str());
  Serial.print("---HTTPS ERROR END---");
}
