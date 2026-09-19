// noise_display.cpp

#include <Arduino.h>
#include "noisy_config.h"
#include "noise_display.h"
#include "noise_level.h"

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
    strip.setBrightness(INIT_BRIGHTNESS);
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

void show_noise_level(double intensity) {      
    int target_level = calc_noise_level(intensity, current_level, PIXEL_COUNT);
    int new_level = apply_rise_limit(current_level, target_level);
    new_level = apply_fall_limit(current_level, target_level);
    Serial.printf(
        " Level current: %d target: %d display: %d\r\n", 
        current_level,
        target_level,
        new_level
    );
    current_level = new_level;
    show_pixels(current_level);
}
