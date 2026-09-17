// noise_display.h
#pragma once
#include <Adafruit_NeoPixel.h>

// #include "config.h"

// // Neopixel stick
constexpr int PIXEL_PIN = 4;
constexpr int PIXEL_COUNT = 8;
constexpr int INIT_BRIGHTNESS = 20;


// Signal processing
constexpr double MIN_DB = 55.0;
constexpr double MAX_DB = 85.0;
constexpr int DIVISOR = (MAX_DB - MIN_DB) / PIXEL_COUNT;
constexpr double RISE_FACTOR = 0.85;
constexpr double FALL_FACTOR = 0.3;
constexpr double HYSTERESIS = 3.0;

extern Adafruit_NeoPixel strip;

void setup_neopixel();
void show_noise_level(double raw_intensity);  

