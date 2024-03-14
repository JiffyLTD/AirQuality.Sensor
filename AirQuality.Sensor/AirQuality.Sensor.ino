#include "WifiConnect.h"
#include "GetJsonData.h"
#include "HttpsRequestToServer.h"
#include "SoftwareSerial.h"
#include "DHT.h"
#include "PMS.h"
#include <Adafruit_BMP085.h>

// DHT22
#define DHTPIN 5 // D1                         
#define DHTTYPE DHT22   
DHT dht(DHTPIN, DHTTYPE);

// PMS7003
#define TX_PIN_PMS7003 14 // D6
#define RX_PIN_PMS7003 12 // D5
SoftwareSerial pmsSerial(RX_PIN_PMS7003, TX_PIN_PMS7003);// RX, TX
PMS pms(pmsSerial);
PMS::DATA data;

// MQ-7
#define ANALOG_MQ7_PIN A0

// BMP180
#define SCL_PIN_BMP180 4 // D2
#define SDA_PIN_BMP180 13 // D7
Adafruit_BMP085 bmp;

// GPS NEO 6M
#define TX_PIN_GPS 3 // D9
#define RX_PIN_GPS 1 // D10
SoftwareSerial gpsSerial(RX_PIN_GPS, TX_PIN_GPS);

bool wifiIsConnected = false;

int32_t timing = 0; // точка отсчета таймера

const int16_t attemptsLimit = 3; // кол-во попыток считывания данных с PMS7003
int16_t currentAttempt = 0;

void setup() 
{
  Serial.begin(115200);
  wifiIsConnected = WiFiConnect();

  pmsSerial.begin(9600);
  gpsSerial.begin(9600);
  dht.begin();

  Wire.begin(SDA_PIN_BMP180, SCL_PIN_BMP180);
  bmp.begin();
}

void loop() 
{
  if (millis() - timing > 600000) 
  {
    currentAttempt = 0;
    int16_t* pmValuesArr = getPMValues();
    while(pmValuesArr[0] == -1 || currentAttempt <= attemptsLimit)
    {
      pmValuesArr = getPMValues();
      currentAttempt += 1;
    }

    int16_t* dhtValuesArr = getDHTValues();
    int16_t coValue = getCOValue();
    int32_t pressureValue = getPressureValue();
    String gpsData = getGPSData();    

    String jsonToPost = GetJsonData(dhtValuesArr[1], dhtValuesArr[0], pmValuesArr[0], pmValuesArr[1], pmValuesArr[2], coValue, pressureValue, gpsData);

    delete[] pmValuesArr;
    delete[] dhtValuesArr;

    if(wifiIsConnected){
      SendData(jsonToPost);
    }

    timing = millis();  
  }
}

int16_t* getPMValues()
{
  int16_t* array = new int16_t[3];

  if(!pms.read(data))
  {
    array[0] = -1;
    array[1] = -1;
    array[2] = -1;

    return array;
  }

  array[0] = data.PM_AE_UG_1_0;
  array[1] = data.PM_AE_UG_2_5;
  array[2] = data.PM_AE_UG_10_0;

  return array;
}

int16_t* getDHTValues()
{
  int16_t* array = new int16_t[2];

  int16_t h = dht.readHumidity();
  int16_t t = dht.readTemperature() * 10;

  if (isnan(h) || isnan(t))
  {
    array[0] = -1;
    array[1] = -273; 

    return array;
  }

  array[0] = h;
  array[1] = t;

  return array;
}

int16_t getCOValue()
{
  int16_t co = analogRead(ANALOG_MQ7_PIN);

  if (co < 0 || co > 1023)
  {
    return -1;
  }

  return co;
}

int32_t getPressureValue()
{
  int32_t pressure = bmp.readPressure();

  if (pressure < 0)
  {
    return -1;
  }

  return pressure;
}

String getGPSData()
{
  if (gpsSerial.available() > 0)
  {
    String line = "";

    while(!line.startsWith(F("$GPGGA"))){
      line = gpsSerial.readStringUntil('\n');
    }

    return line;
  }
  else
  {
    return F("No GPS data");
  }
}







