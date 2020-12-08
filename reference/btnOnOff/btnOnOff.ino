#include "Keyboard.h" 

const int joystick_x = A0;    
const int joystick_y = A1;
const int joystick_z = 8;    //조이스틱 X,Y,Z축을 저장

const int joystick_A = 2;
const int joystick_B = 3;
const int joystick_C = 4;
const int joystick_D = 5;

const int joystick_E = 6;
const int joystick_F = 7;

 
 const int ledPin = 13 ;

 boolean mouseIsActive = false;    // whether or not to control the mouse
 int lastSwitchState = LOW;        // previous switch state

void setup() {
  
  pinMode ( joystick_E, INPUT );
  pinMode ( ledPin, OUTPUT );
  Keyboard.begin();
}

void loop() {
  int switchState = digitalRead ( joystick_E );
  if (switchState != lastSwitchState) {
    if (switchState == HIGH) {
      mouseIsActive = !mouseIsActive;
      digitalWrite(ledPin, mouseIsActive);
    }
  }
  lastSwitchState = switchState;

  if ( mouseIsActive ) {                      // 조이스틱
      int x_Read = analogRead(joystick_x) ;
      if ( x_Read < 150 ) {
      Keyboard.press( KEY_LEFT_ARROW );
      }
      if ( x_Read > 950 ) {
      Keyboard.press( KEY_RIGHT_ARROW );
      }

      int y_Read = analogRead(joystick_y) ;
      if ( y_Read < 150 ) {
      Keyboard.press( KEY_DOWN_ARROW );
      }
      if ( y_Read > 950 ) {
      Keyboard.press( KEY_UP_ARROW );
      }   
  }
  if ( mouseIsActive ) {                      // 버튼
    
      int A_Read = digitalRead(joystick_A) ;
      if ( A_Read == LOW ) {
      Keyboard.press( 0x41 ); //A
      }
      
      int B_Read = digitalRead(joystick_B) ;
      if ( B_Read == LOW ) {
      Keyboard.press( 0x42 ); //s
      }
      
      int C_Read = digitalRead(joystick_C) ;
      if ( C_Read == LOW ) {
      Keyboard.press( 0x43 ); //w
      }

      int D_Read = digitalRead(joystick_D) ;
      if ( D_Read == LOW ) {
      Keyboard.press( 0x44 ); //d
      }
   }
  delay ( 30 );
  Keyboard.releaseAll();
}
