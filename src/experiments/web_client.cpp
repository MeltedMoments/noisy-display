// web_client.cpp

#include <Arduino.h>
// #include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "HTTPClient.h"
#include "wifi_config.h"
#include "heartbeat.h"

constexpr unsigned long HEARTBEAT_INTERVAL_MS = 2000;
constexpr unsigned long REQUEST_INTERVAL_MS = 5000;
unsigned long last_request_time = 0;

WiFiClientSecure client;
HTTPClient http;

void begin_wifi() {
    Serial.println("Waiting for WiFi connection");
    Serial.printf("Connecting to ");
    Serial.println(WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD, 6);
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

void setup_wifi() {
    // Delete old configuration
    WiFi.disconnect(true);
    delay(1000);

    WiFi.onEvent(wifi_station_connected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_CONNECTED);
    WiFi.onEvent(wifi_got_ip, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
    WiFi.onEvent(wifi_disconnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);

    begin_wifi();
}

void setup() {
    Serial.begin(115200);
    setup_heartbeat();
    setup_wifi();
}

void make_http_request() {
    http.begin("http://example.com");
    int status = http.GET();

    Serial.printf("HTTP status: %d\r\n", status);

    if (status > 0) {
        String body = http.getString();
        Serial.println("Body");
        Serial.println(body);
    } else {
        Serial.printf("Request failed: %s\n", http.errorToString(status).c_str());
    }
    http.end();
}

void display_json(const String& json) {
    JsonDocument doc;

    DeserializationError error = deserializeJson(doc, json);
    if (error) {
        Serial.printf("JSON parse error: %s\r\n", error.c_str());
        return;
    }
    int user_id = doc["userId"].as<int>();
    int id = doc["id"].as<int>();
    const char* title = doc["title"].as<const char*>();
    bool completed = doc["completed"].as<bool>();

    Serial.printf("userId: %d\r\n", user_id);
    Serial.printf("id: %d\r\n", id);
    Serial.printf("title: %s\r\n", title);
    Serial.printf("completed: %s\r\n", completed ? "true" : "false");
}

void make_json_request() {
    client.setInsecure();
    http.begin(client, "https://jsonplaceholder.typicode.com/todos/1");
    int status = http.GET();

    Serial.printf("HTTP status: %d\r\n", status);

    if (status > 0) {
        String body = http.getString();
        // Serial.println("Body");
        Serial.println(body);
        display_json(body);
    } else {
        Serial.printf("Request failed: %s\n", http.errorToString(status).c_str());
    }
    http.end();
}

void make_request() {
    unsigned long now = millis();
    if (now - last_request_time < REQUEST_INTERVAL_MS) {
        return;
    }
    if (! WiFi.isConnected()) {
        // Serial.println("No connection for request");
        return;
    }
    last_request_time = now;
    make_json_request();
    make_http_request();
}

void heartbeat() {
    if (update_heartbeat()) {
        Serial.println("Heartbeat");
    }
}

void loop() {
    heartbeat();
    make_request();
    // server.handleClient();
}
