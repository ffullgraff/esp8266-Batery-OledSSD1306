 #include "SSD1306Brzo.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SimpleTimer.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define ONE_WIRE_BUS 0    // D3 en wemos D1

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

char auth[] = "d057580798ec4c96acb1b8b747c159d8";
char ssid[] = "FFULLHE";
char pass[] = "mg833857";

SimpleTimer timer;

// Optionally include custom images
// Initialize the OLED display using brzo_i2c:
 SSD1306Brzo display(0x3c, 5, 4);  // ADDRESS, SDA, SCL

#define DEMO_DURATION 3000
typedef void (*Demo)(void);

int demoMode = 0;
int counter = 1;
int roomTemperature; 

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();
 // Wire.begin();
  //Wire.setClock(400000L);
  sensors.begin();

  // Initialising the UI will init the display too.
  display.init();

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  
  uint32_t m = micros();
Blynk.begin(auth, ssid, pass);
while (Blynk.connect() == false);

}

void drawFontFaceDemo() {
    // Font Demo1
    // create more fonts at http://oleddisplay.squix.ch/
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 0, "Hello world");
    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 10, "Dallas Temperat");
    display.setFont(ArialMT_Plain_24);
    display.drawString(0, 26, String(sensors.getTempCByIndex(0) ));
}

Demo demos[] = {drawFontFaceDemo}; // mdificar aqui primero
int demoLength = (sizeof(demos) / sizeof(Demo));
long timeSinceLastModeSwitch = 0;

void loop(void) {
  // clear the display
  display.clear();
  // draw the current demo method
  demos[demoMode]();
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  float tempC = sensors.getTempCByIndex(0);
   Serial.println(tempC);

  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.drawString(10, 128, String(millis()));
  // write the buffer to the display
  display.display();

  if (millis() - timeSinceLastModeSwitch > DEMO_DURATION) {
    demoMode = (demoMode + 1)  % demoLength;
    timeSinceLastModeSwitch = millis();
  }
  counter++;
  delay(20);

  Blynk.run();
  timer.run();

  sensors.requestTemperatures();                  // Polls the sensors.
  roomTemperature = sensors.getTempCByIndex(0);   // Stores temperature. Change to getTempCByIndex(0) for celcius.
  Blynk.virtualWrite(1, roomTemperature);         // Send temperature to Blynk app virtual pin 1.
  
  
}
