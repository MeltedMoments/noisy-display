// wifi_test.cpp

#include <Arduino.h>
#include <WiFi.h>
#include "wifi_secrets.h"
#include "heartbeat.h"

constexpr unsigned long HEARTBEAT_INTERVAL_MS = 1000;
constexpr unsigned long CONNECTION_INTERVAL_MS = 1000;

unsigned long last_connection_time = 0;

void setup_wifi() {
    Serial.printf("Connecting ");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("Connected!");
    last_connection_time = millis();
    Serial.println(WiFi.localIP()); 
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    setup_wifi();
    setup_heartbeat();
}

void heartbeat() {
    if (update_heartbeat()) {
        Serial.println("Heartbeat");
    }
}

void check_connection() {
    unsigned long now = millis();
    if (now - last_connection_time < CONNECTION_INTERVAL_MS) {
        return;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.printf("Connected ");
        Serial.println(WiFi.localIP()); 
    } else {
        Serial.println("Uh-oh");
    }
    last_connection_time = now;
}


void loop() {
    heartbeat();
    check_connection();
}
