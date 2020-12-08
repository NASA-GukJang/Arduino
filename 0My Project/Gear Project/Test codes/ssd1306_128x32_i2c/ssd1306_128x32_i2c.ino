/*
 * IIC
 * SDA -- A4
 * SCL -- A5
 * 
 */
#include <U8glib.h>
U8GLIB_SSD1306_128X32 u8g(U8G_I2C_OPT_NONE);  // I2C / TWI
void setup(void) {
}

void loop(void) {
  u8g.setFont(u8g_font_unifont);
  u8g.setPrintPos(0, 15);
  u8g.print("0.91 OELD");
  u8g.setPrintPos(0,30);
  u8g.print("SmartPrototyping");
  delay(500);
}
