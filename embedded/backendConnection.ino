#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <DHT22.h>
#include "DHT22Measurement.h"

// WiFi credentials
const char* ssid = "AndroidAP";
const char* password = "qwertyuiop";

// Server settings
const char* serverName = "https://bird.hootsifer.com/api/v1/device";

unsigned long lastTime = 0;
unsigned long timerDelay = 5000; // 5 seconds delay

void setup() {
  Serial.begin(115200);


  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}

void loop() {
  // Read sensor values
  int device_id = 1;
  
  float movement = 0.0;
  float weight = 93.5;
  int light = 0;
  float sound = 40.0;

  // Send an HTTP POST request every timerDelay milliseconds
  if ((millis() - lastTime) > timerDelay) {
    float temperature = measureTemperature();
    float humidity = measureHumidity();
    // Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED) {
      WiFiClientSecure client;
      HTTPClient http;

      // Disable certificate validation (use with caution in production)
      client.setInsecure(); 

      http.begin(client, serverName);

      // Specify content-type header
      http.addHeader("Content-Type", "application/json");

      // Construct the JSON string
      String httpRequestData = "{\"device_id\": " + String(device_id) +
                               ",\"movement\": " + String(movement) + 
                               ",\"temperature\": " + String(temperature) + 
                               ",\"humidity\": " + String(humidity) + 
                               ",\"weight\": " + String(weight) +
                                ",\"light\": " + String(light) +
                                 ",\"sound\": " + String(sound) + "}";

      Serial.println("HTTP Request Data:");
      Serial.println(httpRequestData);

      // Send HTTP POST request
      int httpResponseCode = http.POST(httpRequestData);
      Serial.println(httpResponseCode);

      // Free resources
      http.end();
    } else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}
