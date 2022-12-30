/*#define BLYNK_TEMPLATE_ID "TMPLz7sUh_-y"
#define BLYNK_DEVICE_NAME "TP5"
#define BLYNK_AUTH_TOKEN "PNiTyglWPiT04yHCCSzLQgGDaAvrcEg3"

// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Christ";
char pass[] = "1234ABCD";

// Select your pin with physical button
const int btnPin = 12;

WidgetLED led3(V3);

BlynkTimer timer;

// V3 LED Widget represents the physical button state
boolean btnState = false;
void buttonLedWidget()
{
  // Read button
  boolean isPressed = (digitalRead(btnPin) == LOW);

  // If state has changed...
  if (isPressed != btnState) {
    if (isPressed) {
      led3.on();
    } else {
      led3.off();
    }
    btnState = isPressed;
  }
}

void setup()
{
  // Debug console
  Serial.begin(115200);

  Blynk.begin(auth, ssid, pass);
  Blynk.begin(auth, ssid, pass, IPAddress(192,168,137,10), 8080);

  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Setup physical button pin (active low)
  pinMode(btnPin, INPUT_PULLUP);

  timer.setInterval(500L, buttonLedWidget);
}

void loop()
{
  Blynk.run();
  timer.run();
}*/

/*#define BLYNK_TEMPLATE_ID           "TMPLqvDjP2xJ"
#define BLYNK_DEVICE_NAME           "ESP32"
#define BLYNK_AUTH_TOKEN            "Znp49CU6qG-IS9sWcNyKrHyuFu8DwmT1"

// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Christ";
char pass[] = "1234ABCD";

// Select your pin with physical button
const int btnPin = 13;

WidgetLED led3(V3);

BlynkTimer timer;

// V3 LED Widget represents the physical button state
boolean btnState = false;
void buttonLedWidget()
{
  // Read button
  boolean isPressed = (digitalRead(btnPin) == LOW);

  // If state has changed...
  if (isPressed != btnState) {
    if (isPressed) {
      led3.on();
    } else {
      led3.off();
    }
    btnState = isPressed;
  }
}

void setup()
{
  // Debug console
  Serial.begin(115200);

  Blynk.begin(auth, ssid, pass);
  Blynk.begin(auth, ssid, pass, IPAddress(192,168,137,210), 8080);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Setup physical button pin (active low)
  pinMode(btnPin, INPUT_PULLUP);

  timer.setInterval(500L, buttonLedWidget);
}

void loop()
{
  Blynk.run();
  timer.run();
}*/

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Arduino.h>

#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <Adafruit_Sensor.h>

// Update these with values suitable for your network.
WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];

#define BLYNK_TEMPLATE_ID "TMPL4Gs24xgm"
#define BLYNK_DEVICE_NAME "NodeMCU"
#define BLYNK_AUTH_TOKEN "VpIleLjs4RNBNXWEWMz64bdoyNyypl-s"

int pin = 2;

const char* auth = BLYNK_AUTH_TOKEN;
const char* ssid = "Christ";
const char* pass = "1234ABCD";
const char* mqtt_server = "10.22.1.171";

//Récupérer état du bouton
BLYNK_WRITE(V0){
  digitalWrite(pin, param.asInt());
} 
void setup() {
  pinMode(pin, OUTPUT);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin("Your auth. Token", ssid, pass);
  Serial.begin(115200);
  //setup_wifi();
  client.setServer(mqtt_server, 1883);

}

void loop() {
  client.loop();
  Blynk.run();

    DynamicJsonDocument doc(256);

    doc["BP"] = digitalRead(V0) ? "ACTIF" : "NOT ACTIF";
    doc["LED"] = digitalRead(pin) ? "ON" : "OFF";
  
  char json_string[256];
  serializeJson(doc, json_string);
  client.publish("Blynk", json_string);
}