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
#define LOADCELL_OFFSET 66579
#define LOADCELL_SCALE 360.584747

#define MAX_WEIGHT 110.0
#define MIN_WEIGHT 70.0

static HX711 loadcell;

static float last_weight = 0.0;

static float total_weight = 0.0;
static int total_weight_count = 0;

static bool is_loadcell_initialized = false;
static unsigned long loadcell_last_measurement = 0;

bool setupLoadcell() {
  loadcell.begin(DATA_PIN, CLOCK_PIN);
  // Calibrated using "HX_Calibration" from "HX711.h" library
  loadcell.set_offset(LOADCELL_OFFSET);
  loadcell.set_scale(LOADCELL_SCALE);
  loadcell.set_runavg_mode();

  is_loadcell_initialized = true;
  return is_loadcell_initialized;
}

void runLoadcellLoop() {
  if (!loadcell.is_ready())
    return;

  if (millis() - loadcell_last_measurement < LOADCELL_POLLING_INTERVAL)
    return;

  float weight_measurement = loadcell.get_units(3);

  if (weight_measurement < MIN_WEIGHT)
    return;

  float ratio = weight_measurement / last_weight;
  last_weight = weight_measurement;

  if (ratio < 0.9 && ratio > 1.1)
    return;

  total_weight += weight_measurement;
  total_weight_count++;

  setWeight(weight_measurement);
}

float getLoadcellValue() {
  if (total_weight_count == 0)
    return -1;

  float average_weight = total_weight / total_weight_count;
  total_weight = 0.0;
  total_weight_count = 0;

  if (average_weight > MAX_WEIGHT || average_weight < MIN_WEIGHT)
    return -1;

  return average_weight;
}