#ifndef PIR_MEASUREMENT_H
#define PIR_MEASUREMENT_H

#include <Arduino.h>
// Define global variables for temperature and humidity
extern float movements;

// Function prototypes
void setupPIR();
void checkMovement();
int returnMovements();

#endif // PIR_MEASUREMENT_H
