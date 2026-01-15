#pragma once
#include <Arduino.h>
#include <TimeLib.h>
#include <SD.h>

class SDLogger {
public:
    SDLogger(uint8_t csPin, const char* basePath = "/");

    bool begin();                      // Initialize SD card
    void logReading(float value, const char* unit);  // Log a single reading

private:
    uint8_t _csPin;
    const char* _basePath;
    char _currentFilename[32];  // max path length

    time_t _currentTime;

    void updateCurrentTime();
    void prepareDailyFile();
    void generateFilename();
};
