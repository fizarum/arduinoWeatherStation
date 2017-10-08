#include <dht.h>
#include <Math.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>
#include <Adafruit_ILI9341.h>
#include <SPI.h>
#include <Fonts/FreeMonoBoldOblique12pt7b.h>

//bmp sensor
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

//bmp sensor
#define BMP_SCK 19
#define BMP_MISO 18


#define TFT_DC 9
#define TFT_CS 10

//humidity sensor
#define DHT_PIN 2

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
Adafruit_BMP280 bme;

dht DHT;

double currentTemperature = NAN;
double currentHumidity = NAN;

const char * const tempString = "Temperature: ";
const char * const humidString = "Humidity: ";

const double temperature_normal = 20.0;
const double temperature_high = 27.0;

const int padding = 10;

boolean isBMPReady = false;

/*
baseline - y coord of starting writing text (baseline, bottom line of text)
*/
void printTextWithValue(char *label, int baseline, double value, uint16_t textColor = ILI9341_WHITE);

void setup() {
  Serial.begin(9600);
  tft.begin();
  tft.setRotation(2);
  tft.setFont(&FreeMonoBoldOblique12pt7b);
  tft.setTextSize(1);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextWrap(true);
  tft.fillScreen(ILI9341_BLACK);

  isBMPReady = bme.begin();
}

void loop() {
  int chk = DHT.read11(DHT_PIN);
  printTemperature(DHT.temperature);
  printHumidity(DHT.humidity);
  if(isBMPReady) {
    printTextWithValue("temp2: ", 80, bme.readTemperature());
    printTextWithValue("press: ", 110, bme.readPressure());
    //printAlt(bme.readAltitude(1013.25));
  }
  delay(7000);
}

void printHumidity(double value) {
  if(currentHumidity != NAN && currentHumidity == value) {
    return;
  }
  currentHumidity = value;

  printTextWithValue(humidString, 50, value);
}

void printTemperature(double value) {
  if(currentTemperature != NAN && currentTemperature == value) {
    return;
  }

  currentTemperature = value;
  uint16_t textColor;
  if(currentTemperature < temperature_normal) {
    textColor = ILI9341_BLUE;
  } else if(currentTemperature >= temperature_normal && currentTemperature < temperature_high) {
    textColor = ILI9341_YELLOW;
  } else {
    textColor = ILI9341_RED;
  }
  printTextWithValue(tempString, 20, value, textColor);
}


/*
baseline - y coord of starting writing text (baseline, bottom line of text)
*/
void printTextWithValue(char *label, int baseline, double value, uint16_t textColor = ILI9341_WHITE) {
  int16_t  x1, y1;
  uint16_t w, h;

  tft.getTextBounds(label, 0, 0, &x1, &y1, &w, &h);
  tft.setCursor(0, baseline);
  tft.setTextColor(ILI9341_WHITE);
  tft.print((char *)label);
  tft.setCursor(x1 + w, baseline);
  tft.fillRect(x1 + w, baseline - h, 200, h, ILI9341_BLACK);
  tft.setTextColor(textColor);
  tft.print(value);
  
}

