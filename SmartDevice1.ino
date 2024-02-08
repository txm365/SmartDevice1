// main.cpp

#include <Arduino.h>
#include "WiFiManagerSetup.h"
#include "OTAUpdate.h"
#include <TelnetStream.h>
#include <stdlib.h>
#include <U8g2lib.h>
#include "u8g2_functions.h"
#include "time.h"


//#include "u8g2_instances.cpp"

/********** SET PROJECT DETAILS HERE *****/
const String projectName = "SmartDevice1";
String currentVersion = "0.8";

bool debugMode = true; // if true, frequent update checks
int updateInterval = 60; // in minutes if debug false
/*****************************************/

/****** DECLARE GLOBAL VARIABLES HERE *****/
static unsigned long lastLedToggle = 0;
static unsigned long lastUpdateCheck = 0;
int LED_WIFI = 32;
int LED_RUNTIME = 33;

// Timezone for South Africa (Standard Time)
const char *ntpServer = "za.pool.ntp.org";
const long gmtOffset_sec = 2 * 3600;  // UTC+2 (standard time)
const int daylightOffset_sec = 0;      // South Africa does not observe daylight saving time

const int buttonPin = 0;  // Replace with the actual GPIO pin for your button
volatile bool buttonPressed = false;
// Define enum for different page states
enum PageState {
  HOME,
  TEMPERATURE,
  HUMIDITY,
  PRESSURE
};

PageState currentPage = HOME;

void buttonInterrupt() {
  // Handle button interrupt
  currentPage = static_cast<PageState>((currentPage + 1) % (PRESSURE + 1));
}
/*****************************************/
void setup() {
  Serial.begin(115200);
  u8g2Functions.u8g2Initialization();

  // Set the LED pin as an OUTPUT
  pinMode(LED_RUNTIME, OUTPUT);

  Serial.printf("Initialising");
  for (uint8_t t = 2; t > 0; t--) {
    Serial.printf(".");
    u8g2Functions.welcomePage();
    Serial.flush();
    delay(1000);
  }

  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting to WiFi...");
    // Use WiFiManagerSetup to configure WiFi
    WiFiManagerSetup::setupWiFi("Smart-Device-WiFi", "AutoConnect");
  }

  TelnetStream.begin();
  Serial.println("Connected To WIFI!");
  TelnetStream.println("Connected To WIFI!");

  // Initialize time with timezone settings
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  // Wait for time to be set
  time_t now;
  while ((now = time(nullptr)) < 1609459200) {  // January 1, 2021
    Serial.println("Waiting for time to be set...");
    delay(100);
  }

  // Start the Initial firmware update check
  OTAUpdater::checkForUpdate(projectName, currentVersion);
  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonInterrupt, FALLING);
}

void loop() {
  // Run the OTA Sketch on EVERY TICK
  if (debugMode) {
    updateInterval = 1; // 1min
  }
  if (millis() - lastUpdateCheck >= 60000 * updateInterval) {
    lastUpdateCheck = millis();
    OTAUpdater::checkForUpdate(projectName, currentVersion);
  }

  static int secCounter = 0;
  static long int lastSecondMark = 0;
  if (millis() - lastSecondMark >= 1000) {
    lastSecondMark = millis();
    TelnetStream.println(secCounter++);
  }

  // Check if 500 milliseconds have passed for LED toggle
  if (millis() - lastLedToggle >= 500) {
    lastLedToggle = millis();
    digitalWrite(LED_RUNTIME, !digitalRead(LED_RUNTIME));
  }

  //=========================================================//
  switch (currentPage) {
      case HOME:
        u8g2Functions.homePage();
        break;
        case TEMPERATURE:
        u8g2Functions.tempPage();
        break;
        case HUMIDITY:
        u8g2Functions.humPage();
        break;
        case PRESSURE:
        u8g2Functions.presPage();
        break;
  }
    
    //u8g2Functions.tempPage(35.9);
    // humPage(); //TODO
  //u8g2.sendBuffer();
 
}
