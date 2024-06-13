#include "HX711.h"
#define DATA_PIN 19
#define CLOCK_PIN 21

// NOTE not done, unreliable method
static HX711 loadcell;
static float weight = 0.0;
static bool is_loadcell_initialized = false;

bool setupLoadcell()
{
  loadcell.begin(DATA_PIN, CLOCK_PIN);
  Serial.println("Loadcell: Sensor initialized.");
  loadcell.calibrate_scale(1000, 5);
  is_loadcell_initialized = true;

  return is_loadcell_initialized;
}

void runLoadcellLoop()
{
  if (!is_loadcell_initialized)
    return;

  float measuredWeight = loadcell.get_units(5);
  if (measuredWeight > 10)
  {
    weight = measuredWeight;
  }
}

float getLoadcellValue()
{
  return weight;
}