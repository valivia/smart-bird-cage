#include <Arduino.h>
#include "Climate.h"
#include "Microphone.h"
#include "Radar.h"
#include "Loadcell.h"
#include "Connection.h"
#include "LDR.h"

#define UPLOAD_INTERVAL 15 * 1000
unsigned long lastTime = 0;

int sound = 0;

void setup()
{
  Serial.begin(115200);

  // Initialize the sensors
  setupDHT22();
  setupMicrophone();
  setupRadar();
  setupLoadcell();
}

void loop()
{
  // If enough time has passed, summarize and send data
  if ((millis() - lastTime) > UPLOAD_INTERVAL)
  {
    SummarizeAndSendData();
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

void SummarizeAndSendData()
{
  float temperature = measureTemperature();
  float humidity = measureHumidity();
  float weight = getWeight();
  int movement = getRadarValue();
  int light = getLDRValue();
  int sound = getSound();

  sendDataToServer(temperature, humidity, weight, movement, light, sound);
}
