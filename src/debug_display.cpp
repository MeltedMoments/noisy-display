/** 
 *  debug_display.cpp
*/
#include  <Adafruit_GFX.h>
#include  <Adafruit_SH110X.h>

#include "debug_display.h"
#include "noisy_config.h"

Adafruit_SH1107 display(
    OLED_WIDTH,
    OLED_HEIGHT,
    OLED_MOSI,
    OLED_CLK,
    OLED_DC,
    OLED_RST,
    OLED_CS
);

void setup_debug_display() {
    Serial.println("Starting OLED");
    if (! display.begin(0, true)) {
        Serial.println("OLED init failed");
        return;
    }
    display.clearDisplay();
    display.setTextColor(SH110X_WHITE);
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.setRotation(3);
    display.println("Hello world!");
    display.display();
    Serial.println("OLED ready");
}

void small_heart(int colour) {
    display.drawBitmap(
        OLED_HEARTBEAT_COL, 
        OLED_HEARTBEAT_ROW,
        // 112, 2,
        heart_8x8,
        8,8,
        colour
    );
}

void large_heart(int colour) {
    display.drawBitmap(
        112, 2,
        heart_12x12,
        16,16,
        colour
    );
}

void show_debug_heartbeat(bool heartbeat_on) {
    display.setTextColor(SH110X_WHITE);
    int colour = heartbeat_on ? SH110X_WHITE : SH110X_BLACK;
    small_heart(colour);
    display.display();
}

void show_debug_info(double estimated_db, int target_level, int display_level) {
    display.clearDisplay();
    display.setTextColor(SH110X_WHITE);
    // Temperature
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("Estimated");
    display.setTextSize(2);
    display.setCursor(0, 14);
    display.print(estimated_db, 1);
    display.println(" dB");
}
//     // Humidity
//     display.setTextSize(1);
//     display.setCursor(0, 38);
//     display.println("Humidity");
//     display.setTextSize(2);
//     display.setCursor(0, 50);
//     display.print(humidity, 1);
//     display.println("%");
    
//     // Comfort
//     float comfort = comfort_score(temperature, humidity);
//     display.setTextSize(1);
//     display.setCursor(70, 38);
//     display.println("Comfort ");
//     display.setTextSize(2);
//     display.setCursor(70, 50);
//     display.println(comfort_text(comfort));
    
//     display.display();
// }