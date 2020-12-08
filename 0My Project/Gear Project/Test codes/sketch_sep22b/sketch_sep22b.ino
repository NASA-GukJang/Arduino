int encoderPin1 = 6;                          // Rotary encoder module의 CLK 제어핀 설정
int encoderPin2 = 7;                          // Rotary encoder module의 DT 제어핀 설정
                    // Rotary encoder module의 SW 제어핀 설정
volatile int lastEncoded = 0;           
volatile long encoderValue = 0;         

void setup() {
  Serial.begin(9600);                         // 시리얼 모니터 시작(Baud Rate 9600)
  pinMode(encoderPin1, INPUT_PULLUP);                // Rotary encoder module의 CLK 제어핀 입력 설정
  pinMode(encoderPin2, INPUT_PULLUP);                // Rotary encoder module의 DT 제어핀 입력 설정

  digitalWrite(encoderPin1, HIGH);            // Turn pullup resistor on
  digitalWrite(encoderPin2, HIGH);            // Turn pullup resistor on

  delay(500);
}

void loop() {
  updateEncoder();                           // Right, Left Check
  delay(1);                                  // 1ms 스캔 주기
}

void updateEncoder() {
  int CLK = digitalRead(encoderPin1);        // Rotary encoder module의 CLK Read(GPIO State)
  int DT = digitalRead(encoderPin2);         // Rotary encoder module의 DT Read(GPIO State)
  int encoded = (CLK << 1) | DT; 
  int sum  = (lastEncoded << 2) | encoded; 

  if(sum == 0b1101 || sum == 0b1011) {       // Left 동작 확인
    Serial.println("Left");                  // "Left" 시리얼 출력
  }
  if(sum == 0b1110 || sum == 0b1000) {       // Right 동작 확인
    Serial.println("Right");                 // "Right" 시리얼 출력
  }
  lastEncoded = encoded;
}
