#include <HTTPClient.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "Credentials.h"

bool setupWiFi() {
  WiFi.begin(ssid, password);
  Serial.println("wifi: Connecting...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("wifi: Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  return true;
}

void runWiFiLoop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("wifi: Connection lost, reconnecting...");
    setupWiFi();
  }
}

void sendDataToServer(float temperature, float humidity, float weight, int movement, int light, int sound) {
  // Check WiFi connection status
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wifi: not connected");
    return;
  }

  WiFiClientSecure client;
  HTTPClient http;

  client.setInsecure();

  http.begin(client, serverName);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Authorization", token);

  String weight_string = String(weight);
  if (weight < 40 || weight > 200) {
    weight_string = "null";
  }

  // Construct the JSON string
  String http_request_data = "{\"device_id\": " + String(device_id) + ",\"temperature\": " + String(temperature) + ",\"humidity\": " + String(humidity) + ",\"weight\": " + weight_string + ",\"movement\": " + String(movement) + ",\"light\": " + String(light) + ",\"sound\": " + String(sound) + "}";

  // Send the HTTP POST request
  int http_response_code = http.POST(http_request_data);

  http.end();

  Serial.print("Wifi: HTTP Request Data:");
  Serial.println(http_request_data);
  Serial.print("Wifi: HTTP response code: ");
  Serial.println(http_response_code);
}