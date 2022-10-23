#include <Arduino.h>
#include <SPI.h>
#include "U8glib.h"
#include <DHT.h>
#include "SdFat.h"
#include "sdios.h"
#include <TMRpcm.h>

#define chipSelect 10
#define OLED_RESET -1

#define controlPot 0
#define devButton 8
#define DHTPIN 7
#define DHTTYPE DHT11
#define SmokeInput 0

U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_DEV_0|U8G_I2C_OPT_FAST);
DHT dht(DHTPIN, DHTTYPE);
TMRpcm audio;
SdFat sd;

void draw(void) {
  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_unifont);
  //u8g.setFont(u8g_font_osb21);
  u8g.drawStr( 0, 22, "Hello World!");
}


void setup()
{
  Serial.begin(9600);
  audio.speakerPin = 9;
  
  dht.begin();
    
  if (!sd.begin(53, SPI_FULL_SPEED)) { return; 
  }else{ Serial.println("SD OK"); }
    
  audio.play("song.wav");
  
  // pinModes
  pinMode(controlPot, INPUT);
  pinMode(devButton, INPUT);
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

  String data = String(temperature) + "-" + String(humidity) + "-" + String(smoke);
  Serial.println(data);

   // picture loop
  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );
}
