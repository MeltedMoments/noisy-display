// microphone.h
#pragma once
#include <driver/i2s.h>

constexpr i2s_port_t I2S_PORT = I2S_NUM_0;
constexpr int SAMPLE_COUNT = 4096;
constexpr double FULL_SCALE = 33554432.0;  // 2^25    

extern int32_t samples[];

void setup_microphone();
double measure_sound_level();
