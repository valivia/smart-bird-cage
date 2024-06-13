#include "DHT.h"

#define DHT_DATA_PIN 4
#define DHT_SENSOR_TYPE DHT22
#define TMP_CALIBRATION_FACTOR 1.1

DHT dht(DHT_DATA_PIN, DHT_SENSOR_TYPE);

bool is_climate_sensor_initialized = false;

bool setupClimateSensor()
{
  dht.begin();
  if (dht.read())
  {
    Serial.println("Climate: Sensor initialized.");
    is_climate_sensor_initialized = true;
  }
  else
  {
    Serial.println("Climate: Failed to initialize sensor");
  }

  return is_climate_sensor_initialized;
}

float measureTemperature()
{
  if (!is_climate_sensor_initialized)
    return 0;

  float temp = dht.readTemperature() / TMP_CALIBRATION_FACTOR;
  if (isnan(temp))
  {
    Serial.println("Climate: Failed to read temperature from DHT sensor!");
    return 0;
  }
  return temp;
}

float measureHumidity()
{
  if (!is_climate_sensor_initialized)
    return 0;

  float hum = dht.readHumidity();
  if (isnan(hum))
  {
    Serial.println("Climate: Failed to read humidity from DHT sensor!");
    return 0;
  }
  return hum;
}
