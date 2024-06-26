#include <BH1750.h>
#include <Wire.h>
#include "Display.h"

// Needs 3.3V
#define DISPLAY_DATA_PIN 22
#define DISPLAY_CLOCK_PIN 21

#define LIGHT_POLLING_INTERVAL 1000

BH1750 lightMeter;
int light_measurement_count = 0;
int light_total = 0;

unsigned long light_last_measurement = 0;
static bool is_light_initialized = false;

bool setupLight() {
  Wire.begin(DISPLAY_DATA_PIN, DISPLAY_CLOCK_PIN);
  lightMeter.begin();
  is_light_initialized = true;

  return is_light_initialized;
}

void runLightLoop() {
  if (millis() - light_last_measurement < LIGHT_POLLING_INTERVAL)
    return;

  light_total += lightMeter.readLightLevel();
  setLight(lightMeter.readLightLevel());
  light_measurement_count++;
}

float getLightvalue() {
  int lux = light_total / light_measurement_count;
  light_total = 0;
  light_measurement_count = 0;
  return lux;
}