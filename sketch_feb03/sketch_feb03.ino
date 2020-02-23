/*
POS 1
  slot: 
  1: 1
  (2-20)
  21: all
POS 2
  targets: 
  1: light
  2: water
  3: heater
  sensors: 
  4: temp(erature)
  5: humid(ity)
  6: sun(light)
  7: prox(imity)
POS 3
  action:
  1: on (light/water/heater/sensor)
  2: off (light/water/heater/sensor)
  3: timer (when on)
  4: read (sensor)
  5: sim(ulate sensor, incl. trigger)
  6: reset (action on/timer/sim)
POS 4+
  on
    4: duration+unit (optional, blank = indefinitly)
  off (single event)
  timer
    4: start time | 5: duration+unit (optional, blank = indefinitly)
  read (single event)
  sim
    4: simulated value | 5: duration+unit (optional, blank = indefinitly)
  reset (single event)
    
units: 
  1: millisecond (default), 
  2: second (s), 
  3: minute (m), 
  4: hour (h), 
  5: day (d)  
*/

// shelve timer
int s1[] = {};
int s2[] = {};
int s3[] = {};
int s4[] = {};
int s5[] = {};
int s6[] = {};
int s7[] = {};
int s8[] = {};
int s9[] = {};
int s10[] = {};
int s11[] = {};
int s12[] = {};
int s13[] = {};
int s14[] = {};
int s15[] = {};
int s16[] = {};
int s17[] = {};
int s18[] = {};
int s19[] = {};
int s20[] = {};

// supplies
int light[] = {};
int water[] = {};
int heater[] = {};

// sensors
int temp[] = {};
int humid[] = {};
int sun[] = {};
int prox[] = {};

// processing vars
String cmd[9] = {}; // command

// testing
String message1 = "s1 light on";
String message2 = "s2 light on 3h";
String message3 = "s3 light timer 15:00:00";
String message4 = "s4 water timer 15:00:00 7m";
String message5 = "s4 temp sim 20 3h"; // temp in celsius
String message6 = "s4 humid sim 10 5m"; // humidity in percent
String message7 = "s5 prox read";
String message8 = "all water reset";
String message9 = "exit";  

///// FUNCTIONS /////

int buildCmdArray(String m, String c, String cmd[]) {
  // m: message
  // c: character
  // cp: first character position in message
  // cmd: command

  Serial.print("=> NEW COMMAND:\n");
  int cp = -1; // defaults to false
  int i = 0;
  bool hasChar = 0;
  
  do {
    cp = m.indexOf(c);
    hasChar = cp != -1;
    
    if (hasChar) {            
      // extract command element 
      cmd[i] = m.substring(0, cp);
      // remove command element from message
      m = m.substring(cp+1, m.length());
      
    } else {      
      // message remainder is last command element
      cmd[i] = m;
      // delete message remainder
      m = "";
      // delete cmdArray remainder
      for (int j = i+1; j <= 9; j++) {
        cmd[j] = "";
      }
      
    } // if
    
    i++;
    
  } while (hasChar);
  
  return cmd;
}

void printCmdArray(String cmd[]) {
  for (int i = 0; i <= 9; i++) {
    Serial.print(i);
    Serial.print(" | ");
    Serial.print(cmd[i]);
    Serial.print("\n");
  }
}

