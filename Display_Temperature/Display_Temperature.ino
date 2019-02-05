/***************************************************
  This is a library for the Adafruit 1.8" SPI display.

This library works with the Adafruit 1.8" TFT Breakout w/SD card
  ----> http://www.adafruit.com/products/358
The 1.8" TFT shield
  ----> https://www.adafruit.com/product/802
The 1.44" TFT breakout
  ----> https://www.adafruit.com/product/2088
as well as Adafruit raw 1.8" TFT display
  ----> http://www.adafruit.com/products/618

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>
#include <Wire.h>
#include <SI7021.h>
SI7021 sensor;
// For the breakout, you can use any 2 or 3 pins
// These pins will also work for the 1.8" TFT shield
#define TFT_CS     10
#define TFT_RST    9  // you can also connect this to the Arduino reset
                      // in which case, set this #define pin to 0!
#define TFT_DC     8

// Option 1 (recommended): must use the hardware SPI pins
// (for UNO thats sclk = 13 and sid = 11) and pin 10 must be
// an output. This is much faster - also required if you want
// to use the microSD card (see the image drawing example)
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);

// Option 2: use any pins but a little slower!
#define TFT_SCLK 13   // set these to be whatever pins you like!
#define TFT_MOSI 11   // set these to be whatever pins you like!
//Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);


float p = 3.1415926;
String temp1,hum1;
String Ser;
void setup(void) {
  Serial.begin(9600);

  // Use this initializer if you're using a 1.8" TFT
  //tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab

  // Use this initializer (uncomment) if you're using a 1.44" TFT
  tft.initR(INITR_144GREENTAB);   // initialize a ST7735S chip, black tab
  tft.cp437(true);
  // large block of text
  tft.fillScreen(ST7735_BLACK);
  tft.setCursor(12, 54);
  tft.setTextSize(3);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextWrap(true);
  tft.print(utf8rus("ПРИВЕТ"));
  
  tft.drawLine(123, 78, 5, 78, ST7735_RED);
  tft.drawLine(124, 79, 4, 79, ST7735_RED);
  tft.drawLine(125, 80, 3, 80, ST7735_RED);
  
  delay(10000);
    tft.setCursor(12, 54);
  tft.setTextSize(3);
  tft.setTextColor(ST7735_BLACK);
  tft.setTextWrap(true);
  tft.print(utf8rus("ПРИВЕТ"));

  tft.drawLine(123, 78, 5, 78, ST7735_BLACK);
  tft.drawLine(124, 79, 4, 79, ST7735_BLACK);
  tft.drawLine(125, 80, 3, 80, ST7735_BLACK);

}

void loop() {
  si7021_env data = sensor.getHumidityAndTemperature();
  temp1=data.celsiusHundredths-data.celsiusHundredths/100*100;
  hum1=data.humidityBasisPoints-data.humidityBasisPoints/100*100;

  
   tft.drawLine(26, 3, 3, 3, ST7735_RED); 
  tft.drawLine(26, 4, 4, 4, ST7735_RED); 
  tft.drawLine(26, 5, 5, 5, ST7735_RED); 

  tft.drawLine(125, 3, 100, 3, ST7735_RED); 
  tft.drawLine(124, 4, 100, 4, ST7735_RED); 
  tft.drawLine(123, 5, 100, 5, ST7735_RED);
  
  tft.drawLine(123, 56, 5, 56, ST7735_RED);
  tft.drawLine(124, 57, 4, 57, ST7735_RED);
  tft.drawLine(125, 58, 3, 58, ST7735_RED);


  tft.drawLine(31, 66, 3, 66, ST7735_RED); 
  tft.drawLine(31, 67, 4, 67, ST7735_RED); 
  tft.drawLine(31, 68, 5, 68, ST7735_RED); 

  tft.drawLine(125, 66, 96, 66, ST7735_RED); 
  tft.drawLine(124, 67, 96, 67, ST7735_RED); 
  tft.drawLine(123, 68, 96, 68, ST7735_RED);

  tft.drawLine(123, 120, 5, 120, ST7735_RED);
  tft.drawLine(124, 121, 4, 121, ST7735_RED);
  tft.drawLine(125, 122, 3, 122, ST7735_RED);

       
  tft.setCursor(30, 0);
  tft.setTextSize(1);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextWrap(true);
  //tft.print(data.celsiusHundredths/100);
  //tft.print(",");
  //tft.print(temp1);
  tft.print(utf8rus("ТЕМПЕРАТУРА"));

  tft.setCursor(37, 64);
  tft.setTextSize(1);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextWrap(true);
  //tft.print(data.celsiusHundredths/100);
  //tft.print(",");
  //tft.print(temp1);
  tft.print(utf8rus("ВЛАЖНОСТЬ"));

  tft.setCursor(2, 20);
  tft.setTextSize(3);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextWrap(true);
  tft.print(" ");
  tft.print(data.celsiusHundredths/100);
  tft.print(",");
  tft.print(temp1);

  tft.setCursor(2, 84);
  tft.setTextSize(3);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextWrap(true);
  tft.print(data.humidityBasisPoints/100);
  tft.print(",");
  tft.print(hum1);
  tft.print(" %");
  
  delay(10000);
  tft.setCursor(2, 20);
  tft.setTextSize(3);
  tft.setTextColor(ST7735_BLACK);
  tft.setTextWrap(true);
  tft.print(" ");
  tft.print(data.celsiusHundredths/100);
  tft.print(",");
  tft.print(temp1);

  tft.setCursor(2, 84);
  tft.setTextSize(3);
  tft.setTextColor(ST7735_BLACK);
  tft.setTextWrap(true);
  tft.print(data.humidityBasisPoints/100);
  tft.print(",");
  tft.print(hum1);
  tft.print(" %");
}

  String utf8rus(String source)
{
  int i,k;
  String target;
  unsigned char n;
  char m[2] = { '0', '\0' };
  k = source.length(); i = 0;
  while (i < k) {
    n = source[i]; i++;
    if (n >= 0xC0) {
      switch (n) {
        case 0xD0: {
          n = source[i]; i++;
          if (n == 0x81) { n = 0xA8; break; }
          if (n >= 0x90 && n <= 0xBF) n = n + 0x30;
          break;
        }
        case 0xD1: {
          n = source[i]; i++;
          if (n == 0x91) { n = 0xB8; break; }
          if (n >= 0x80 && n <= 0x8F) n = n + 0x70;
          break;
        }
      }
    }
    m[0] = n; target = target + String(m);
  }
return target;
}

