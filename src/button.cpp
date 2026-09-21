/** 
 *  button.cpp - generic button handling
*/
#include  <Adafruit_GFX.h>

#include "noisy_config.h"
  
int last_raw_button_state = HIGH;
int stable_button_state = HIGH;
unsigned long last_change_time = 0;


void setup_button() {
    // Set up the display's button(s)
    pinMode(OLED_KEY1, INPUT_PULLUP);
    // last_key_state = 
}

bool button_pressed() {
    unsigned long now = millis();
    // get the raw state and determine if it's changed
    int raw_state = digitalRead(OLED_KEY1);
    if (raw_state != last_raw_button_state) {
        last_change_time = now;
        last_raw_button_state = raw_state;
    }

    // Apply debouncing
    if (now - last_change_time >= DEBOUNCE_MS) {
        if (raw_state != stable_button_state) {
            stable_button_state = raw_state;
            if (stable_button_state == LOW) {
                return true;
            }
        }
    }
    return false;
}