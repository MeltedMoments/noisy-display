// noisy_config.h
#pragma once

// Neopixel stick
constexpr int PIXEL_PIN = 4;
constexpr int PIXEL_COUNT = 8;
constexpr int INIT_BRIGHTNESS = 20;

// INMP441 microphone
constexpr int I2S_SD = 16;
constexpr int I2S_SCK = 17;
constexpr int I2S_WS = 18;

// Waveshare OLED 
constexpr int OLED_MOSI = 11;       // SPI MOSI/data
constexpr int OLED_CLK  = 12;       // SPI clock
constexpr int OLED_CS   = 10;       // chip select
constexpr int OLED_DC   = 9;        // data/command
constexpr int OLED_RST  = 8;        // reset

constexpr unsigned long HEARTBEAT_INTERVAL_MS = 1000;
