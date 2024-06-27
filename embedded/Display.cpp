#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSans9pt7b.h>
// Needs 3.3V
#define DISPLAY_SCREEN_WIDTH 128
#define DISPLAY_SCREEN_HEIGHT 64
#define DISPLAY_ADDRESS 0x3C

#define DISPLAY_DATA_PIN 22
#define DISPLAY_CLOCK_PIN 21

#define DISPLAY_UPDATE_TIMEOUT 5000

Adafruit_SSD1306 display(DISPLAY_SCREEN_WIDTH, DISPLAY_SCREEN_HEIGHT, &Wire, -1);

static bool is_display_initialized = false;

float display_weight = 0.0;
int display_light = -1.0;
float display_temperature = -1.0;
bool display_connection_updating = false;
unsigned long display_last_update = 0;

bool setupDisplay() {
  Wire.begin(DISPLAY_DATA_PIN, DISPLAY_CLOCK_PIN);

  if (display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_ADDRESS)) {
    is_display_initialized = true;
  } else {
    return false;
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 6);
  display.println("Booting up...");
  display.display();

  return is_display_initialized;
}

void runDisplayLoop() {
  if (millis() - display_last_update > DISPLAY_UPDATE_TIMEOUT)
    return;

  // Turn off the display if the light is too low to protect the birds sleep
  if (display_light < 20) {
    display.clearDisplay();
    display.display();
    return;
  }

  display.clearDisplay();
  display.setCursor(0, 6);
  display.println("Weight: " + String(display_weight) + "g");

  display.setCursor(0, 16);
  display.println("Light: " + String(display_light) + "lux");

  display.setCursor(0, 26);
  display.println("Temperature: " + String(display_temperature) + "C");

  if (display_connection_updating) {
    display.setCursor(0, 36);
    display.println("Updating...");
  }

  display.display();
}

void setWeight(float weight) {
  if (weight == display_weight)
    return;

  display_last_update = millis();
  display_weight = weight;
}

void setLight(int light) {
  if (light == display_light)
    return;

  display_last_update = millis();
  display_light = light;
}

void setTemperature(float temperature) {
  if (temperature == display_temperature)
    return;

  display_last_update = millis();
  display_temperature = temperature;
}

void setUpdating(bool state) {
  if (state == display_connection_updating)
    return;

  display_last_update = millis();
  display_connection_updating = state;
}