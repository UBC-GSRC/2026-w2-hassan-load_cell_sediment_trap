#include "HX711.h"
#include <SdFat.h>
#include <SPI.h>
#include <TimeLib.h>
#include <Snooze.h>

// ------------------------------------------------------------
// Snooze setup
// ------------------------------------------------------------
SnoozeSPI   sdCard;
SnoozeAlarm alarm;
SnoozeBlock config(sdCard, alarm);
int intervalSeconds = 10; // interval between measurements
long unsigned int counter = 0;

// ------------------------------------------------------------
// SD / SdFat setup
// ------------------------------------------------------------
const uint8_t chipSelect = BUILTIN_SDCARD;
SdFat sd;
SdFile logFile;

// ------------------------------------------------------------
// RTC Time
// ------------------------------------------------------------
time_t getTeensy3Time() {
  return Teensy3Clock.get();
}

String digitalClockDisplay() {
  time_t t = getTeensy3Time();
  time_t local = t - 8 * 3600;

  char buf[20];
  snprintf(buf, sizeof(buf), "%04d-%02d-%02d,%02d:%02d:%02d", year(local), month(local), day(local), hour(local), minute(local), second(local));
  return String(buf);
}

// ------------------------------------------------------------
// Logging
// ------------------------------------------------------------
void logData(float reading, int idx, String units) {
  logFile.print(digitalClockDisplay());
  logFile.print(",");
  logFile.print(counter);
  logFile.print(",");
  logFile.print(idx);
  logFile.print(",");
  logFile.print(reading);
  logFile.print(",");
  logFile.print(units);
  logFile.println();
  logFile.sync();
  while (sd.card()->isBusy()) {}
}


// HX711 circuit wiring
// const int LOADCELL_DOUT_PIN = 2;
// const int LOADCELL_SCK_PIN = 14; 

const int LOADCELL_DOUT_PIN = 15;
const int LOADCELL_SCK_PIN = 4; 

const int LOADCELL_DOUT_PIN_2 = 14;
const int LOADCELL_SCK_PIN_2 = 2;

const int LOADCELL_DOUT_PIN_3 = 33;
const int LOADCELL_SCK_PIN_3 = 3;

HX711 scale;
HX711 scale2;
HX711 scale3;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(57600);
  
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.tare();
  scale.set_scale(-42437.94);

  scale2.begin(LOADCELL_DOUT_PIN_2, LOADCELL_SCK_PIN_2);
  scale2.tare();
  scale2.set_scale(-42065.61);
 
  scale3.begin(LOADCELL_DOUT_PIN_3, LOADCELL_SCK_PIN_3);
  scale3.tare();
  scale3.set_scale(-42336.10);

  pinMode(LED_BUILTIN, OUTPUT);

  setSyncProvider(getTeensy3Time);
  
  alarm.setRtcTimer(0, 0, intervalSeconds);  // wake every intervalSeconds sec
 
  sdCard.setClockPin(BUILTIN_SDCARD);
  delay(3000); // wait for SD card to stabilize
  if (!sd .begin(chipSelect)) {
    while (1) {
      digitalWriteFast(LED_BUILTIN, !digitalReadFast(LED_BUILTIN));
      delay(200);
    }
  }

  if (!logFile.open("datalog.csv", O_CREAT | O_WRITE | O_APPEND)) {
    while (1);
  }

  alarm.setRtcTimer(0, 0, intervalSeconds);  // wake every intervalSeconds sec
}

void loop() {     
  digitalWriteFast(LED_BUILTIN, HIGH);
  delay(1000);  
  digitalWriteFast(LED_BUILTIN, LOW);
  counter++;


  long reading = 0;
  long reading2 = 0;  
  long reading3 = 0;


  if (scale.wait_ready_timeout(1000)) {
    reading = scale.get_units(10);
    // Serial.print("HX711 reading: ");
    // Serial.println(reading);
  } else {
    // Serial.println("HX711 not found.");
  }

  if (scale2.wait_ready_timeout(1000)) {
    reading2 = scale2.get_units(10);
    // Serial.print("HX711_2 reading: ");
    // Serial.println(reading2);
  } else {
    // Serial.println("HX711_2 not found.");
  }
 
  if (scale3.wait_ready_timeout(1000)) {
    reading3 = scale3.get_units(10);
    // Serial.print("HX711_3 reading: ");
    // Serial.println(reading3);
  } else {
    // Serial.println("HX711_3 not found.");
  }

  // Serial.println("-----");
  // Serial.print("Sum:");
  // Serial.println(reading + reading2 + reading3);

  logData(reading, 1, "kg");
  logData(reading2, 2, "kg");
  logData(reading3, 3, "kg");
  logData(reading + reading2 + reading3, -1, "kg_total");

  Snooze.deepSleep(config);

  // delay(2000);
  
}