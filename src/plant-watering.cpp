/* The RTC code was written mostly by Harrison in Dec 22.  I slowly leanring it - Jan 2023  */
#include <Arduino.h>
#include <Wire.h>                    // to speak I2C
#include <Adafruit_GFX.h>            // graphic lib for 1306
#include <Adafruit_SSD1306.h>        // 1306 commands
#include <SSD1306AsciiAvrI2c.h>      // I2C interface, not sure how diff from Wire.h
#include <RTClib.h>                  // provides all the RTC lib commands, etc.

const int pumpOnDuration = 6000;      // pump on time in ms
const int pumpPin = 4;                // pump pin
RTC_DS1307 rtc;                       // initalize RTC module/chip
const TimeSpan oneHour = TimeSpan(60*60); // TimeSpan is part of RTClib = 1 sec
const TimeSpan oneMin = TimeSpan(60);     
enum daysOfWeek {SUN, MON, TUE, WED, THU, FRI, SAT};  // a list of the days, per Harrison's help
struct runTime {                      // I get this but not totally :(
  enum daysOfWeek dayOfWeek;          // Data inside daysOfWeek enum (array?)
  int hour; 
};

const int SCHEDULE_SIZE = 1;            // still learning: allows an 'array' of one
struct runTime schedule[SCHEDULE_SIZE]; // create an array called runTime
DateTime lastRun;                       // DateTime from RTClib

void setUpSchedule() {                  // when to run pump/watering
  schedule[0].dayOfWeek = SAT;
  schedule[0].hour = 10;
}  

bool shouldRun(DateTime now) {          // checks if pump should run
  for (int i = 0; i < SCHEDULE_SIZE; i++) {
    if (
      schedule[i].dayOfWeek == now.dayOfTheWeek() && 
      schedule[i].hour == now.hour() &&
      now > lastRun + oneHour 
    ) return true;
  }
  return false;  
} 

void runPump(){                        // runs the pump for 'pumpOnDuration'
  digitalWrite(pumpPin, HIGH);
  delay(pumpOnDuration);
  digitalWrite(pumpPin, LOW);  
}

void setup() {
  setUpSchedule();                    // call to set schedule for pump run            
  Serial.begin(9600);               // init serial monitor 
  delay(2000);                        // wait for rtc to start (one time)
  if (! rtc.begin()) {                // I found this code online, strongly suggest to use
    Serial.println("Couldn't find RTC");
    while (1);
  }
  
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));     // sets the RTC on each compile to the PC time

}

void loop() {
 
  DateTime now = rtc.now();                           // set "now" var to current
  Serial.println(String("TIME:\t")+now.timestamp(DateTime::TIMESTAMP_FULL));

// Below code commented out to test run
 // if (shouldRun(now)) {
 //   Serial.println(" in 'should run' ");
 //   runPump();
 //   lastRun = now;
 // }
 // delay(oneMin.totalseconds() * 1000);
delay(2000);

}
