#include <SoftwareSerial.h>   //We are using software serial so as not to conflict with serial download and monitor
SoftwareSerial mySerial(2,3); // RX, TX

int testvalue = 200;

void setup() {

  mySerial.begin(9600);
}

void loop()  {
  Serial.print(testvalue);
 mySerial.println(testvalue);
 delay(200);
}
