#include <HTTPClient.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "Credentials.h"
#include "Display.h"

bool setupWiFi() {
  WiFi.begin(ENV_SSID, ENV_PASSWORD);
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

  setUpdating(true);
  runDisplayLoop();

  WiFiClientSecure client;
  client.setInsecure();
  client.setHandshakeTimeout(10);

  HTTPClient http;
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Authorization", ENV_TOKEN);
  http.setTimeout(5000);
  http.setConnectTimeout(5000);
  http.useHTTP10(true);
  http.setReuse(false);
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);

  if (http.begin(client, ENV_SERVER_NAME)) {
    Serial.println("Wifi: Connected to server");
  } else {
    Serial.println("Wifi: Failed to connect to server");
    return;
  }

  String weight_string = String(weight);
  if (weight == -1) {
    weight_string = "null";
  }

  // Construct the JSON string
  String http_request_data = "{\"device_id\": " + String(ENV_DEVICE_ID) + ",\"temperature\": " + String(temperature) + ",\"humidity\": " + String(humidity) + ",\"weight\": " + weight_string + ",\"movement\": " + String(movement) + ",\"light\": " + String(light) + ",\"sound\": " + String(sound) + "}";

  // Send the HTTP POST request
  int http_response_code = http.POST(http_request_data);

  http.end();

  Serial.print("Wifi: HTTP Request Data:");
  Serial.println(http_request_data);
  Serial.print("Wifi: HTTP response code: ");
  Serial.println(http_response_code);
  if (http_response_code < 0) {
    Serial.println(http.errorToString(http_response_code));
    Serial.println(http.getString());
  }

  setUpdating(false);
}