// noise_level.h
#pragma once

#include "sensitivity.h"

// Signal processing
constexpr double MIN_DB = 55.0;
constexpr double MAX_DB = 85.0;

// Levels for min-db
constexpr double MIN_DB_LOW_LEVEL    = 65.0;
constexpr double MIN_DB_MEDIUM_LEVEL = 60.0;
constexpr double MIN_DB_HIGH_LEVEL   = 55.0;

// constexpr double RISE_FACTOR = 0.85;
// constexpr double FALL_FACTOR = 0.3;
constexpr double HYSTERESIS = 3.0;
constexpr int RISE_LIMIT = 1;
constexpr int FALL_LIMIT = 1;

int calc_noise_level(double raw_intensity, int display_level, int max_levels);
int apply_rate_limit(int display_level, int target_level);
double minimum_db(Sensitivity sensitivity);
