#include "HttpsRequestToServer.h"
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>

HTTPClient https;

void SendData(String jsonToPost){
  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
  client->setFingerprint(fingerprint);
          
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