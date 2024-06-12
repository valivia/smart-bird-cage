#include <ld2410.h>

#define RADAR_RX_PIN 27
#define RADAR_TX_PIN 26
#define RADAR_SERIAL Serial1

#define RADAR_POLLING_INTERVAL 1000

ld2410 radar;
int radar_total_score = 0;
int radar_measurement_count = 0;
unsigned long radar_last_measurement = 0;

void setupRadar(void)
{
  RADAR_SERIAL.begin(256000, SERIAL_8N1, RADAR_RX_PIN, RADAR_TX_PIN);
  if (radar.begin(RADAR_SERIAL))
  {
    Serial.println("Radar connected.");
  }
  else
  {
    Serial.println("Radar couldnt connect.");
  }
}

void runRadarLoop()
{

  // Check if it is time to read the sensor
  if (millis() - radar_last_measurement < RADAR_POLLING_INTERVAL)
    return;

  radar_last_measurement = millis();

  radar.read();
  if (radar.movingTargetDetected())
  {
    radar_total_score = radar_total_score + radar.movingTargetEnergy();
  }

  radar_measurement_count++;
}

int getRadarValue()
{
  int average = radar_total_score / radar_measurement_count;

  // Serial.print("Radar: Score = ");
  // Serial.print(average);
  // Serial.print(" Sum = ");
  // Serial.print(radar_total_score);
  // Serial.print(" Counter = ");
  // Serial.println(radar_measurement_count);

  radar_total_score = 0;
  radar_measurement_count = 0;
  return average;
}
