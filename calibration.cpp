#include "HX711.h"

// Pins for the three scales
const int LOADCELL_DOUT_PIN_1 = 15;
const int LOADCELL_SCK_PIN_1  = 4;

const int LOADCELL_DOUT_PIN_2 = 14;
const int LOADCELL_SCK_PIN_2  = 2;

const int LOADCELL_DOUT_PIN_3 = 33;
const int LOADCELL_SCK_PIN_3  = 3;

// HX711 objects
HX711 scale1;
HX711 scale2;
HX711 scale3;

// Calibration factors
float factor1 = 0;
float factor2 = 0;
float factor3 = 0;

// Forward declaration
void calibrateInteractiveScale(HX711 &scale, int scaleNum);

// -------------------------
// Waits for any input over Serial
// -------------------------
void waitForSerial() {
  Serial.println("Send any character to continue...");
  while (Serial.available() == 0) {
    // do nothing, just wait
  }
  // Clear the input buffer
  while (Serial.available() > 0) Serial.read();
}

void setup() {
  Serial.begin(57600);
  while (!Serial);

  Serial.println("Interactive 3-Scale HX711 Calibration");
  Serial.println("--------------------------------------");

  // Initialize all scales
  scale1.begin(LOADCELL_DOUT_PIN_1, LOADCELL_SCK_PIN_1);
  scale2.begin(LOADCELL_DOUT_PIN_2, LOADCELL_SCK_PIN_2);
  scale3.begin(LOADCELL_DOUT_PIN_3, LOADCELL_SCK_PIN_3);

  // Reset scale factors
  scale1.set_scale();
  scale2.set_scale();
  scale3.set_scale();
}

void loop() {
  // Calibrate scales one by one
  calibrateInteractiveScale(scale1, 1);
  calibrateInteractiveScale(scale2, 2);
  calibrateInteractiveScale(scale3, 3);

  // All scales calibrated
  Serial.println("\nAll scales calibrated!");
  Serial.print("Scale 1 factor: "); Serial.println(factor1);
  Serial.print("Scale 2 factor: "); Serial.println(factor2);
  Serial.print("Scale 3 factor: "); Serial.println(factor3);

  // Apply factors for live display
  scale1.set_scale(factor1);
  scale2.set_scale(factor2);
  scale3.set_scale(factor3);

  Serial.println("\nLive readings every second:");
  while (1) {
    if (scale1.wait_ready_timeout(1000)) { Serial.print("S1: "); Serial.print(scale1.get_units(10)); Serial.print(" kg, "); }
    if (scale2.wait_ready_timeout(1000)) { Serial.print("S2: "); Serial.print(scale2.get_units(10)); Serial.print(" kg, "); }
    if (scale3.wait_ready_timeout(1000)) { Serial.print("S3: "); Serial.println(scale3.get_units(10)); }
    delay(1000);
  }
}

// -------------------------
// Interactive calibration function
// -------------------------
void calibrateInteractiveScale(HX711 &scale, int scaleNum) {
  Serial.print("\n--- Preparing Scale ");
  Serial.print(scaleNum);
  Serial.println(" ---");

  // Step 1: choose the scale interactively
  Serial.print("Confirm Scale ");
  Serial.print(scaleNum);
  Serial.println(" by observing live readings.");
  Serial.println("Press 'y' and send over Serial when ready.");

  while (true) {
    // Print live readings for this scale every 500 ms
    if (scale.wait_ready_timeout(500)) {
      float val = scale.get_units(5);
      Serial.print("Current reading: ");
      Serial.println(val);
    }

    if (Serial.available()) {
      char c = Serial.read();
      if (c == 'y' || c == 'Y') break;
    }
    delay(100);
  }

  // Step 2: tare
  Serial.println("Taring... Remove all weight from scale.");
  Serial.println("Press any key and send over Serial to continue.");
  waitForSerial();
  scale.tare();
  Serial.println("Tare complete. Place known weight on scale.");

  // Step 3: measure known weight
  Serial.println("Enter the known weight in kg and send over Serial:");
  while (Serial.available() == 0) {}
  float knownWeight = Serial.parseFloat();

  // Step 4: measure raw reading
  long reading = scale.get_units(10);
  Serial.print("Raw reading: ");
  Serial.println(reading);

  // Step 5: compute suggested scale factor
  float scaleFactor = reading / knownWeight;
  Serial.print("Suggested scale factor for Scale ");
  Serial.print(scaleNum);
  Serial.print(": ");
  Serial.println(scaleFactor);

  // Save factor
  if (scaleNum == 1) factor1 = scaleFactor;
  if (scaleNum == 2) factor2 = scaleFactor;
  if (scaleNum == 3) factor3 = scaleFactor;

  Serial.println("Calibration done for this scale.\n");
}
