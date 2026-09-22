// wifi_scanner.cpp

#include <Arduino.h>
#include <WiFi.h>

void wifi_scan() {
    Serial.println("Scanning WiFi Networks...");

    int count = WiFi.scanNetworks();
    for (int i = 0; i < count; i++) {
        Serial.printf(
            "%2d: %-30s RSSI: %d\n",
            i,
            WiFi.SSID(i).c_str(),
            WiFi.RSSI(i)
        );        
    }
    Serial.printf("Finished scanning %d networks\n", count);
}

void setup() {
    Serial.begin(115200);
    delay(1000);
}

void loop() {
    wifi_scan();
    delay(5000);
}
