#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <BH1750.h>
#include <Adafruit_AHTX0.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
BH1750 lightMeter;
Adafruit_AHTX0 aht;
Adafruit_Sensor *aht_temp;//, *aht_humidity;

void setup() {

  Wire.begin(); // Initialize the I2C bus

  //SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!oled.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
   Serial.println(F("SSD1306 allocation failed"));
   while (true); // Don't proceed
  }

  oled.clearDisplay();
  delay(250);
  oled.setTextSize(2);
  oled.setCursor(0, 12);
  oled.println(" Luximetro");
  oled.setCursor(0, 40);
  oled.println("  Digital");
  oled.display();
  delay(2000); // Pause

  lightMeter.begin();

  if (!aht.begin()) {
    Serial.println("Failed to find AHT10/AHT20 chip");
    while (true); // Don't proceed
  }

  aht_temp = aht.getTemperatureSensor();
  aht_temp -> printSensorDetails();
  // aht_humidity = aht.getHumiditySensor();
  // aht_humidity -> printSensorDetails();

}

void loop() {

  sensors_event_t temp;
  aht_temp -> getEvent(&temp);
  //aht_humidity -> getEvent(&humidity);

  float v = lightMeter.readLightLevel();
  String light = String(int(round(v))) + " Lux";

  String temp = readTemperature();

  oledDisplayLeft(light, temp, 2);

  delay(700);

}

String readTemperature() {
  double param, fractpart, intpart;
  param = temp.temperature;
  fractpart = modf (param , &intpart);
  return String(int(intpart)) + "." + String(int(floor(fractpart * 10))) + char(247) + "C";
}

void oledDisplayLeft(String tx1, String tx2, int size) {
  oled.clearDisplay();

  int16_t x1, y1;
  uint16_t width, height;
  oled.setTextSize(1);
  oled.getTextBounds("Luminosidade", 0, 0, &x1, &y1, &width, &height);
  int xln1 = (SCREEN_WIDTH - width) / 2;
  oled.getTextBounds("Temperatura", 0, 0, &x1, &y1, &width, &height);
  int xln3 = (SCREEN_WIDTH - width) / 2;
  oled.setTextSize(2);
  oled.getTextBounds(tx1, 0, 0, &x1, &y1, &width, &height);
  int xln2 = (SCREEN_WIDTH - width) / 2;
  oled.getTextBounds(tx2, 0, 0, &x1, &y1, &width, &height);
  int xln4 = (SCREEN_WIDTH - width) / 2;

  //Luminosidade
  oled.setTextSize(1,0);
  oled.setCursor(xln1, 0);
  oled.print("Luminosidade");
  oled.setTextSize(size);
  oled.setCursor(xln2, 13);
  oled.println(tx1);

  //Temperatura
  oled.setTextSize(1,0);
  oled.setCursor(xln3, 37);
  oled.println("Temperatura");
  oled.setTextSize(size);
  oled.setCursor(xln4, 49);
  oled.println(tx2);
  
  oled.display();
}
