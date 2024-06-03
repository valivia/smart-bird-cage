#include <Arduino.h>
int sensor = 18;              // the pin that the sensor is atteched to
int state = LOW;             // by default, no motion detected
int val = 0;                 // variable to store the sensor status (value)
int movements = 0;

void setupPIR() {
  pinMode(sensor, INPUT);    // initialize sensor as an input
}

int returnMovements(){
  return movements;
}

void checkMovement(){
  // unsigned long currentMillis = millis();
  
  val = digitalRead(sensor);   // read sensor value
  if (val == HIGH) {           // check if the sensor is HIGH
    // delay(100);                // delay 100 milliseconds 
    
    if (state == LOW) {
      movements = movements + 1;
      state = HIGH;       // update variable state to HIGH
    }
  } 
  else {
      // delay(200);             // delay 200 milliseconds 
      
      if (state == HIGH){
        state = LOW;       // update variable state to LOW
    }
  }
}