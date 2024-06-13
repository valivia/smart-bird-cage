#include <Arduino.h>
#include "Climate.h"
#include "Microphone.h"
#include "Radar.h"
#include "Loadcell.h"
#include "Connection.h"
#include "LDR.h"

#define UPLOAD_INTERVAL 15 * 1000
#define STATUS_LED 2

unsigned long lastTime = 0;

int sound = 0;

void setup()
{
  Serial.begin(115200);

  pinMode(STATUS_LED, OUTPUT);

  bool hasFailures = false;

  // Initialize the sensors.
  hasFailures |= !setupClimateSensor();
  hasFailures |= !setupLoadcell();
  hasFailures |= !setupRadar();
  hasFailures |= !setupLDR();
  setupMicrophone(); // NOTE idk how to use this one

  // If any sensor failed to initialize, turn on the status LED
  if (hasFailures)
    digitalWrite(STATUS_LED, LOW);

  // Initialize other
  setupWiFi();
}

void loop()
{
  // If enough time has passed, summarize and send data
  if ((millis() - lastTime) > UPLOAD_INTERVAL)
  {
    SummarizeAndUploadData();
    lastTime = millis();
  }
  // Otherwise, poll the sensors
  else
  {
    pollSensors();
  }
}

void pollSensors()
{
  runLoadcellLoop();
  runRadarLoop();
  runLDRLoop();
  runMicrophoneLoop();
}

void SummarizeAndUploadData()
{
  float temperature = measureTemperature();
  float humidity = measureHumidity();
  float weight = getLoadcellValue();
  int movement = getRadarValue();
  int light = getLDRValue();
  int sound = getMicrophoneValue();

  sendDataToServer(temperature, humidity, weight, movement, light, sound);
}
