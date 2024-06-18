#include <BH1750.h>
#include <Wire.h>


#define DISPLAY_DATA_PIN 22
#define DISPLAY_CLOCK_PIN 21

BH1750 lightMeter;
float lux = 0;
static bool is_light_initialized = false;

bool setupLight() {
  Wire.begin(DISPLAY_DATA_PIN, DISPLAY_CLOCK_PIN);
  lightMeter.begin();

  Serial.println("BH1750 initialized");
  is_light_initialized = true;

  return is_light_initialized;
}

void runLightLoop() {
  if (!is_light_initialized)
    return;
  lux = lightMeter.readLightLevel();
}

float getLightvalue() {
  return lux;
}