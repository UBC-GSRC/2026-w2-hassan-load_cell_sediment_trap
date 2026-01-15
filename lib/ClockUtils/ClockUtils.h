#pragma once
#include <Arduino.h>
#include <TimeLib.h>

time_t getTeensy3Time();
void printDigits(int digits);
void digitalClockDisplay();
unsigned long processSyncMessage();
