#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <driver/i2s.h>

#include "noisy_config.h"
#include "microphone.h"
#include "noise_display.h"
#include "noise_level.h"

// uint8_t current_level = 0;
uint8_t target_level = 0;
uint8_t display_level = 0;

void setup() {
    Serial.begin(115200);
    // delay(10000);
    Serial.println("Noisy Display");

    setup_microphone();
    setup_noise_display();
}

// void calculate_noise_level(double intensity) {
//     target_level = calc_noise_level(intensity, target_level, PIXEL_COUNT);
//     display_level = apply_rate_limit(display_level, target_level);
//     Serial.printf(
//         " Level current: %d target: %d display: %d\r\n", 
//         display_level,
//         target_level,
//         new_level
//     );

// }

void loop() {
    double intensity = measure_sound_level();

    target_level = calc_noise_level(intensity, target_level, PIXEL_COUNT);
    display_level = apply_rate_limit(display_level, target_level);
    Serial.printf(
        " Level target: %d display: %d\r\n", 
        target_level,
        display_level
    );
    show_noise_level(display_level);

}