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
int intervalSeconds = 20; // interval between measurements
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
void logData(int idx, float raw, float rough_kg, float conversion_factor) {
  logFile.print(digitalClockDisplay());
  logFile.print(",");
  logFile.print(counter);
  logFile.print(",");
  logFile.print(idx);
  logFile.print(",");
  logFile.print(raw);
  logFile.print(",");
  logFile.print(rough_kg);
  logFile.print(",");
  logFile.print(conversion_factor);
  logFile.println();
  logFile.sync();
  while (sd.card()->isBusy()) {}
}


// HX711 circuit wiring

const int LOADCELL_DOUT_PIN = 15;
const int LOADCELL_SCK_PIN = 4; 

const int LOADCELL_DOUT_PIN_2 = 14;
const int LOADCELL_SCK_PIN_2 = 2;

const int LOADCELL_DOUT_PIN_3 = 33;
const int LOADCELL_SCK_PIN_3 = 3;

const float SCALE_FACTOR_1 = -42437.94; // Calibration factor for load cell 1
const float SCALE_FACTOR_2 = -42065.61; // Calibration factor for load cell 2
const float SCALE_FACTOR_3 = -42336.10; // Calibration factor for load cell 3

HX711 scale;
HX711 scale2;
HX711 scale3;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(57600);
  
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.tare();
  scale.set_scale(SCALE_FACTOR_1);

  scale2.begin(LOADCELL_DOUT_PIN_2, LOADCELL_SCK_PIN_2);
  scale2.tare();
  scale2.set_scale(SCALE_FACTOR_2);
 
  scale3.begin(LOADCELL_DOUT_PIN_3, LOADCELL_SCK_PIN_3);
  scale3.tare();
  scale3.set_scale(SCALE_FACTOR_3);

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

  counter++;


  long raw = 0;
  long raw2 = 0;  
  long raw3 = 0;


  if (scale.wait_ready_timeout(1000)) {
    raw = scale.get_value(10);
    // Serial.print("HX711 raw: ");
    // Serial.println(raw);
  } else {
    // Serial.println("HX711 not found.");
  }

  if (scale2.wait_ready_timeout(1000)) {
    raw2 = scale2.get_value(10);
    // Serial.print("HX711_2 raw: ");
    // Serial.println(raw2);
  } else {
    // Serial.println("HX711_2 not found.");
  }
 
  if (scale3.wait_ready_timeout(1000)) {
    raw3 = scale3.get_value(10);
    // Serial.print("HX711_3 raw: ");
    // Serial.println(raw3);
  } else {
    // Serial.println("HX711_3 not found.");
  }

  // Serial.println("-----");
  // Serial.print("Sum:");
  // Serial.println(raw + raw2 + raw3);

  logData(1, raw, raw / SCALE_FACTOR_1, SCALE_FACTOR_1);
  logData(2, raw2, raw2 / SCALE_FACTOR_2, SCALE_FACTOR_2);
  logData(3, raw3, raw3 / SCALE_FACTOR_3, SCALE_FACTOR_3);
  logData(-1, (raw + raw2 + raw3), (raw / SCALE_FACTOR_1 + raw2 / SCALE_FACTOR_2 + raw3 / SCALE_FACTOR_3), -1);

  digitalWriteFast(LED_BUILTIN, HIGH);
  delay(1000);  
  digitalWriteFast(LED_BUILTIN, LOW);

  Snooze.deepSleep(config);

  // delay(2000);
  
}