#define BLYNK_TEMPLATE_ID           "TMPLz7sUh_-y"
#define BLYNK_DEVICE_NAME           "TP5"
#define BLYNK_AUTH_TOKEN            "-Mx9rlT7Giwn5OU6i9plwlqlRXrjkDu_"


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

//WidgetLED led1(V1);

//BlynkTimer timer;
void led_pwm(int pwm_value);
byte pwm_pin = 2;

BLYNK_WRITE(V1)
{
  // Set incoming value from pin V0 to a variable
  //int value = param.asInt();
  led_pwm(param.asInt());

  // Update state
  //digitalWrite(2, value);
}

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

// This function sends Arduino's uptime every second to Virtual Pin 2.
/*void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, millis() / 1000);
}

// V1 LED Widget is blinking
void blinkLedWidget()
{
  if (led1.getValue()) {
    led1.off();
    Serial.println("LED on V1: off");
  } else {
    led1.on();
    Serial.println("LED on V1: on");
  }
}
void myTimerEvent()
{
  Blynk.virtualWrite(V5, millis() / 1000);
}*/

void setup()
{
  // Debug console
  Serial.begin(9600);
  pinMode (pwm_pin, OUTPUT);
  Blynk.begin(auth, ssid, pass);

  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  //timer.setInterval(1000L, blinkLedWidget);
}

void loop()
{
  Blynk.run();
  //timer.run();
}

void led_pwm(int pwm_value)
{
  analogWrite(pwm_pin, pwm_value);
}

/*#include <Arduino.h>*/
/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
/*#define BLYNK_TEMPLATE_ID           "TMPLz7sUh_-y"
#define BLYNK_DEVICE_NAME           "TP5"
#define BLYNK_AUTH_TOKEN            "-Mx9rlT7Giwn5OU6i9plwlqlRXrjkDu_"


// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Adafruit_NeoPixel.h>


char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Christ";
char pass[] = "1234ABCD";

#define DHTPIN 18          // What digital pin we're connected to
#define DHTTYPE DHT11     // DHT 11
DHT dht(DHTPIN, DHTTYPE);

int prevState = -1;
int currState = -1;
long lastChangeTime = 0;

#define PIN 0

Adafruit_NeoPixel strip = Adafruit_NeoPixel(30, PIN, NEO_GRB + NEO_KHZ800);

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if (WheelPos < 85) {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);

  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    

  } else {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    

  }
}


BLYNK_WRITE(V8)
{
  int shift = param.asInt();
  for (int i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, Wheel(shift & 255));
    // OR: strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + shift) & 255));
  }
  strip.show();
}



void checkPin()
{
  // Invert state, since button is "Active LOW"
  int state = !digitalRead(12);

  // Debounce mechanism
  long t = millis();
  if (state != prevState) {
    lastChangeTime = t;
  }
  if (t - lastChangeTime > 50) {
    if (state != currState) {
      currState = state;
      Blynk.virtualWrite(V7, state);
    }
  }
  prevState = state;
}


BlynkTimer timer;

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more than 10 values per second.
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);
}


// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0)
{
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();

  // Update state
  digitalWrite(15, value);
}

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more than 10 values per second.
  Blynk.virtualWrite(V2, millis() / 1000);
}

void setup()
{
  // Debug console
  Serial.begin(115200);
  pinMode (15,OUTPUT);

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  dht.begin();

  strip.begin();

  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);

  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);

  // Make pin 2 default HIGH, and attach INT to our handler
  pinMode(12, INPUT_PULLUP);

  strip.show();

 

}

void loop()
{
  Blynk.run();
  timer.run();
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
  checkPin();
} */