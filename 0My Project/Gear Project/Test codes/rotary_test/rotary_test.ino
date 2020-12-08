#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int encoderPin1 = 6;  // left
int encoderPin2 = 7;  // right
volatile int lastEncoded = 0;           
volatile long encoderValue = 0;   
int component = 0;      

void setup() {
  Serial.begin(9600);
  pinMode(encoderPin1, INPUT_PULLUP);
  pinMode(encoderPin2, INPUT_PULLUP);
  digitalWrite(encoderPin1, HIGH);
  digitalWrite(encoderPin2, HIGH);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  
  delay(500);
}

void loop() {
  selectComponent();                           // Right, Left Check
  delay(1);                                  // 1ms 스캔 주기
}

void selectComponent() {
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
