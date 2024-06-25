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

bool setupDisplay() {
  Wire.begin(DISPLAY_DATA_PIN, DISPLAY_CLOCK_PIN);

  if (display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_ADDRESS)) {
    Serial.println("Display: Module initialized.");
    is_display_initialized = true;
  } else {
    Serial.println("Display: Could not initialize module.");
    return false;
  }

  display.clearDisplay();
  display.setFont(&FreeSans9pt7b);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, (DISPLAY_SCREEN_HEIGHT / 2));
  display.println("Booting up...");
  display.display();

  return is_display_initialized;
}

void displayValues(float weight) {
  if (!is_display_initialized)
    return;

  display.clearDisplay();
  display.setCursor(0, (DISPLAY_SCREEN_HEIGHT / 2));
  if (weight == 0){
    display.println("No bird :(");
  } else {
    display.print("Weight:");
    display.println(weight);
  }
  display.display();
}