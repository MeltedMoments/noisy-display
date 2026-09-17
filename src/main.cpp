#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <driver/i2s.h>

#include "microphone.h"
#include "noise_display.h"

void setup() {
    Serial.begin(115200);
    // delay(10000);
    Serial.println("Noisy Display");

    setup_microphone();
    setup_noise_display();
}

void loop() {
    double reading = measure_sound_level();
    show_noise_level(reading);
}