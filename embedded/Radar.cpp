#include <ld2410.h>

#define RADAR_RX_PIN 27
#define RADAR_TX_PIN 26
#define RadarSerial Serial1

ld2410 radar;
int energySum = 0;
int movementCheckCounter = 0;

void setupRadar(void) {
  RadarSerial.begin(256000, SERIAL_8N1, RADAR_RX_PIN, RADAR_TX_PIN);
  if (radar.begin(RadarSerial)) {
    Serial.println("Radar connected.");
  } else {
    Serial.println("Radar couldnt connect.");
  }
}

void runRadarLoop() {
  radar.read();
  if (radar.movingTargetDetected()) {
    energySum = energySum + radar.movingTargetEnergy();
  }
  movementCheckCounter++;
}

int getRadarScore() {
  int average = energySum / movementCheckCounter;
  energySum = 0;
  movementCheckCounter = 0;
  return average;
}
