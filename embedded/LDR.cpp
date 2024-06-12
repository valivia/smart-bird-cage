#include <Arduino.h>

#define LDR_PIN 35
#define LDR_CALIBRATION_FACTOR 4.45
#define LDR_POLLING_INTERVAL 1000

int LDR_sum_lux = 0;
int LDR_measurement_count = 0;
unsigned long last_LDR_measurement = 0;

void setupLDR()
{
    pinMode(LDR_PIN, INPUT);
    Serial.println("LDR: Sensor initialized.");
}

void runLDRLoop()
{
    // Check if it is time to read the sensor
    if (millis() - last_LDR_measurement < LDR_POLLING_INTERVA)
        return;

    int voltage = analogRead(LDR_PIN);
    int lux = voltage * LDR_CALIBRATION_FACTOR;
    LDR_sum_lux += lux;
    LDR_measurement_count++;
}

int getLDRValue()
{
    int average = LDR_sum_lux / LDR_measurement_count;

    LDR_sum_lux = 0;
    LDR_measurement_count = 0;

    return average;
}