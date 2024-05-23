#include "DHT22Measurement.h"

// Define pin and type for DHT sensor
#define DHTPIN 4       // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22  // DHT 22 (AM2302)

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// Define global variables
float temperature = -1;
float humidity = -1;

void setupDHT22() {
  // Initialize the DHT sensor
  dht.begin();
}

float measureTemperature() {
  float temp = dht.readTemperature();
  if (isnan(temp)) {
    Serial.println("Failed to read temperature from DHT sensor!");
    return -1;
  }
  return temp;
}

float measureHumidity() {
  float hum = dht.readHumidity();
  if (isnan(hum)) {
    Serial.println("Failed to read humidity from DHT sensor!");
    return -1;
  }
  return hum;
}
