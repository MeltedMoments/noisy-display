// board_test.cpp

#include <Arduino.h>

void setup() {
    Serial.begin(115200);
    // delay(3000);

    Serial.println("Hello from the new ESP!");
    Serial.printf(
        "Flash:  %u bytes (%1.f MB)\n",
        ESP.getFlashChipSize(),
        ESP.getFlashChipSize() / 1024.0 / 1024.0
    );
    Serial.printf(
        "PSRAM:  %u bytes (%1.f MB)\n",
        ESP.getPsramSize(),
        ESP.getPsramSize() / 1024.0 / 1024.0
    );
    Serial.printf("Chip model: %s\n", ESP.getChipModel());
    Serial.printf("Chip revision: %d\n", ESP.getChipRevision());
    Serial.printf("Chip cores: %d\n", ESP.getChipCores());
    Serial.printf("SDK version: %s\n", ESP.getSdkVersion());
}

void loop() {
}
