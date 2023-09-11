#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <SimplePortal.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include "DHT.h"

ESP8266WiFiMulti WiFiMulti;
#define DHTPIN 2
DHT dht(DHTPIN, DHT11); //Инициация датчика
String serverName = "http://192.168.50.170:8080/Home/Post";

void setup() {
  Serial.begin(9600);
  
    WiFi.mode(WIFI_STA);
    WiFiMulti.addAP("-", "-");
    dht.begin();
}

void loop() {
  if(WiFiMulti.run() == WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;
        
        float h = dht.readHumidity(); //Измеряем влажность
        float t = dht.readTemperature(); //Измеряем температуру
        if (isnan(h) || isnan(t)) {  // Проверка. Если не удается считать показания, выводится «Ошибка считывания», и программа завершает работу
          Serial.println("Ошибка считывания");
          return;
        } 
        String serverPath = serverName + "?T=" + t + "&H=" + h;
        Serial.println(serverPath);
        
        // Your Domain name with URL path or IP address with path
        http.begin(client, serverPath.c_str());
          
        // Send HTTP GET request
        int httpResponseCode = http.GET();
        
        if (httpResponseCode>0) {
          Serial.print("HTTP Response code: ");
          Serial.println(httpResponseCode);
          String payload = http.getString();
          Serial.println(payload);
        }
        else {
          Serial.print("Error code: ");
          Serial.println(httpResponseCode);
          Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpResponseCode).c_str());
        }
        // Free resources
        http.end();
  }else{
    Serial.print("ошибка с вифи");
  }
  // Отправка данных раз в 10 секунд
  delay(60000UL);
}
