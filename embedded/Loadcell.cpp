#include "HX711.h"
#define DATA_PIN 19
#define CLOCK_PIN 21

// NOTE not done, unreliable method
static HX711 loadcell;
static float weight = 0.0;
static float weight_measure_1 = 0.0;
static float weight_measure_2 = 0.0;
static float weight_measure_3 = 0.0;
static float weight_measure_4 = 0.0;
static float weight_measure_5 = 0.0;
static float avg_weight_measure = 0.0;
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

  weight_measure_1 = weight_measure_2;
  weight_measure_2 = weight_measure_3;
  weight_measure_3 = weight_measure_4;
  weight_measure_4 = weight_measure_5;
  weight_measure_5 = loadcell.get_units(5);
  avg_weight_measure = (weight_measure_1 + weight_measure_2 + weight_measure_3 + weight_measure_4 + weight_measure_5) / 5;

  if ((avg_weight_measure / weight_measure_5) > 0.9 && (avg_weight_measure / weight_measure_5) < 1.1){
    weight = avg_weight_measure;
  }
}

float getLoadcellValue()
{
  return weight;
}