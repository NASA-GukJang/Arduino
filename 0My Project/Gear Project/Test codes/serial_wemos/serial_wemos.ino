#include <SoftwareSerial.h>   //using software serial so as not to conflict with serial download
SoftwareSerial mySerial(D2,D3); // RX, TX

int potpinValue = 0;
int testvalue = 0;

void setup() {

  mySerial.begin(9600); //setup software serial
  Serial.begin(9600);    //setup serial monitor
}

void loop()  {

  while (mySerial.available() == 0) { }
  testvalue = mySerial.parseInt();
  potpinValue =mySerial.parseInt();

//print received values to serial monitor
  Serial.print(testvalue);


  delay(100);
}
