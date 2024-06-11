#define RADAR_RX_PIN 27
#define RADAR_TX_PIN 26

#include <ld2410.h>

ld2410 radar;

int energySum = 0;
int movementCheckCounter = 0;
// float energyAvg = 0;

void setupRadar(void) {
  Serial.begin(115200);                                                //Feedback over Serial Monitor
  Serial.begin(256000, SERIAL_8N1, RADAR_RX_PIN, RADAR_TX_PIN);  //UART for monitoring the radar
  delay(500);
  if (radar.begin(Serial)) {
    Serial.println(F("OK"));
    Serial.print(F("LD2410 firmware version: "));
    Serial.print(radar.firmware_major_version);
    Serial.print('.');
    Serial.print(radar.firmware_minor_version);
    Serial.print('.');
    Serial.println(radar.firmware_bugfix_version, HEX);
  } else {
    Serial.println(F("not connected"));
  }
}


void movementCheck() {
  radar.read();
  if (radar.movingTargetDetected()) {
      Serial.print(F("energy:"));
      Serial.println(radar.movingTargetEnergy());
      energySum = energySum + radar.movingTargetEnergy();
    }
  else {
      Serial.println(F("No target"));
      energySum = energySum + 0;
    }
  movementCheckCounter++;
}
