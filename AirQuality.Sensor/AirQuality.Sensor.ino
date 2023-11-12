#include "WifiConnect.h"
#include "GetJsonData.h"
#include "HttpsRequestToServer.h"
#include "SoftwareSerial.h"
#include "DHT.h"
#include "PMS.h"
#include <Adafruit_BMP085.h>

//DHT22
#define DHTPIN 5 //D1  
#define DHTTYPE DHT22   
DHT dht(DHTPIN, DHTTYPE);

// PMS7003
#define TX_PIN_PMS7003 14 //D5
#define RX_PIN_PMS7003 12 //D6
SoftwareSerial pmsSerial(RX_PIN_PMS7003, TX_PIN_PMS7003);// RX, TX
PMS pms(pmsSerial);
PMS::DATA data;

// MQ-7
int analogMQ7 = A0; 

// BMP180
#define SCL_PIN_BMP180 4 //D2
#define SDA_PIN_BMP180 13 //D7
Adafruit_BMP085 bmp;

// GPS NEO 6M
#define TX_PIN_GPS 3 //D9
#define RX_PIN_GPS 1 //D10
SoftwareSerial gpsSerial(RX_PIN_GPS, TX_PIN_GPS);

bool wifiIsConnected = false;

unsigned long timing; // Переменная для хранения точки отсчета

void setup() {
  wifiIsConnected = WiFiConnect();
  pmsSerial.begin(9600);
  gpsSerial.begin(9600);

  //BMP180
  Wire.begin(SDA_PIN_BMP180, SCL_PIN_BMP180);
  bmp.begin();
  //DHT 22
  dht.begin();
}

void loop() {
  if(pms.read(data))
  {
    if (millis() - timing > 60000) 
    {
      float pm_1 = data.PM_AE_UG_1_0;
      float pm2_5 = data.PM_AE_UG_2_5;
      float pm_10 = data.PM_AE_UG_10_0;

      float h = dht.readHumidity();
      float t = dht.readTemperature();

      int co = analogRead(analogMQ7);

      float pressure = bmp.readPressure();

      String gpsData = "Invalid"; 

      if (gpsSerial.available() > 0)
      {
        String data = "";

        while(!data.startsWith("$GPGGA")){
          data = gpsSerial.readStringUntil('\n');
        }

        gpsData = data; 
      }      

      String jsonToPost = GetJsonData(t, h, pm_1, pm2_5, pm_10, co, pressure, gpsData);

      if(wifiIsConnected){
        SendData(jsonToPost);
      }

      timing = millis();  
    }
  } 
}



