#ifndef DHT22Sensor
#define DHT22Sensor
#include <DHT22.h>
//define pin data
#define pinDATA 4 // SDA, or almost any other I/O pin

DHT22 dht22(pinDATA); 
 
void setupDHT22() {
  Serial.begin(115200); //1bit=10µs
  Serial.println("Starting measurement for DTH22");
}

float measureTemperature() {
  float temperature = dht22.getTemperature();
}
  float measureHumidity() {
    float humidity = dht22.getHumidity();
  } 

#endif