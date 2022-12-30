#include <Arduino.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <SSD1306AsciiWire.h>
#include <WiFi.h>

#define BUTTON 32
#define HEATER 33
#define LED 12
#define RGB_R 25
#define RGB_G 26
#define RGB_B 27
#define SHUTTER 14

const char* ssid = "Wifi_4588";
const char* password = "A1B2C3D4E5F6";

const char* mqtt_user = "admin";
const char* mqtt_passwd = "NZnD1D9c";
const char* mqtt_server = "192.168.12.150";

bool buttonState = false;
bool heaterState = false, shutterState = false;
int ledState = 0;
bool msgReceived = true;

float temperature = 0;
int humidity, consigne = 0;

WiFiClient espClient;
PubSubClient client(espClient);

SSD1306AsciiWire oled;

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str(), mqtt_user, mqtt_passwd)) {
      Serial.println("connected");
      client.subscribe("My Room/ESP32-2/cmd");
      client.subscribe("My Room/ESP32-1/tele");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  DynamicJsonDocument doc(100);
  deserializeJson(doc, payload);
  JsonObject obj = doc.as<JsonObject>();

  const uint8_t r = obj["rgb"][0];
  const uint8_t g = obj["rgb"][1];
  const uint8_t b = obj["rgb"][2];

  if (!obj["rgb"].isNull()) {
    analogWrite(RGB_R, r);
    analogWrite(RGB_G, g);
    analogWrite(RGB_B, b);
  }

  heaterState = obj["heater"].isNull() ? heaterState : obj["heater"];
  ledState = obj["led"].isNull() ? ledState : obj["led"];
  shutterState = obj["shutter"].isNull() ? shutterState : obj["shutter"];

  temperature = obj["temperature"].isNull() ? temperature : obj["temperature"];
  humidity = obj["humidity"].isNull() ? humidity : obj["humidity"];
  consigne = obj["consigne"].isNull() ? consigne : obj["consigne"];
  msgReceived = true;
}

void sendState() {
  DynamicJsonDocument doc(100);

  doc["button"] = buttonState;
  doc["heater"] = heaterState;
  doc["led"] = ledState;
  doc["shutter"] = shutterState;

  char json_string[100];
  serializeJson(doc, json_string);

  Serial.println(json_string);

  client.publish("My Room/ESP32-2/state", json_string);
}

void initDisplay() {
  oled.setCursor(0, 0);
  oled.print("Temperature:");
  oled.setCursor(112, 0);
  oled.print("C");
  oled.setCursor(0, 1);
  oled.print("Humidite :");
  oled.setCursor(112, 1);
  oled.print("%");
  oled.setCursor(0, 2);
  oled.print("Consigne :");
  oled.setCursor(112, 2);
  oled.print("C");
  oled.setCursor(0, 4);
  oled.print("Chauffage :");
}

void updateDisplay() {
  oled.setCursor(0, 0);
  oled.clearField(75, 0, 5);
  oled.printf("%.2f ", temperature);
  oled.setCursor(0, 1);
  oled.clearField(75, 1, 5);
  oled.print(humidity);
  oled.setCursor(0, 2);
  oled.clearField(75, 2, 5);
  oled.print(consigne);
  oled.setCursor(0, 4);
  oled.clearField(75, 4, 5);
  oled.print(heaterState ? "ON" : "OFF");
}

void setup() {
  Serial.begin(115200);

  Wire.begin(21, 22);
  delay(250);
  oled.begin(&Adafruit128x64, 0x3C, -1);
  oled.setFont(Adafruit5x7);
  oled.clear();
  oled.print("Demarrage...");

  pinMode(BUTTON, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(SHUTTER, OUTPUT);
  pinMode(HEATER, OUTPUT);
  pinMode(RGB_R, OUTPUT);
  pinMode(RGB_G, OUTPUT);
  pinMode(RGB_B, OUTPUT);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  initDisplay();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  client.loop();

  boolean state = digitalRead(BUTTON);
  if (msgReceived || (state != buttonState)) {
    buttonState = state;
    sendState();
    updateDisplay();
  }

  if (msgReceived) {
    digitalWrite(HEATER, heaterState);
    analogWrite(LED, ledState);
    digitalWrite(SHUTTER, shutterState);
    msgReceived = false;
  }
}