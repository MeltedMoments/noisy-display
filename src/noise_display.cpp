// noise_display.cpp

// #include "config.h"
#include "noise_display.h"


Adafruit_NeoPixel strip(
    PIXEL_COUNT,
    PIXEL_PIN,
    NEO_GRB + NEO_KHZ800
);
const uint32_t GREEN  = strip.Color(0, 255, 0);
const uint32_t YELLOW = strip.Color(255, 180, 0);
const uint32_t RED    = strip.Color(255, 0, 0);
const uint32_t colours[] = {
    GREEN, GREEN, YELLOW, YELLOW,
    YELLOW, RED, RED, RED,    
};

unsigned int current_level = 0;

void setup_noise_display() {
    strip.begin();
    strip.setBrightness(20);
    strip.clear();
    strip.show();
}

void show_pixels(int pixels) {
    strip.clear();
    for (int i=0; i < pixels; i++) {
        strip.setPixelColor(i, colours[i]);
    }
    strip.show();
}

void show_noise_level(double raw_intensity) {      
    if (raw_intensity < MIN_DB) {
        raw_intensity = MIN_DB;
    }
    if (raw_intensity > MAX_DB) {
        raw_intensity = MAX_DB;
    }

    int level = current_level;
    double intensity = raw_intensity - MIN_DB;
    double lower_threshold = current_level * DIVISOR - HYSTERESIS;
    double upper_threshold = (current_level + 1) * DIVISOR + HYSTERESIS;
    double threshold = current_level * DIVISOR;   
    if (   (intensity < lower_threshold) 
        || (intensity > upper_threshold)) {
            level = intensity / DIVISOR;   
    }

    Serial.printf(
        " Level current: %d new: %d\r\n", 
        current_level,
        level
    );
    if (level < 0) {
        level = 0;
    } else if (level > PIXEL_COUNT) {
        level = PIXEL_COUNT;
    } 
    current_level = level;
    show_pixels(level);
}

