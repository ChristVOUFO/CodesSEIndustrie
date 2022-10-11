#include <WiFi.h>
#include <HTTPClient.h>

//Prototypes
void sendData(String params);

const char *ssid = "Christ";
const char *password = "1234ABCD";
String GOOGLE_SCRIPT_ID = "AKfycbxmjdhMoLe8uuvtq-Nn3YN3jO1cwUvJTdQTOSiXRaRgWiF-xWF9UgAuFl9J4rFffKHumA";

const char * root_ca=\
"-----BEGIN CERTIFICATE-----\n" \
"MIIL0jCCCrqgAwIBAgIQWTYQvT6JzXw6NnPkNWEjtDANBgkqhkiG9w0BAQsFADBI\n" \
"MRswGQYDVQQDExJFU0VUIFNTTCBGaWx0ZXIgQ0ExHDAaBgNVBAoTE0VTRVQsIHNw\n" \
"b2wuIHMgci4gby4xCzAJBgNVBAYTAlNLMB4XDTIxMDkxMzAxMzgzN1oXDTIxMTEy\n" \
"MDAxMzgzNlowFzEVMBMGA1UEAwwMKi5nb29nbGUuY29tMFkwEwYHKoZIzj0CAQYI\n" \
"KoZIzj0DAQcDQgAEV6U1GezZC8KUMWCG5O2n/35M3YR/5lLT8lDfxzn1qf48Ny5D\n" \
"L1PSTPdwMccqof74AqST8/g6dTyyBJMs+WvZ36OCCbIwggmuMBMGA1UdJQQMMAoG\n" \
"CCsGAQUFBwMBMAwGA1UdEwEB/wQCMAAwgglWBgNVHREEgglNMIIJSYIMKi5nb29n\n" \
"bGUuY29tghYqLmFwcGVuZ2luZS5nb29nbGUuY29tggkqLmJkbi5kZXaCEiouY2xv\n" \
"dWQuZ29vZ2xlLmNvbYIYKi5jcm93ZHNvdXJjZS5nb29nbGUuY29tghgqLmRhdGFj\n" \
"b21wdXRlLmdvb2dsZS5jb22CCyouZ29vZ2xlLmNhggsqLmdvb2dsZS5jbIIOKi5n\n" \
"b29nbGUuY28uaW6CDiouZ29vZ2xlLmNvLmpwgg4qLmdvb2dsZS5jby51a4IPKi5n\n" \
"b29nbGUuY29tLmFygg8qLmdvb2dsZS5jb20uYXWCDyouZ29vZ2xlLmNvbS5icoIP\n" \
"Ki5nb29nbGUuY29tLmNvgg8qLmdvb2dsZS5jb20ubXiCDyouZ29vZ2xlLmNvbS50\n" \
"coIPKi5nb29nbGUuY29tLnZuggsqLmdvb2dsZS5kZYILKi5nb29nbGUuZXOCCyou\n" \
"Z29vZ2xlLmZyggsqLmdvb2dsZS5odYILKi5nb29nbGUuaXSCCyouZ29vZ2xlLm5s\n" \
"ggsqLmdvb2dsZS5wbIILKi5nb29nbGUucHSCEiouZ29vZ2xlYWRhcGlzLmNvbYIP\n" \
"Ki5nb29nbGVhcGlzLmNughEqLmdvb2dsZXZpZGVvLmNvbYIMKi5nc3RhdGljLmNu\n" \
"ghAqLmdzdGF0aWMtY24uY29tghIqLmdzdGF0aWNjbmFwcHMuY26CD2dvb2dsZWNu\n" \
"YXBwcy5jboIRKi5nb29nbGVjbmFwcHMuY26CEWdvb2dsZWFwcHMtY24uY29tghMq\n" \
"Lmdvb2dsZWFwcHMtY24uY29tggxna2VjbmFwcHMuY26CDiouZ2tlY25hcHBzLmNu\n" \
"ghJnb29nbGVkb3dubG9hZHMuY26CFCouZ29vZ2xlZG93bmxvYWRzLmNughByZWNh\n" \
"cHRjaGEubmV0LmNughIqLnJlY2FwdGNoYS5uZXQuY26CC3dpZGV2aW5lLmNugg0q\n" \
"LndpZGV2aW5lLmNughFhbXBwcm9qZWN0Lm9yZy5jboITKi5hbXBwcm9qZWN0Lm9y\n" \
"Zy5jboIRYW1wcHJvamVjdC5uZXQuY26CEyouYW1wcHJvamVjdC5uZXQuY26CF2dv\n" \
"b2dsZS1hbmFseXRpY3MtY24uY29tghkqLmdvb2dsZS1hbmFseXRpY3MtY24uY29t\n" \
"Y29tggthbmRyb2lkLmNvbYINKi5hbmRyb2lkLmNvbYITKi5mbGFzaC5hbmRyb2lk\n" \
"LmNvbYIEZy5jboIGKi5nLmNuggRnLmNvggYqLmcuY2+CBmdvby5nbIIKd3d3Lmdv\n" \
"by5nbIIUZ29vZ2xlLWFuYWx5dGljcy5jb22CFiouZ29vZ2xlLWFuYWx5dGljcy5j\n" \
"b22CCmdvb2dsZS5jb22CEmdvb2dsZWNvbW1lcmNlLmNvbYIUKi5nb29nbGVjb21t\n" \
"ZXJjZS5jb22CCGdncGh0LmNuggoqLmdncGh0LmNuggp1cmNoaW4uY29tggwqLnVy\n" \
"Y2hpbi5jb22CCHlvdXR1LmJlggt5b3V0dWJlLmNvbYINKi55b3V0dWJlLmNvbYIU\n" \
"eW91dHViZWVkdWNhdGlvbi5jb22CFioueW91dHViZWVkdWNhdGlvbi5jb22CD3lv\n" \
"dXR1YmVraWRzLmNvbYIRKi55b3V0dWJla2lkcy5jb22CBXl0LmJlggcqLnl0LmJl\n" \
"ghphbmRyb2lkLmNsaWVudHMuZ29vZ2xlLmNvbYIbZGV2ZWxvcGVyLmFuZHJvaWQu\n" \
"Z29vZ2xlLmNughxkZXZlbG9wZXJzLmFuZHJvaWQuZ29vZ2xlLmNughhzb3VyY2Uu\n" \
"YW5kcm9pZC5nb29nbGUuY24wDgYDVR0PAQH/BAQDAgOIMB8GA1UdIwQYMBaAFAEr\n" \
"TGk3C7ZnPeYdBxHbG/W4dvVSMA0GCSqGSIb3DQEBCwUAA4IBAQCnfsrKunIZZZvr\n" \
"uNKQ9AcTzpNEMOcn7ZhPCvIBBHIHqYT8Z8Iwv4BU0Iw8y1Pel2PJ66Hn/R1x1LvF\n" \
"+x9uVPw6/ss/LvYxuSnuBBG1pJY7NBM8sAD3qutmAq43PsXkfIK3QJ4gCIcTI6y4\n" \
"+TUDzinLloO8jSB7P79QQxGuFmP7MJ4Y+k2jPepc2cX8tdlBOPdi/K0a+pzpcZ7S\n" \
"yrEHWJJqlBu8UHL6Th8AXbI9XjMYbsA+Tmuo/l46WEVevIIr7QgXr/CRD9LQmMX+\n" \
"bkwPSuilbnZwM5hHze4F25Ogj0n58PH0N9DQyGLGbysJdaIEhBSNG+CoPwLxojfq\n" \
"FipLs+20\n" \
"-----END CERTIFICATE-----\n";


WiFiClientSecure client;

void setup() {
  Serial.begin(115200);
  delay(10);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  Serial.println("Started");
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(analogRead(0));
  Serial.println("TP3 GoogleSheets ready...");
}

unsigned long lastTime = 0;
unsigned long timerDelay = 10000;
String strTemp, strHum, strParameter;
void loop() {
  //Send an HTTP POST request every delay
  if ((millis() - lastTime) > timerDelay) {
    strTemp = String(random(100), DEC);
    strHum = String(random(100), DEC);
    Serial.println(strTemp);
    Serial.println(strHum);
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      strParameter = "temperature=" + strTemp + "&humidity=" + strHum;
      sendData(strParameter);
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}

void sendData(String params) {
  
  HTTPClient http;
  String url="https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec?"+params;
  Serial.println(url);
  Serial.println("Making a request");
  // Your Domain name with URL path or IP address with path
  http.begin(url, root_ca); //Specify the URL and certificate
  // Send HTTP GET request
  int httpCode = http.GET();
  if (httpCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpCode);
    String payload = http.getString();
    Serial.println(payload);
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpCode);
  }
  // Free resources
  http.end();
}