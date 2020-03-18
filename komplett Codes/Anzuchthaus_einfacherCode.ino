#include "RTClib.h"

RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//Variables
int water_hour = 17;
int water_OnTime[] = {1000, 2000, 3000}; //on time in ms
int light_time_on = 17;
int light_time_off = 6;

bool water_rem = 0;
bool light_rem = 0;

//pins
int water_pin[] = {30, 31, 32};
int light_pin = 33;
int pump_pin = 34;
int suply_pin = 35; //12V power suply


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
  pinMode(water_pin[1], OUTPUT);
  pinMode(water_pin[2], OUTPUT);
  pinMode(water_pin[3], OUTPUT);
  pinMode(light_pin, OUTPUT);
  pinMode(pump_pin, OUTPUT);
  pinMode(suply_pin, OUTPUT);
} //setup



void loop() {

  DateTime now = rtc.now();

  //water
  if (now.hour() == water_hour && water_rem == 0) {
    digitalWrite(suply_pin, HIGH); //activate 12V power suply
    delay(1000);
    digitalWrite(pump_pin, HIGH); //activate pump
    for (int i = 0; i <= 3; i++) {
      digitalWrite(water_pin[i], HIGH);
      delay(water_OnTime[i]);
      digitalWrite(water_pin[i], LOW);
    }
    water_rem++;
    digitalWrite(pump_pin, LOW); //deactivare pump
    digitalWrite(suply_pin, LOW); //deactivate 12V power suply
    delay(1000);

  } //water

  //reset water_rem
  if (now.hour() == water_hour + 2) {
    water_rem--;
  } //reset water_rem



  //light on
  if (now.hour() == light_time_on) {
    digitalWrite(light_pin, HIGH);
  } //light on

  //light off
  if (now.hour() == light_time_off) {
    digitalWrite(light_pin, LOW);
  } //light off

} //loop
