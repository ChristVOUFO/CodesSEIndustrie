#include <DHT.h>
#include <WiFi.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <Adafruit_Sensor.h>

// Update these with values suitable for your network.

const char* ssid = "Christ";
const char* password = "1234ABCD";
const char* mqtt_server = "10.22.1.145";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;
int sensorValue;

// DHT 11 sensor
#define DHTPIN 26
#define DHTTYPE DHT11
#define PIN_RED    23 // GIOP23
#define PIN_GREEN  19 // GIOP22
#define PIN_BLUE   18 // GIOP21
#define BUTTON_PIN 12 // GIOP21 pin connected to button
// DHT sensor
DHT dht(DHTPIN, DHTTYPE);

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

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    analogWrite(PIN_GREEN, HIGH);
    Serial.print((char)payload[i]);
  }
  for (int i = 0; i > length; i--) {
    digitalWrite(PIN_GREEN, LOW);
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == 'O' && (char)payload[1] == 'N') {
    digitalWrite(BUILTIN_LED, HIGH);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } 
  else if ((char)payload[0] == 'O' && (char)payload[1] == 'F' && (char)payload[2] == 'F')
  {
    digitalWrite(BUILTIN_LED, LOW);
  }
   
  /*if (payload == payload ) {
    digitalWrite(PIN_RED, HIGH);
    digitalWrite(PIN_GREEN, HIGH);
    digitalWrite(PIN_BLUE, HIGH);

  } 
   else 
  {
    digitalWrite(PIN_RED, LOW); 
    digitalWrite(PIN_GREEN, LOW);
    digitalWrite(PIN_BLUE, LOW);
  }

  if (payload > 0) {
    digitalWrite(PIN_GREEN, HIGH);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } 
  else {
    digitalWrite(PIN_GREEN, LOW);  // Turn the LED off by making the voltage HIGH
  }

  if (payload > 0) {
    digitalWrite(PIN_BLUE, HIGH);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } 
  else {
    digitalWrite(PIN_BLUE, LOW);  // Turn the LED off by making the voltage HIGH
  }*/

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("LED");
      client.subscribe("RGB");
      
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  
  dht.begin();
  pinMode(PIN_RED,   OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE,  OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  // Grab the current state of the sensor
  //int humidity_data = dht.readHumidity();
  //int temperature_data = dht.readTemperature();
  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    /*snprintf (msg, MSG_BUFFER_SIZE, "hello world #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);*/
    
    int humidity = dht.readHumidity();
    int temperature = dht.readTemperature();
    sensorValue = analogRead(39); // read analog input pin 0

    DynamicJsonDocument doc(256);

    doc["BP"] = digitalRead(BUTTON_PIN) ? "ACTIF" : "NOT ACTIF";
    doc["TEMPERATURE"] = temperature;
    doc["HUMIDITY"] = humidity;
    doc["LDR"] = sensorValue;
  
  char json_string[256];
  serializeJson(doc, json_string);
  client.publish("Gauge", json_string);
  

 }