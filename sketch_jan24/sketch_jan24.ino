#include <SoftwareSerial.h>
SoftwareSerial mySerial(19, 18); // RX, TX

char val;
int ledpin = 13;

void setup() {
  pinMode(ledpin, OUTPUT);
  Serial.begin(9600);
}
void loop() {
  if( Serial.available() )
  {
    val = Serial.read();
  }
  if( val == 'H' ){
    digitalWrite(ledpin, HIGH);
  } else if ( val == 'L' ){
    digitalWrite(ledpin, LOW);
  } else {
    Serial.print("~");
  }
}
