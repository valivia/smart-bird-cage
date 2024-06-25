#include "HX711.h"

// ## Esp side:
// vcc = 5V
// ## Loadcell side wiring:
// red (vcc) = e+
// black (gnd) = e-
// white (output-) = a-
// green (output+) = a+

#define DATA_PIN 19
#define CLOCK_PIN 18
#define LOADCELL_POLLING_INTERVAL 1000

// NOTE not done, unreliable method
static HX711 loadcell;
static float weight = 0.0;
static float max_weight = 0.0;
static float weight_measure_1 = 0.0;
static float weight_measure_2 = 0.0;
static float weight_measure_3 = 0.0;
static float weight_measure_4 = 0.0;
static float weight_measure_5 = 0.0;
static float avg_weight_measure = 0.0;
static bool is_loadcell_initialized = false;
static unsigned long loadcell_last_measurement = 0;

bool setupLoadcell()
{
  loadcell.begin(DATA_PIN, CLOCK_PIN);
  // Calibrated using "HX_Calibration" from "HX711.h" library
  loadcell.set_offset(55637);
  loadcell.set_scale(477.479980);
  Serial.println("Loadcell: Sensor initialized.");
  is_loadcell_initialized = true;

  return is_loadcell_initialized;
}

void runLoadcellLoop()
{
  if (!is_loadcell_initialized)
    return;

  if (!loadcell.is_ready() || millis() - loadcell_last_measurement < LOADCELL_POLLING_INTERVAL)
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

  if (weight > max_weight){
    max_weight = weight;
  }
}

float getLoadcellValue()
{
  float temp_max_weight = max_weight;
  max_weight = 0.0; // Reset max_weight to 0 after returning the value
  return temp_max_weight;
}