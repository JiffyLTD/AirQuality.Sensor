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
int analogMQ7 = A0; 

// BMP180
#define SCL_PIN_BMP180 4 // D2
#define SDA_PIN_BMP180 13 // D7
Adafruit_BMP085 bmp;

// GPS NEO 6M
#define TX_PIN_GPS 3 // D9
#define RX_PIN_GPS 1 // D10
SoftwareSerial gpsSerial(RX_PIN_GPS, TX_PIN_GPS);

bool wifiIsConnected = false;

long timing = 0; // точка отсчета таймера

int attemptsLimit = 3; // кол-во попыток считывания данных с PMS7003
int currentAttempt = 0;

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
  if (millis() - timing > 60000) 
  {
    currentAttempt = 0;
    float* pmValuesArr = getPMValues();
    while(pmValuesArr[0] == -1 || currentAttempt <= attemptsLimit)
    {
      pmValuesArr = getPMValues();
      currentAttempt += 1;
    }

    float* dhtValuesArr = getDHTValues();
    int coValue = getCOValue();

    float pressureValue = getPressureValue();

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

float* getPMValues()
{
  try
  {
    if(!pms.read(data))
    {
      throw "No pms data";
    }
    
    float pm_1 = data.PM_AE_UG_1_0;
    float pm2_5 = data.PM_AE_UG_2_5;
    float pm_10 = data.PM_AE_UG_10_0;

    float* array = new float[3];

    array[0] = pm_1;
    array[1] = pm2_5;
    array[2] = pm_10;

    return array;
  }
  catch (...)
  {
    float* array = new float[3];

      array[0] = -1;
      array[1] = -1;
      array[2] = -1;

      return array;
  }
}

float* getDHTValues()
{
    try
    {
      float h = dht.readHumidity();
      float t = dht.readTemperature();

      if (isnan(h) || isnan(t))
      {
        throw "No DHT data";
      }

      float* array = new float[2];

      array[0] = h;

      if(t > 0)
        array[1] = t;
      else
        array[1] = (t + 3276.6) * -1; // из за кривой библиотеки приходится высчитывать отрицательные значения

      return array;
    }
    catch (...)
    {
      float* array = new float[2];

      array[0] = -1;
      array[1] = -273; 

      return array;
    }
}

int getCOValue()
{
    try
    {
      int co = analogRead(analogMQ7);

      if (co < 0 || co > 1023)
      {
        throw "No MQ-7 data";
      }

      return co;
    }
    catch (...)
    {
      return -1;
    }
}

float getPressureValue()
{
    try
    {
      float pressure = bmp.readPressure();

      if (pressure < 0)
      {
        throw "No BMP180 data";
      }

      return pressure;
    }
    catch (...)
    {
      return -1;
    }
}

String getGPSData()
{
    try
    {
      if (gpsSerial.available() > 0)
      {
        String gpsData = "";
        String line = "";

        while(!line.startsWith("$GPGGA")){
          line = gpsSerial.readStringUntil('\n');
        }

        gpsData += line;

        return gpsData;
      }
      else
      {
        throw "No GPS data";
      }
    }
    catch (...)
    {
      return "Invalid";
    }
}







