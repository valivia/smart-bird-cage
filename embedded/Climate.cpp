#include "DHT22Measurement.h"

#define SENSOR_PIN 4
#define SENSOR_TYPE DHT22
#define MEASUREMENT_INTERVAL 10000

DHT dht(SENSOR_PIN, SENSOR_TYPE);

void setupClimateSensor()
{
  dht.begin();
  Serial.println("Climate: Sensor initialized.");
}

float measureTemperature()
{
  float temp = dht.readTemperature();
  if (isnan(temp))
  {
    Serial.println("Climate: Failed to read temperature from DHT sensor!");
    return 0;
  }
  return temp;
}

float measureHumidity()
{
  float hum = dht.readHumidity();
  if (isnan(hum))
  {
    Serial.println("Climate: Failed to read humidity from DHT sensor!");
    return 0;
  }
  return hum;
}
