#include <TFT_eSPI.h>
#include "icon.h" //(Bitmap generated with LCD Image Converter) https://sourceforge.net/projects/lcd-image-converter/
#include <SoftwareSerial.h>   //using software serial so as not to conflict with serial download

TFT_eSPI tft = TFT_eSPI();   // Invoke library

SoftwareSerial modeSerial(D2,D3); // RX, TX

void setup(void) {
  Serial.begin(9600);
  modeSerial.begin(9600);

  tft.begin();     // initialize a ST7789 chip
  tft.setSwapBytes(true); // Swap the byte order for pushImage() - corrects endianness

  tft.fillScreen(TFT_BLACK);
}

void loop() {
  while (modeSerial.available() == 0) { }
  int PMode = modeSerial.parseInt();

  if(PMode == 1) tft.pushImage(0,0,240,240,photoshop);
  else if(PMode == 2) tft.pushImage(0,0,240,240,lightroom);
  else if(PMode == 3) tft.pushImage(0,0,240,240,aftereffect);

//print received values to serial monitor
  Serial.print(PMode);
}
