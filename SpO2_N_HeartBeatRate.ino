#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSerif9pt7b.h>
Adafruit_SSD1306 display(128, 64, &Wire, -1);
#include "MAX30100_PulseOximeter.h"
#define REPORTING_PERIOD_MS     1000
PulseOximeter pox;

uint32_t tsLastReport =0 ;
uint32_t lastBeat =0;
uint32_t lastSpO2 =0; 
void onBeatDetected() {
  Serial.println("Beat!");
}
void setup() {
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
    for (;;);
  }
  display.setFont(&FreeSerif9pt7b);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(20, 15);
  display.println("Welcom to");
  display.setCursor(30, 40);
  display.println("Group 7!");
  display.display();
  display.setTextSize(1);
  delay(2000);
  Serial.print("Initializing pulse oximeter..");
  if (!pox.begin()) {
    Serial.println("FAILED");
    for (;;);
  } else {
    Serial.println("SUCCESS");
  }
  pox.setOnBeatDetectedCallback(onBeatDetected);
}
void loop() {
  pox.update();
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    Serial.print("Heart rate:");
    Serial.print(pox.getHeartRate());
    Serial.print("bpm / SpO2:");
    Serial.print(pox.getSpO2());
    Serial.println("%");
    display.clearDisplay();
    display.setCursor(10, 12);
    display.print("Pulse Oximeter");
    display.setCursor(0, 35);
    display.print("HeartR:");
    display.setCursor(62, 35);
    uint32_t current = pox.getHeartRate();
    if (current == 0) {
      display.print((int) lastBeat , 0);
    } else {
      display.print((int) current , 0);
      lastBeat = current;
    }
    display.println(" bpm");
    
    display.setCursor(0, 59);
    display.print("SpO2  : ");
    display.setCursor(62, 59);
//    display.print(pox.getSpO2());
     current = pox.getSpO2();
    if (current == 0) {
      display.print((int) lastSpO2 , 0);
    } else {
      display.print((int) current , 0);
      lastSpO2 = current;
    }
    display.println(" %");
    display.display();
    tsLastReport = millis();
  }
}