#include "WifiConnect.h"
#include "GetJsonData.h"
#include "HttpsRequestToServer.h"
#include "SoftwareSerial.h"
#include "DHT.h"
#include <TinyGPSPlus.h>
#include "PMS.h"
#include <Adafruit_BMP085.h>
#include <TinyGPSPlus.h>

//DHT22
#define DHTPIN 14   
#define DHTTYPE DHT22   
DHT dht(DHTPIN, DHTTYPE);

// PMS7003
#define TX_PIN_PMS7003 3
#define RX_PIN_PMS7003 2
SoftwareSerial pmsSerial(RX_PIN_PMS7003, TX_PIN_PMS7003);// RX, TX
PMS pms(pmsSerial);
PMS::DATA data;

// MQ-7
int analogMQ7 = A0; 

// BMP180
#define SCL_PIN_BMP180 12
#define SDA_PIN_BMP180 13

Adafruit_BMP085 bmp;

// GPS NEO 6M
#define TX_PIN_GPS 4
#define RX_PIN_GPS 5
TinyGPSPlus gps;
SoftwareSerial gpsSerial(RX_PIN_GPS, TX_PIN_GPS);

bool wifiIsConnected = false;

void setup() {
  Serial.begin(115200);
  pmsSerial.begin(9600);
  gpsSerial.begin(9600);

  //BMP180
  Wire.begin(SDA_PIN_BMP180, SCL_PIN_BMP180);
  bmp.begin();
  //DHT 22
  dht.begin();

  wifiIsConnected = WiFiConnect();
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

      float pressure = bmp.readPressure(); 

      float latitude = gps.location.lat();
      float longitude = gps.location.lng();
      float altitude = gps.altitude.meters();

      String jsonToPost = GetJsonData(t, h, pm_1, pm2_5, pm_10, co, pressure, latitude, longitude, altitude);

      if(wifiIsConnected){
        //SendData(jsonToPost);
      } 

      delay(60000);
    }  
}