void execCmdArray(String cmd[]) {
  Serial.print("=> EXECUTING COMMAND");
  Serial.print("\n");
  int cmd0, cmd1, cmd2, cmd3, cmd4, cmd5, cmd6, cmd7, cmd8, cmd9;
  
  if      (cmd[1] == "light") { cmd1 = 1; }
  else if (cmd[1] == "water") { cmd1 = 2; }
  else if (cmd[1] == "heat")  { cmd1 = 3; }
  else                        { cmd1 = -1; }
  
  if      (cmd[2] == "on")    { cmd2 = 1; }
  else if (cmd[2] == "off")   { cmd2 = 2; }
  else if (cmd[2] == "timer") { cmd2 = 3; }
  else if (cmd[2] == "read")  { cmd2 = 4; }
  else if (cmd[2] == "sim")   { cmd2 = 5; }
  else if (cmd[2] == "reset") { cmd2 = 6; }
  else                        { cmd2 = -1; }

  switch (cmd1) {
    case 1:
      Serial.print("... light");
      break;
    case 2:
      Serial.print("... water");
      break;
    case 3:
      Serial.print("... water");
      break;
    default:
      Serial.print("... default");
      break;
  }
  
  Serial.print("\n");

  switch (cmd2) {
    case 1:
      Serial.print("... on");
      break;
    case 2:
      Serial.print("... off");
      break;
    case 3:
      Serial.print("... timer");
      break;
    case 4:
      Serial.print("... read");
      break;
    case 5:
      Serial.print("... sim");
      break;
    case 6:
      Serial.print("... reset");
      break;
    default:
      Serial.print("... default");
      break;
  }

  Serial.print("\n");

  // time
  
  //Serial.print(cmd[3]);
  int cmdHour = cmd[3].substring(0, 2).toInt();
  int cmdMin = cmd[3].substring(3, 5).toInt();
  int cmdSec = cmd[3].substring(6, 8).toInt();

  Serial.print(cmdHour);
  Serial.print(":");
  Serial.print(cmdMin);
  Serial.print(":");
  Serial.print(cmdSec);

  Serial.print("\n");

  // duration + unit
  int unit;
  int dur;
  if (cmd[4].endsWith("ms")) { 
    unit = 1;
    dur = cmd[4].substring(0, cmd[4].length() - 2).toInt();
    
  } else if (cmd[4].endsWith("s")) {
    unit = 2;
    dur = cmd[4].substring(0, cmd[4].length() - 1).toInt();

  } else if (cmd[4].endsWith("m")) {
    unit = 3;
    dur = cmd[4].substring(0, cmd[4].length() - 1).toInt();

  } else if (cmd[4].endsWith("h")) {
    unit = 4;
    dur = cmd[4].substring(0, cmd[4].length() - 1).toInt();

  } else if (cmd[4].endsWith("d")) {
    unit = 5;
    dur = cmd[4].substring(0, cmd[4].length() - 1).toInt();
  
  } else {
    Serial.print("no unit defined");
  }
  
  Serial.print(dur);
  Serial.print(" duration|unit ");
  Serial.print(unit);

} // execCmdArray

///// TIME MODULE /////

#include <Wire.h>
#include "RTClib.h"
 
RTC_DS3231 rtc;
 
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};



void setup() {
  
  pinMode(13, OUTPUT);    // sets the digital pin 13 as output
  Serial.begin(9600);

  buildCmdArray(message8, " ", cmd);
  printCmdArray(cmd);
  
  buildCmdArray(message4, " ", cmd);
  printCmdArray(cmd);
  
  execCmdArray(cmd);

  ///// TIME MODULE /////
  
  #ifndef ESP8266
    while (!Serial); // for Leonardo/Micro/Zero
  #endif
   
    Serial.begin(9600);
   
    delay(3000); // wait for console opening
   
    if (! rtc.begin()) {
      Serial.println("Couldn't find RTC");
      while (1);
    }
   
    if (rtc.lostPower()) {
      Serial.println("RTC lost power, lets set the time!");
      // following line sets the RTC to the date & time this sketch was compiled
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
      // This line sets the RTC with an explicit date & time, for example to set
      // January 21, 2014 at 3am you would call:
      // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
    }
  }
  
} // setup



void loop() {
  
  // findAllCharPos(cmd, acp, message6, " ");

/*  
  digitalWrite(13, HIGH); // sets the digital pin 13 on
  delay(1000);            // waits for a second
  digitalWrite(13, LOW);  // sets the digital pin 13 off
  delay(1000);            // waits for a second
*/

} // loop
