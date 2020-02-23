// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include "RTClib.h"

RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//Variables:
int input = 0;
int ledPin = 13;
int futureTime;



void setup () {


    pinMode(13, OUTPUT);
    Serial.begin(9600);
    delay(3000); // wait for console opening

    if (! rtc.begin()) {
      Serial.println("Couldn't find RTC");
       while (1);
    }


}

void loop () {
    DateTime now = rtc.now();

    input = Serial.read();

    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();


    if (input > 0) {

      DateTime futureTime (now + TimeSpan(0,0,0,input));
      
      
      Serial.print(futureTime.second(), DEC);
      Serial.println();      
      
      input = 0;
    }

   
    delay(1000);
}
