#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSans9pt7b.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);

  Wire.begin(5, 4);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false, false)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
}

void loop() {
  display.clearDisplay();
  display.setFont(&FreeSans9pt7b);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.println("");
  display.print("Sound:"); display.println("Test"); 
  display.print("Weight:"); display.println("Test");
  display.display();
  delay(300000);
}