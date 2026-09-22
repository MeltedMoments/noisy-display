#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <driver/i2s.h>

#include "noisy_config.h"
#include "microphone.h"
#include "noise_display.h"
#include "noise_level.h"
#include "heartbeat.h"
#include "button.h"
#include "sensitivity.h"
#include "debug_display.h"

uint8_t target_level = 0;
uint8_t display_level = 0;
Sensitivity sensitivity = Sensitivity::MEDIUM_LEVEL;

void setup() {
    Serial.begin(115200);
    // delay(10000);
    Serial.println("Noisy Display");

    setup_microphone();
    setup_noise_display();
    setup_debug_display();
}

void heartbeat() {
    if (update_heartbeat()) {
        show_debug_heartbeat(heartbeat_state());
    }
}

void loop() {
    heartbeat();

    if (button_pressed()) {
        sensitivity = next_sensitivity(sensitivity);
        Serial.printf("Sensitivity: %s\n", sensitivity_name(sensitivity));
    }

    double estimated_db;
    if (update_sound_measurement(estimated_db)) {
        double min_db = minimum_db(sensitivity);
        target_level = calc_noise_level(estimated_db, target_level, PIXEL_COUNT, min_db);
        display_level = apply_rate_limit(display_level, target_level);
        show_noise_level(display_level);

        show_debug_info(
            estimated_db, 
            target_level, 
            display_level, 
            sensitivity_name(sensitivity)
        );
    }
}

    // Serial.printf(
    //     "Estimated dB: %.1f target level: %d display: %d\r\n", 
    //     estimated_db,
    //     target_level,
    //     display_level
    // );
