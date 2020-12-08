int center_x;
int center_y;
int deadzone = 1;

void setup() {
  center_x = analogRead(A0);
  center_y = analogRead(A1);
}

void loop() {
  int joystickValue_x = readStick(A0, center_x);
  int joystickValue_y = readStick(A1, center_y);
}

int readStick(int pinNumber, int center) {
  int rawValue = analogRead(pinNumber);
  int adjustedValue = 0;
  if(rawValue >= center + deadzone) {
    adjustedValue = map(rawValue, center + deadzone, 1023, 0, 255);
  }
  else if(rawValue <= center - deadzone) {
    adjustedValue = map(rawValue, 0, center - deadzone, -255, 0);
  }    
  return adjustedValue;
}
