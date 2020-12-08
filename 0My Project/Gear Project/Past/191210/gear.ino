#include <SoftwareSerial.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

SoftwareSerial modeSerial(4,5); // RX, TX

int encoderPin1 = 2;  // left
int encoderPin2 = 3;  // right
int aState;
int aLastState; 
int component = 0;   

int PMode = 0; // Program Mode
int selectorPin1 = 0;
int selectorPin2 = 1;

void setup() {
  Serial.begin(9600);
  modeSerial.begin(9600);

  pinMode(encoderPin1, INPUT);
  pinMode(encoderPin2, INPUT);

  aLastState = digitalRead(encoderPin1);

  pinMode(selectorPin1, INPUT_PULLUP);
  pinMode(selectorPin2, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(selectorPin1), selectProgram, CHANGE);
  attachInterrupt(digitalPinToInterrupt(selectorPin2), selectProgram, CHANGE);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
}

void loop() {
  //selectProgram();
  selectComponent();                           // Right, Left Check
  
  //testdrawstyles();
}

void selectProgram(){
  int selectorStatus1 = digitalRead(selectorPin1);
  int selectorStatus2 = digitalRead(selectorPin2);

  if(selectorStatus1 == 1 && selectorStatus2 == 0){
    PMode = 1;
    Serial.println("Photoshop Mode");
    modeSerial.println(PMode);
  }
  else if(selectorStatus1 == 1 && selectorStatus2 == 1){
    PMode = 2;
    Serial.println("Lightroom Mode");
    modeSerial.println(PMode);
  }
  else{ //selectorStatus1 == 0 && selectorStatus2 == 1
    PMode = 3;
    Serial.println("AfterEffect Mode");
    modeSerial.println(PMode);
  }
  delay(500);
}

/* void selectComponent() {
  int left = digitalRead(encoderPin1);
  int right = digitalRead(encoderPin2);
  int encoded = (left << 1) | right; 
  int sum  = (lastEncoded << 2) | encoded; 

  if(sum == 0b1101 || sum == 0b1011) {       // Left 동작 확인
    Serial.println("Left");
    component--;
    if(component < -40) component = 0;
    display_Component();
    Serial.println(component);
  }
  if(sum == 0b1110 || sum == 0b1000) {       // Right 동작 확인
    Serial.println("Right");
    component++;
    if(component > 40) component = 1;
    display_Component();
    Serial.println(component);
  }
  lastEncoded = encoded;
} */

void selectComponent() {
  aState = digitalRead(encoderPin1); // Reads the "current" state of the outputA
   // If the previous and the current state of the outputA are different, that means a Pulse has occured
   if (aState != aLastState){     
     // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
     if (digitalRead(encoderPin2) != aState) { 
       component ++;
       if(component > 40) component = 1;
     } 
     else {
       component --;
       if(component < -40) component = 0;
     }
     display_Component();
     Serial.println(component);
   } 
   aLastState = aState; // Updates the previous state of the outputA with the current state
}

void display_Component(void) {  
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,14); 

  if(component >= 0 && component < 10 || component >= -40 && component < -30) display.println(F("Brightness"));
  else if(component >= 10 && component < 20 || component >= -30 && component < -20) display.println(F("Opacity"));
  else if(component >= 20 && component < 30 || component >= -20 && component < -10) display.println(F("Other 1"));
  else if(component >= 30 && component < 40 || component >= -10 && component < 0) display.println(F("Other 2"));

  display.display();
}

void testdrawstyles(void) {
  display.clearDisplay();

  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0,14);             // Start at top-left corner
  display.println(F("Hello"));

  display.display();
  delay(2000);
}
