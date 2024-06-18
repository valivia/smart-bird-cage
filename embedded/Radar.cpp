#include <ld2410.h>
#define RADAR_RX_PIN 17
#define RADAR_TX_PIN 16
#define RADAR_SERIAL Serial2

#define RADAR_POLLING_INTERVAL 1000

static ld2410 radar;
static int radar_total_score = 0;
static int radar_measurement_count = 0;
static unsigned long radar_last_measurement = 0;

static bool is_radar_initialized = false;

bool setupRadar(void) {
  RADAR_SERIAL.begin(256000, SERIAL_8N1, RADAR_RX_PIN, RADAR_TX_PIN);
  // delay(500);
  if (radar.begin(RADAR_SERIAL)) {
    Serial.println("Radar: Sensor initialized.");
    is_radar_initialized = true;
  } else {
    Serial.println("Radar: Failed to initialize sensor.");
  }

  return is_radar_initialized;
}

void runRadarLoop() {
  if (!is_radar_initialized)
    return;
  radar.read();

  // Check if it is time to read the sensor
  if (!radar.isConnected() || millis() - radar_last_measurement < RADAR_POLLING_INTERVAL)
    return;

  radar_last_measurement = millis();

  // if (radar.presenceDetected()) {
  //   // if (radar.stationaryTargetDetected()) {
  //   //   Serial.print(F("Stationary target: "));
  //   //   Serial.print(radar.stationaryTargetDistance());
  //   //   Serial.print(F("cm energy:"));
  //   //   Serial.print(radar.stationaryTargetEnergy());
  //   //   Serial.print(' ');
  //   // }
  //   if (radar.movingTargetDetected()) {
  //     Serial.print(F("Moving target: "));
  //     Serial.print(radar.movingTargetDistance());
  //     Serial.print(F("cm energy:"));
  //     Serial.println(radar.movingTargetEnergy());
  //   }
  // //   Serial.println();
  // } else {
  //   Serial.println(F("No target"));
  // }


  int radar_score = radar.movingTargetEnergy();
  if (radar_score) {
    radar_total_score = radar_total_score + radar_score;
  }

  Serial.println(radar_total_score);

  radar_measurement_count++;
}

int getRadarValue() {
  if (radar_measurement_count == 0)
    return 0;

  int average = radar_total_score / radar_measurement_count;

  radar_total_score = 0;
  radar_measurement_count = 0;
  return average;
}
