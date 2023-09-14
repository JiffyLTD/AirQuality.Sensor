#include "WifiConnect.h"
#include "GetJsonData.h"
#include "HttpsRequestToServer.h"
#include <DHT.h>

bool wifiIsConnected = false;


#define DHTPIN 2
DHT dht(DHTPIN, DHT11); 


void setup() {
  Serial.begin(115200);

  wifiIsConnected = WiFiConnect();

  dht.begin();
}

void loop() {
    if(wifiIsConnected == true){
      float t = dht.readTemperature(); 
      float h = dht.readHumidity(); 
      if (isnan(h) || isnan(t)) {  
        Serial.println("---DHT Error Read---");
        return;
      }

      String jsonToPost = GetJsonData(t, h);

      SendData(jsonToPost);
    }else{
      Serial.print("---WiFi error---");
    }

    // задержка на 20 секунд
    delay(20000);
}

