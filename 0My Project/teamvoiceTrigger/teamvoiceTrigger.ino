#include <Keyboard.h>

const int joystick_A = 2;

void setup() {
  Serial.begin(115200);
  Keyboard.begin();
}

void loop() {
      int A_Read = digitalRead(joystick_A) ;
      if ( A_Read == LOW ) {
      Keyboard.press( 't' ); //TTTTTTTTTTTTTTTTTTTT
      }
      delay ( 100 );
      Keyboard.releaseAll(); 
  }
