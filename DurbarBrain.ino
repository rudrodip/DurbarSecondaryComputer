#include <Arduino.h>
#include <SPI.h>
#include "U8glib.h"
#include <DHT.h>
#include "SdFat.h"
#include "sdios.h"
#include <TMRpcm.h>

#define chipSelect 10
#define OLED_RESET -1

#define controlPot 1
#define devButton 8
#define DHTPIN 7
#define DHTTYPE DHT11
#define SmokeInput 0

U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE);
DHT dht(DHTPIN, DHTTYPE);
TMRpcm audio;
SdFat sd;

void HumMeter(float* humPerc, float* temp, int* smoke)
{
  u8g.setFont(u8g_font_fub11);
  u8g.setFontRefHeightExtendedText();
  u8g.setDefaultForegroundColor();
  u8g.setFontPosTop();
  
  u8g.drawStr(4, 0, "Hum %");
  u8g.setPrintPos(80, 0);
  u8g.print(*humPerc);
  
  u8g.drawStr(4, 20, "Temp C");
  u8g.setPrintPos(80, 20);
  u8g.print(*temp);

  u8g.drawStr(4, 40, "Smoke% ");
  u8g.setPrintPos(80, 40);
  u8g.print(*smoke);
}


void setup()
{
  Serial.begin(9600);
  audio.speakerPin = 9;
  
  dht.begin();
    
  if (!sd.begin(chipSelect, SPI_FULL_SPEED)) { 
    Serial.println("sd problem");
    return; 
  }
  else{ Serial.println("SD OK"); }
    
  audio.play("song.wav");
  
  // pinModes
  pinMode(controlPot, INPUT);
  pinMode(devButton, INPUT_PULLUP);
  pinMode(SmokeInput, INPUT);

  // assign default color value
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }
  
  pinMode(8, OUTPUT);
}

void loop()
{
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  int smoke = analogRead(SmokeInput);
  int control = analogRead(controlPot);

  String data = "t" + String(temperature) + "-" + String(humidity) + "-" + String(smoke);
  Serial.println(data);
  Serial.println(control);
   // picture loop
  u8g.firstPage();
      do
      {
        HumMeter(&humidity, &temperature, &smoke);
      }
      while (u8g.nextPage());
  delay(100);
}
