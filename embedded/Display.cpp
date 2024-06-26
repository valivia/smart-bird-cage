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

Adafruit_SSD1306 display(DISPLAY_SCREEN_WIDTH, DISPLAY_SCREEN_HEIGHT, &Wire, -1);

static bool is_display_initialized = false;

float display_weight = -1.0;
int display_light = -1.0;
float display_temperature = -1.0;
bool has_changed = false;

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
  if (!has_changed)
    return;

  display.clearDisplay();
  display.setCursor(0, 6);
  display.println("Weight: " + String(display_weight) + "g");

  display.setCursor(0, 16);
  display.println("Light: " + String(display_light) + "lux");

  display.setCursor(0, 26);
  display.println("Temperature: " + String(display_temperature) + "C");

  display.display();
  has_changed = false;
}

void setWeight(float weight) {
  has_changed = weight != display_weight;
  display_weight = weight;
}

void setLight(int light) {
  has_changed = light != display_light;
  display_light = light;
}

void setTemperature(float temperature) {
  has_changed = temperature != display_temperature;
  display_temperature = temperature;
}