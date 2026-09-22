// web_server.cpp

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "wifi_secrets.h"
#include "heartbeat.h"

constexpr unsigned long HEARTBEAT_INTERVAL_MS = 2000;
WebServer server(80);

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
}

void handle_root() {
    Serial.println("GET /");
    server.send(200, "text/plain", "Hello from ESP32");
}

void handle_uptime() {
    Serial.println("GET /uptime");
    server.send(200, "text/plain", String(millis()));
}

void handle_status() {
    Serial.println("GET /api/status");
    String json = "{";
    json += "\"uptime\":";
    json += millis();
    json += ",";
    json += "\"message\":\"Hello from ESP32\"";
    json += "}";
    server.send(200, "application/json", json);
}

void setup_wifi() {
    // Delete old configuration
    WiFi.disconnect(true);
    delay(1000);

    WiFi.onEvent(wifi_station_connected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_CONNECTED);
    WiFi.onEvent(wifi_got_ip, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
    WiFi.onEvent(wifi_disconnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);

    begin_wifi();
}

void setup_server() {
    server.on("/", handle_root);
    server.on("/uptime", handle_uptime);
    server.on("/api/status", handle_status);
    server.begin();
    Serial.println("Web server started");
}

void setup() {
    Serial.begin(115200);
    setup_heartbeat();
    setup_wifi();
    setup_server();
}

void heartbeat() {
    if (update_heartbeat()) {
        Serial.println("Heartbeat");
    }
}

void loop() {
    heartbeat();
    server.handleClient();
}
