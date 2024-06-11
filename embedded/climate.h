#ifndef DHT22_MEASUREMENT_H
#define DHT22_MEASUREMENT_H

#include <DHT.h>

// Define global variables for temperature and humidity
extern float temperature;
extern float humidity;

// Function prototypes
void setupDHT22();
float measureTemperature();
float measureHumidity();

#endif // DHT22_MEASUREMENT_H
