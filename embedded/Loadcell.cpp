#include "HX711.h"
#define DATA_PIN 19
#define CLOCK_PIN 21

// NOTE not done, unreliable method
HX711 scale;
float measuredWeight = -1.0;
float weight = -1.0;

void setupLoadcell()
{
    scale.begin(DATA_PIN, CLOCK_PIN);
    scale.calibrate_scale(1000, 5);
    Serial.println("Loadcell: Sensor initialized.");
}

void runLoadcellLoop()
{
    measuredWeight = scale.get_units(5);
    if (measuredWeight > 10)
    {
        weight = measuredWeight;
    }
}

float getLoadcellValue()
{
    return weight;
}