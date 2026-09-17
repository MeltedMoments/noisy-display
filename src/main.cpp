#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <driver/i2s.h>

#include "microphone.h"
#include "noise_display.h"

// Neopixel stick
// constexpr int PIXEL_PIN = 4;
// constexpr int PIXEL_COUNT = 8;
// constexpr double MIN_DB = 55.0;
// constexpr double MAX_DB = 85.0;
// constexpr int DIVISOR = (MAX_DB - MIN_DB) / PIXEL_COUNT;
// constexpr double RISE_FACTOR = 0.85;
// constexpr double FALL_FACTOR = 0.3;
// constexpr double HYSTERESIS = 3.0;

// Adafruit_NeoPixel strip(
//     PIXEL_COUNT,
//     PIXEL_PIN,
//     NEO_GRB + NEO_KHZ800
// );
// const uint32_t GREEN  = strip.Color(0, 255, 0);
// const uint32_t YELLOW = strip.Color(255, 180, 0);
// const uint32_t RED    = strip.Color(255, 0, 0);
// const uint32_t colours[] = {
//     GREEN, GREEN, YELLOW, YELLOW,
//     YELLOW, RED, RED, RED,    
// };

// unsigned long last_report_time = 0;
// unsigned long last_sensor_time = 0;
// unsigned int current_level = 0;



// void setup_neopixel() {
//     strip.begin();
//     strip.setBrightness(20);
//     // strip.clear();
//     // show_pixels(8);
//     // delay(2000);
//     strip.clear();
//     strip.show();
// }

void setup() {
    Serial.begin(115200);
    // delay(10000);
    Serial.println("Noisy Display");

    setup_microphone();
    setup_neopixel();
}

// void show_pixels(int pixels) {
//     strip.clear();
//     for (int i=0; i < pixels; i++) {
//         strip.setPixelColor(i, colours[i]);
//     }
//     strip.show();
// }

// void show_noise_level(double raw_intensity) {      
//     if (raw_intensity < MIN_DB) {
//         raw_intensity = MIN_DB;
//     }
//     if (raw_intensity > MAX_DB) {
//         raw_intensity = MAX_DB;
//     }

//     int level = current_level;
//     double intensity = raw_intensity - MIN_DB;
//     double lower_threshold = current_level * DIVISOR - HYSTERESIS;
//     double upper_threshold = (current_level + 1) * DIVISOR + HYSTERESIS;
//     double threshold = current_level * DIVISOR;   
//     if (   (intensity < lower_threshold) 
//         || (intensity > upper_threshold)) {
//             level = intensity / DIVISOR;   
//     }

//     Serial.printf(
//         " Level current: %d new: %d\r\n", 
//         current_level,
//         level
//     );
//     if (level < 0) {
//         level = 0;
//     } else if (level > PIXEL_COUNT) {
//         level = PIXEL_COUNT;
//     } 
//     current_level = level;
//     show_pixels(level);
// }

void loop() {
    double reading = measure_sound_level();
    show_noise_level(reading);
}