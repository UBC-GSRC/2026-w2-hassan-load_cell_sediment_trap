#include "SDLogger.h"

SDLogger::SDLogger(uint8_t csPin, const char* basePath)
    : _csPin(csPin), _basePath(basePath), _currentTime(0) {
    _currentFilename[0] = 0;
}

bool SDLogger::begin() {
    if (!SD.begin(_csPin)) {
        Serial.println("SD card initialization failed!");
        return false;
    }
    Serial.println("SD card initialized.");
    return true;
}

void SDLogger::updateCurrentTime() {
    _currentTime = now(); // TimeLib function
}

void SDLogger::generateFilename() {
    int yr = year(_currentTime);
    int mo = month(_currentTime);
    int dy = day(_currentTime);
    sprintf(_currentFilename, "%s%04d-%02d-%02d.csv", _basePath, yr, mo, dy);
}

void SDLogger::prepareDailyFile() {
    updateCurrentTime();
    generateFilename();

    if (!SD.exists(_currentFilename)) {
        File file = SD.open(_currentFilename, FILE_WRITE);
        if (file) {
            file.println("timestamp,reading,unit"); // CSV header
            file.close();
            Serial.print("Created new log file: ");
            Serial.println(_currentFilename);
        } else {
            Serial.print("Failed to create file: ");
            Serial.println(_currentFilename);
        }
    }
}

void SDLogger::logReading(float value, const char* unit) {
    prepareDailyFile();

    updateCurrentTime();
    File file = SD.open(_currentFilename, FILE_WRITE);
    if (file) {
        file.print(_currentTime); file.print(",");
        file.print(value, 3); file.print(",");
        file.println(unit);
        file.close();
    } else {
        Serial.print("Failed to open file for writing: ");
        Serial.println(_currentFilename);
    }
}
