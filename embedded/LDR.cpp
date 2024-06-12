#include <Arduino.h>

#define LDR_PIN 35
#define LDR_CALIBRATION_FACTOR 4.45
#define LDR_POLLING_INTERVAL 1000

int LDR_sum_lux = 0;
int LDR_measurement_count = 0;
unsigned long last_LDR_measurement = 0;

bool setupLDR()
{
    pinMode(LDR_PIN, INPUT);
    Serial.println("LDR: Sensor initialized.");
    // NOTE, maybe add a check for a specific voltage that'd imply its broken
    return true;
}

void runLDRLoop()
{
    // Check if it is time to read the sensor
    if (millis() - last_LDR_measurement < LDR_POLLING_INTERVAL)
        return;

    int voltage = analogRead(LDR_PIN);
    // NOTE, is this actually accurate?
    int lux = voltage * LDR_CALIBRATION_FACTOR;
    LDR_sum_lux += lux;
    LDR_measurement_count++;
    last_LDR_measurement = millis();
}

int getLDRValue()
{
    int average = LDR_sum_lux / LDR_measurement_count;

    LDR_sum_lux = 0;
    LDR_measurement_count = 0;

    return average;
}