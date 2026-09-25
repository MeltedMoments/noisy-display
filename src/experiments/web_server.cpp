// web_server.cpp

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <WebServer.h>
#include "wifi_config.h"
#include "heartbeat.h"

constexpr unsigned long HEARTBEAT_INTERVAL_MS = 2000;
WebServer server(80);
bool led_state = false;
constexpr int ONBOARD_LED = 38;


Adafruit_NeoPixel pixel(
    1, ONBOARD_LED, NEO_GRB + NEO_KHZ800
);

void set_pixel(bool led_on) {
    if (led_on) {
        pixel.setPixelColor(0, pixel.Color(255, 0, 255));
    } else {
        pixel.setPixelColor(0, pixel.Color(0, 0, 255));
    }
    pixel.show();
}

void begin_wifi() {
    Serial.println("Waiting for WiFi connection");
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

void send_json(JsonDocument doc) {
    String json;
    serializeJson(doc, json);

    server.send(200, "application/json", json);
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
    JsonDocument doc;
    doc["uptime_ms"] = millis();
    doc["wifi_connected"] = WiFi.isConnected();
    doc["rssi"] = WiFi.RSSI();
    doc["ip"] = WiFi.localIP().toString(); 
    doc["auto_reconnect"] = WiFi.getAutoReconnect();
    doc["led_on"] = led_state; 

    send_json(doc);
    // String json;
    // serializeJson(doc, json);

    // server.send(200, "application/json", json);
}

void handle_hello() {
    Serial.println("GET /api/hello");
    String name_arg = "name";
    String name = "stranger";
    if (server.hasArg(name_arg)) {
        name = server.arg(name_arg);
    }

    JsonDocument doc;
    doc["uptime_ms"] = millis();
    String message = "Hello ";
    message += name;
    doc["message"] = message;

    send_json(doc);
    // String json;
    // serializeJson(doc, json);

    // server.send(200, "application/json", json);
}

void handle_led() {
    Serial.println("POST /api/led");
    String led_arg = "state";
    String state = "off";
    if (server.hasArg(led_arg)) {
        state = server.arg(led_arg);
    }
    led_state = (state == "on") ? true : false;
    set_pixel(led_state);

    JsonDocument doc;
    doc["led_on"] = led_state;

    send_json(doc);
 
    // String json;
    // serializeJson(doc, json);

    // server.send(200, "application/json", json);

    //     case "on": led_state = true;
    //     case "off": 
    //     default: led_state = false;
    // }
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
    server.on("/api/hello", handle_hello);
    server.on("/api/led", HTTP_POST, handle_led);
    server.begin();
    Serial.println("Web server started");
}

void setup() {
    Serial.begin(115200);
    setup_heartbeat();
    setup_wifi();
    setup_server();
    pixel.begin();
    pixel.setBrightness(20);
    pixel.clear();
    pixel.show();
    set_pixel(led_state);
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
