#include "ClockUtils.h"

time_t getTeensy3Time() {
  return Teensy3Clock.get();
}

void printDigits(int digits) {
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

void digitalClockDisplay() {
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year());
  Serial.println();
}

#define TIME_HEADER  "T"

unsigned long processSyncMessage() {
  unsigned long pctime = 0L;
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013 

  if (Serial.find(TIME_HEADER)) {
    pctime = Serial.parseInt();
    if (pctime < DEFAULT_TIME)
      return 0L;
  }
  return pctime;
}
