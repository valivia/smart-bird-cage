#include "HX711.h"
#define DATA_PIN 19
#define CLOCK_PIN 21

// NOTE not done, unreliable method
HX711 loadcell;
float weight = 0.0;
bool is_loadcell_initialized = false;

bool setupLoadcell()
{
  loadcell.begin(DATA_PIN, CLOCK_PIN);

  // NOTE supposed to check if the loadcell is connected, doesnt work!?
  if (loadcell.wait_ready_timeout(1000))
  {
    Serial.println("Loadcell: Sensor initialized.");
    loadcell.calibrate_scale(1000, 5);
    is_loadcell_initialized = true;
  }
  else
    Serial.println("Loadcell: Failed to initialize sensor.");

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