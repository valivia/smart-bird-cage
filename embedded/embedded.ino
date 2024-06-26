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

struct Sensor {
  const char *name;
  bool (*setupFunction)();
  void (*loopFunction)();
  bool has_failed;
  unsigned long execution_time;
};

Sensor sensors[] = {
  { "Display", setupDisplay, runDisplayLoop, false, 0 },
  { "WiFi", setupWiFi, runWiFiLoop, false, 0 },
  { "Loadcell", setupLoadcell, runLoadcellLoop, false, 0 },
  { "Radar", setupRadar, runRadarLoop, false, 0 },
  { "Microphone", setupMicrophone, runMicrophoneLoop, false, 0 },
  { "Light", setupLight, runLightLoop, false, 0 },
  { "Climate", setupClimateSensor, runClimateLoop, false, 0 }
};

// State
unsigned long last_upload = 0;
static bool has_failures = false;

void setup() {
  Serial.begin(115200);
  Serial.println("Initializing...");

  pinMode(STATUS_LED_PIN, OUTPUT);

  for (int i = 0; i < sizeof(sensors) / sizeof(Sensor); i++) {
    sensors[i].has_failed = !sensors[i].setupFunction();
    has_failures |= sensors[i].has_failed;
    Serial.print(sensors[i].name);
    Serial.print(": ");
    Serial.println(sensors[i].has_failed ? "Device failed to initialize." : "Device initialized.");
  }

  // If any sensor failed to initialize, turn on the status LED
  if (has_failures) {
    digitalWrite(STATUS_LED_PIN, HIGH);
    Serial.println("Core: One or more sensors failed to initialize.");
  }
}

void loop() {
  if ((millis() - last_upload) > UPLOAD_INTERVAL) {
    // If enough time has passed, summarize and send data
    SummarizeAndUploadData();
    last_upload = millis();
  } else {
    // Otherwise, poll the sensors
    pollSensors();
  }
}

void pollSensors() {
  for (int i = 0; i < sizeof(sensors) / sizeof(Sensor); i++) {
    if (sensors[i].has_failed)
      continue;

    unsigned long start = millis();
    sensors[i].loopFunction();
    sensors[i].execution_time = millis() - start;
  }
}

void SummarizeAndUploadData() {
  float temperature = measureTemperature();
  float humidity = getHumidity();
  float weight = getTemperature();
  int movement = getRadarValue();
  int light = getLightvalue();
  int sound = getMicrophoneValue();

  // Send the data to the server
  unsigned long start = millis();
  sendDataToServer(temperature, humidity, weight, movement, light, sound);
  sensors[1].execution_time += millis() - start;

  // Summarise core usage in percentages
  unsigned long total_execution_time = 0;
  for (int i = 0; i < sizeof(sensors) / sizeof(Sensor); i++) {
    total_execution_time += sensors[i].execution_time;
  }

  // Print the execution times
  Serial.println("Core: Execution times:");
  Serial.println("==================================");
  for (int i = 0; i < sizeof(sensors) / sizeof(Sensor); i++) {
    Serial.print(sensors[i].name);
    Serial.print(": ");
    Serial.print(sensors[i].execution_time);
    Serial.print("ms (");
    Serial.print((sensors[i].execution_time * 100) / total_execution_time);
    Serial.println("%)");
    sensors[i].execution_time = 0;
  }

  // Print the total execution time
  Serial.print("Total execution time: ");
  Serial.print(total_execution_time);
  Serial.println("ms");
  Serial.println("==================================");
}
