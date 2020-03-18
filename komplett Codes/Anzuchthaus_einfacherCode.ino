#include "RTClib.h"

RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//Variables

//pins
int water1_pin = 30;
int water2_pin = 31;
int water3_pin = 32;
int light1_pin = 33;
int light2_pin = 34;



void setup() {

  Serial.begin(9600);
  Serial.print("starting...")

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
     while (1);
  } //if(!rtc.begin)

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  } //if rtc(lostpower)

  //pinModes
  pinMode(water1_pin, OUTPUT);
  pinMode(water2_pin, OUTPUT);
  pinMode(water3_pin, OUTPUT);
  pinMode(light1_pin, OUTPUT);
  pinMode(light2_pin, OUTPUT);
} //setup



void loop() {

  DateTime now = rtc.now();


} //loop
