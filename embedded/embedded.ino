#include <Arduino.h>

// Sensors
#include "Climate.h"
#include "Microphone.h"
#include "Radar.h"
#include "Loadcell.h"
#include "Light.h"

// Other
#include "Connection.h"
#include "Display.h"

// Configuration
#define UPLOAD_INTERVAL 15 * 1000
#define STATUS_LED_PIN 2

// State
unsigned long lastTime = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("Initializing...");

  pinMode(STATUS_LED_PIN, OUTPUT);
  bool hasFailures = false;

  // Initialize the sensors.
  hasFailures |= !setupClimateSensor();
  hasFailures |= !setupLoadcell();
  hasFailures |= !setupRadar();
  hasFailures |= !setupLight();
  hasFailures |= !setupMicrophone();

  // If any sensor failed to initialize, turn on the status LED
  if (hasFailures) {
    digitalWrite(STATUS_LED_PIN, HIGH);
    Serial.println("One or more sensors failed to initialize.");
  }

  // Initialize other
  setupWiFi();
  setupDisplay();
}

void loop() {
  if ((millis() - lastTime) > UPLOAD_INTERVAL) {
    // If enough time has passed, summarize and send data
    SummarizeAndUploadData();
    lastTime = millis();
  } else {
    // Otherwise, poll the sensors
    pollSensors();
  }
}

void pollSensors() {
  runLoadcellLoop();
  runRadarLoop();
  runMicrophoneLoop();
  runLightLoop();
}

void SummarizeAndUploadData() {
  float temperature = measureTemperature();
  float humidity = measureHumidity();
  float weight = getLoadcellValue();
  int movement = getRadarValue();
  int light = getLightvalue();
  int sound = getMicrophoneValue();

  sendDataToServer(temperature, humidity, weight, movement, light, sound);
  displayValues(weight);
}
