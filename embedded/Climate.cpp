#include "DHT.h"
#include "Display.h"

// From left to right: Power, Data pin with 10k Ohm resitor and 3.3V, Ground
// Needs 3.3V

#define DHT_DATA_PIN 4
#define DHT_SENSOR_TYPE DHT22
#define TMP_CALIBRATION_FACTOR 1

DHT dht(DHT_DATA_PIN, DHT_SENSOR_TYPE);

static bool is_climate_sensor_initialized = false;
static bool has_measurement = false;
static float temperature = 0;
static float humidity = 0;

bool setupClimateSensor() {
  dht.begin();
  if (dht.read()) {
    is_climate_sensor_initialized = true;
  }

  return is_climate_sensor_initialized;
}

float measureTemperature() {
  if (!is_climate_sensor_initialized)
    return 0;

  float temp = dht.readTemperature();
  if (isnan(temp)) {
    Serial.println("Climate: Failed to read temperature from DHT sensor!");
    return 0;
  }

  temp = temp * TMP_CALIBRATION_FACTOR;

  setTemperature(temp);
  return temp;
}

float measureHumidity() {
  if (!is_climate_sensor_initialized)
    return 0;

  float hum = dht.readHumidity();
  if (isnan(hum)) {
    Serial.println("Climate: Failed to read humidity from DHT sensor!");
    return 0;
  }
  return hum;
}

void runClimateLoop() {
  if (has_measurement)
    return;

  temperature = measureTemperature();
  humidity = measureHumidity();
  has_measurement = true;
}

float getTemperature() {
  has_measurement = false;
  return temperature;
}

float getHumidity() {
  has_measurement = false;
  return humidity;
}