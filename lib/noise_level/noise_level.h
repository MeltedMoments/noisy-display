// noise_level.h
#pragma once

// Signal processing
constexpr double MIN_DB = 55.0;
constexpr double MAX_DB = 85.0;
// constexpr double RISE_FACTOR = 0.85;
// constexpr double FALL_FACTOR = 0.3;
constexpr double HYSTERESIS = 3.0;
constexpr int RISE_LIMIT = 1;

int calc_noise_level(double raw_intensity, int current_level, int max_levels);
int apply_rise_limit(int current_level, int target_level);
