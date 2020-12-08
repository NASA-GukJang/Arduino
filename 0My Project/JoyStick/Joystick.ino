#include "Keyboard.h" 

const int joystick_x = A2;    
const int joystick_y = A1;
const int joystick_z = 14;    //조이스틱 X,Y,Z축을 저장

const int joystick_A = 4;
const int joystick_B = 7;
const int joystick_C = 9;
const int joystick_D = 3;

const int btnONOFF = 15;
const int btnCoin = 6;

int center_x;
int center_y;
int deadzone = 1;

 
const int ledPin = 2;

boolean mouseIsActive = false;    // whether or not to control the mouse
int lastSwitchState = LOW;        // previous switch state

void setup() {
  
  pinMode ( btnONOFF, INPUT );
  pinMode ( ledPin, OUTPUT );

  center_x = analogRead(A2);
  center_y = analogRead(A1);

  Serial.begin(115200);
  Keyboard.begin();
}

void loop() {
  joystickOnOff();

  if ( mouseIsActive) {                      // 조이스틱
    
      int joystickValue_x = readStick(A2, center_x);
      Serial.print("x : ");
      Serial.println(joystickValue_x);
      
      if ( joystickValue_x < 0 ) {
      Keyboard.press( KEY_DOWN_ARROW );
      }
      if ( joystickValue_x > 0 ) {
      Keyboard.press( KEY_UP_ARROW );
      }

      int joystickValue_y = readStick(A1, center_y);
      Serial.print("y : ");
      Serial.println(joystickValue_y);
      
      if ( joystickValue_y < 0 ) {
      Keyboard.press( KEY_LEFT_ARROW );
      }
      if ( joystickValue_y > 0 ) {
      Keyboard.press( KEY_RIGHT_ARROW );
      }
      
      if ( mouseIsActive ) {                      // 버튼
    
      int A_Read = digitalRead(joystick_A) ;
      if ( A_Read == HIGH ) {
      Keyboard.press( 'D' ); //A
      }
      
      int B_Read = digitalRead(joystick_B) ;
      if ( B_Read == HIGH ) {
      Keyboard.press( 'S' ); //s
      }
      
      int C_Read = digitalRead(joystick_C) ;
      if ( C_Read == HIGH ) {
      Keyboard.press( 'A' ); //a
      }

      int D_Read = digitalRead(joystick_D) ;
      if ( D_Read == HIGH ) {
      Keyboard.press( 'A' ); //d
      }

      int btnCoin_Read = digitalRead(btnCoin) ;
      if ( btnCoin_Read == HIGH ) {
      Keyboard.press( '5' ); //5
      }
   }

      delay ( 30 );
      Keyboard.releaseAll(); 
  }
}

int readStick(int pinNumber, int center) {
  int rawValue = analogRead(pinNumber);
  int adjustedValue = 0;
  if(rawValue >= center + deadzone) {
    adjustedValue = map(rawValue, center + deadzone, 1023, 0, 100);
  }
  else if(rawValue <= center - deadzone) {
    adjustedValue = map(rawValue, 0, center - deadzone, -100, 0);
  }   
  return adjustedValue;
}

int joystickOnOff()
{
  int switchState = digitalRead ( btnONOFF );
  if (switchState != lastSwitchState) {
    if (switchState == HIGH) {
      mouseIsActive = !mouseIsActive;
      digitalWrite(ledPin, mouseIsActive);
      center_x = analogRead(A2);
      center_y = analogRead(A1);
    }
  }
  lastSwitchState = switchState;

  return mouseIsActive;
}
