#include "HX711.h"
#include "Display.h"

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

#define MAX_WEIGHT 200.0
#define MIN_WEIGHT 40.0

// NOTE not done, unreliable method
static HX711 loadcell;
static float weight = 0.0;
static float weight_offset = 0.0;

static bool is_loadcell_initialized = false;
static unsigned long loadcell_last_measurement = 0;

bool setupLoadcell() {
  loadcell.begin(DATA_PIN, CLOCK_PIN);
  // Calibrated using "HX_Calibration" from "HX711.h" library
  loadcell.set_offset(55637);
  loadcell.set_scale(477.479980);
  loadcell.set_runavg_mode();

  is_loadcell_initialized = true;
  return is_loadcell_initialized;
}

void runLoadcellLoop() {
  if (!loadcell.is_ready())
    return;

  if (millis() - loadcell_last_measurement < LOADCELL_POLLING_INTERVAL)
    return;

  float measure_avg = loadcell.get_units(5);
  float measure = loadcell.get_units(1);
  float ratio = measure / measure_avg;

  if (ratio < 0.9 && ratio > 1.1)
    return;

  if (measure < MIN_WEIGHT) {
    weight_offset = measure;
  } else {
    Serial.print("Loadcell: ");
    Serial.print(measure);
    Serial.print("g");
    Serial.print(" (offset: ");
    Serial.print(weight_offset);
    Serial.println("g)");
  }

  weight = measure - weight_offset;

  setWeight(weight);
}

float getLoadcellValue() {
  float temp_max_weight = weight;
  weight = 0.0;  // Reset max_weight to 0 after returning the value
  return temp_max_weight;
}