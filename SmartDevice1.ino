// main.cpp

#include <Arduino.h>
#include "WiFiManagerSetup.h"
#include "OTAUpdate.h"
#include <TelnetStream.h>

/********** SET PROJECT DETAILS HERE *****/
const String projectName = "SmartDevice1";
String currentVersion = "0.0";

bool debugMode = true; //if true, frequent update checks
int updateInterval = 60; //in minutes ifdebug false
/*****************************************/

/****** DECLARE GLOBAL VARIABLES HERE *****/
static unsigned long lastLedToggle = 0;
static unsigned long lastUpdateCheck = 0; 
int LED_WIFI = 32;
int LED_RUNTIME = 33;

/*****************************************/

void setup() {
  Serial.begin(115200);
  Serial.printf("Initialising");
  for (uint8_t t = 5; t > 0; t--) {
    Serial.printf(".");
    Serial.flush();
    delay(1000);
  }
  Serial.println("Connecting to WIFI...");
  // Use WiFiManagerSetup to configure WiFi
  WiFiManagerSetup::setupWiFi("SmartDevice-WIFI", "AutoConnect");

  // Set the LED pin as an OUTPUT
  pinMode(LED_RUNTIME, OUTPUT);
  TelnetStream.begin();
  Serial.println("Connected To WIFI!");
  TelnetStream.println("Connected To WIFI!");

   // Start the Initial firmware update check
  OTAUpdater::checkForUpdate(projectName, currentVersion);
}

void loop() {
  /************* DO NOT DELETE THIS BLOCK ***************/
  //Run the OTA Sketch on EVERY TICK
  if(debugMode){
    updateInterval = 1; //1min
  }
  if (millis() - lastUpdateCheck >= 60000*updateInterval) {
    lastUpdateCheck = millis();

  OTAUpdater::checkForUpdate(projectName, currentVersion);
  }
  /******************************************************/

/**************YOUR CODE STARTS HERE *********************/
  static int secCounter = 0;
  static long int lastSecondMark =0;
  if (millis() - lastSecondMark >= 1000){
    lastSecondMark = millis();
  TelnetStream.println(secCounter++);
  }
  // Check if 500 milliseconds have passed for LED toggle
  if (millis() - lastLedToggle >= 500) {
    lastLedToggle = millis();
    // Toggle the onboard LED
    digitalWrite(LED_RUNTIME, !digitalRead(LED_RUNTIME));
  }

}
