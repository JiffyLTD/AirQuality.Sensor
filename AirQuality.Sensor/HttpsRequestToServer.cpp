#include "HttpsRequestToServer.h"
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>

HTTPClient https;

void SendData(String jsonToPost){
  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
  client->setInsecure();

  if(https.begin(*client, serverPath.c_str())){
    https.addHeader(F("Content-Type"), F("application/json"));
    https.addHeader(F("API-Key"), token);

    https.POST(jsonToPost);        
            
    https.end();
  }        
}
