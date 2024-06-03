#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include "DHT22Measurement.h"
#include "PIRMeasurement.h"
#include "HX711.h"

#define LDRpin 35

int device_id = 1;

// // WiFi credentials
const char* ssid = "AndroidAP";
const char* password = "qwertyuiop";

// // Server settings
const char* serverName = "https://bird.hootsifer.com/api/v1/data";

unsigned long lastTime = 0;
const unsigned long timerDelay = 10000; // 10 seconds delay

const unsigned long dhtDelay = 2000;
unsigned long dhtMeasured = 0;

// PIR
int movement = 0;

// LDR
int light = 0;
int lux = 0;

// Loadcell
HX711 scale;
uint8_t dataPin = 19;
uint8_t clockPin = 21;
float measuredWeight = -1.0;
float weight = -1.0;

void setup() {
  Serial.begin(115200);
  
  setupDHT22();
  setupPIR();
  scale.begin(dataPin, clockPin);
  scale.calibrate_scale(1000, 5); // MOET NOG GECALIBREERD WORDEN!!!!

  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Timer set to 10 seconds (timerDelay variable), it will take 10 seconds before publishing the first reading.");
}

void loop() {
  
  // NOG TE FIXEN SENSORS
  int sound = 1;

  unsigned long currentMillis = millis();

  // Measure temperature and humidity
  if (currentMillis - dhtMeasured >= dhtDelay) {
      temperature = measureTemperature();
      humidity = measureHumidity();
      dhtMeasured = currentMillis; // set the time since measurement to the current time
    }

  // Measure movements
  checkMovement();

  // Measure Weight
  measuredWeight = scale.get_units(5);
  // Check if bird is there
  if (measuredWeight > 10){
    weight = measuredWeight;
  }

  // Send an HTTP POST request every timerDelay milliseconds
  if ((millis() - lastTime) > timerDelay) {
    // Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED) {
      WiFiClientSecure client;
      HTTPClient http;

      // Disable certificate validation (use with caution in production)
      client.setInsecure(); 

      http.begin(client, serverName);

      // Specify content-type header
      http.addHeader("Content-Type", "application/json");
      // Add authorization header
      http.addHeader("Authorization", "beepboopbeepbeep");
     
      // Update movements
      movement = returnMovements();

      // Measure Light value
      light = analogRead(LDRpin);
      lux = light / 4.45; // calibrated to convert raw ldr value to lux with 10k Ohm resistor

      // Construct the JSON string
      String httpRequestData = "{\"device_id\": " + String(device_id) +
                               ",\"movement\": " + String(movement) + 
                               ",\"temperature\": " + String(temperature) + 
                               ",\"humidity\": " + String(humidity) + 
                               ",\"weight\": " + String(weight) +
                                ",\"light\": " + String(lux) +
                                 ",\"sound\": " + String(sound) + "}";

      

  //     // Send HTTP POST request
      int httpResponseCode = http.POST(httpRequestData);
      Serial.println("HTTP Request Data:");
      Serial.println(httpRequestData);
      Serial.println(httpResponseCode);
      // Free resources
      http.end();
    } else {
      Serial.println("WiFi Disconnected");
    }
    movements = 0;
    lastTime = millis();
  }
}
