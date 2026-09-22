// wifi_events.cpp

#include <Arduino.h>
#include <WiFi.h>
#include "wifi_secrets.h"
#include "heartbeat.h"

constexpr unsigned long HEARTBEAT_INTERVAL_MS = 2000;

void begin_wifi() {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.println("Waiting for WiFi connection");
}

void wifi_station_connected(WiFiEvent_t event, WiFiEventInfo_t info){
    Serial.println("Connected to AccessPoint!");
}

void wifi_got_ip(WiFiEvent_t event, WiFiEventInfo_t info){
    Serial.printf("WiFi connected. IP: ");
    Serial.println(WiFi.localIP());
}

void wifi_disconnected(WiFiEvent_t event, WiFiEventInfo_t info){
    Serial.println("Disconnected from AccessPoint");
    Serial.printf("WiFi connection lost. Reason: ");
    Serial.println(info.wifi_sta_disconnected.reason);
    
    Serial.println("Reconnecting...");
    begin_wifi();
    // WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

}

void setup_wifi() {
    // Delete old configuration
    WiFi.disconnect(true);
    delay(1000);

    WiFi.onEvent(wifi_station_connected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_CONNECTED);
    WiFi.onEvent(wifi_got_ip, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
    WiFi.onEvent(wifi_disconnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);

    begin_wifi();
    // WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    // Serial.println("Waiting for WiFi");
}

void setup() {
    Serial.begin(115200);
    setup_heartbeat();
    setup_wifi();
}

void heartbeat() {
    if (update_heartbeat()) {
        Serial.println("Heartbeat");
    }
}

// void check_connection() {
//     unsigned long now = millis();
//     if (now - last_connection_time < CONNECTION_INTERVAL_MS) {
//         return;
//     }

//     if (WiFi.status() == WL_CONNECTED) {
//         Serial.printf("Connected ");
//         Serial.println(WiFi.localIP()); 
//     } else {
//         Serial.println("Uh-oh");
//     }
//     last_connection_time = now;
// }


void loop() {
    heartbeat();
    // check_connection();
}
