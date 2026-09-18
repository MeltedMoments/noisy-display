// noise_level.h
#pragma once

// Signal processing
constexpr double MIN_DB = 55.0;
constexpr double MAX_DB = 85.0;
// constexpr double RISE_FACTOR = 0.85;
// constexpr double FALL_FACTOR = 0.3;
constexpr double HYSTERESIS = 3.0;

int calc_noise_level(double intensity, int current_level, int max_levels);
