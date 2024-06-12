#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "Credentials.h"

void setupWifi()
{
    WiFi.begin(ssid, password);
    Serial.println("wifi: Connecting...");

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.print("wifi: Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());
}

void sendDataToServer(float temperature, float humidity, float weight, int movement, int light, int sound)
{
    // Check WiFi connection status
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("Wifi: not connected");
        return;
    }

    WiFiClientSecure client;
    HTTPClient http;

    client.setInsecure();

    // Check if we can connect to the server
    if (!client.connect(serverName, 443))
    {
        Serial.println("Wifi: connection failed");
        return;
    }

    http.begin(client, serverName);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Authorization", token);

    // Construct the JSON string
    String httpRequestData = "{\"device_id\": " + String(device_id) +
                             ",\"temperature\": " + String(temperature) +
                             ",\"humidity\": " + String(humidity) +
                             ",\"weight\": " + String(weight) +
                             ",\"movement\": " + String(movement) +
                             ",\"light\": " + String(light) +
                             ",\"sound\": " + String(sound) + "}";

    // Send the HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);

    http.end();

    Serial.print("Wifi: HTTP response code: ");
    Serial.println(httpRequestData);
    Serial.print("Wifi: HTTP Request Data:");
    Serial.println(httpResponseCode);
}