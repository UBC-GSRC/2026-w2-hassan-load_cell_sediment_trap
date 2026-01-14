#include "HX711.h"

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
  Serial.begin(57600);
  
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.tare();
  scale.set_scale(-3233933.0 / 175.0);

  scale2.begin(LOADCELL_DOUT_PIN_2, LOADCELL_SCK_PIN_2);
  scale2.tare();
  scale2.set_scale(-3233933.0 / 175.0);
 
  scale3.begin(LOADCELL_DOUT_PIN_3, LOADCELL_SCK_PIN_3);
  scale3.tare();
  scale3.set_scale(-3233933.0 / 175.0);
}

void loop() {     

  long reading = 0;
  long reading2 = 0;  
  long reading3 = 0;

  if (scale.wait_ready_timeout(1000)) {
    reading = scale.get_units();
    Serial.print("HX711 reading: ");
    Serial.println(reading);
  } else {
    Serial.println("HX711 not found.");
  }

  if (scale2.wait_ready_timeout(1000)) {
    reading2 = scale2.get_units();
    Serial.print("HX711_2 reading: ");
    Serial.println(reading2);
  } else {
    Serial.println("HX711_2 not found.");
  }
 
  if (scale3.wait_ready_timeout(1000)) {
    reading3 = scale3.get_units();
    Serial.print("HX711_3 reading: ");
    Serial.println(reading3);
  } else {
    Serial.println("HX711_3 not found.");
  }

  Serial.println("-----");
  Serial.print("Sum:");
  Serial.println(reading + reading2 + reading3);

  delay(333);
  
}