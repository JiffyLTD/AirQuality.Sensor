#include "WifiConnect.h"
#include "GetJsonData.h"
#include "HttpsRequestToServer.h"

//DHT22
#include "DHT.h"
#define DHTPIN 14   
#define DHTTYPE DHT22   
DHT dht(DHTPIN, DHTTYPE);

// PMS7003
#include "PMS.h"
#include "SoftwareSerial.h"
SoftwareSerial pmsSerial(2, 3);// RX, TX
PMS pms(pmsSerial);
PMS::DATA data;

int analogMQ7 = A0; // MQ-7

bool wifiIsConnected = false;

void setup() {
  Serial.begin(115200);
  pmsSerial.begin(9600);

  wifiIsConnected = WiFiConnect();

  dht.begin();
}

void loop() {
 if(pms.read(data))
    {
      float pm_1 = data.PM_AE_UG_1_0;
      float pm2_5 = data.PM_AE_UG_2_5;
      float pm_10 = data.PM_AE_UG_10_0;

      float h = dht.readHumidity();
      float t = dht.readTemperature();

      int co = analogRead(analogMQ7); 

      if(wifiIsConnected){
        String jsonToPost = GetJsonData(t, h, pm_1, pm2_5, pm_10, co);

        SendData(jsonToPost);
      } 

      delay(60000);
    }  
}

