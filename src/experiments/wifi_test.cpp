// wifi_test.cpp

#include <Arduino.h>
#include <WiFi.h>
#include "wifi_secrets.h"


void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println("== Wifi Test ==");
    Serial.printf("Connecting ");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("Connected!");
    Serial.println(WiFi.localIP());
}

void loop() {
    
}
