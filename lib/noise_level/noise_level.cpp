// noise_level.cpp
#include <algorithm>
#include "noise_level.h"

int calc_noise_level(double raw_intensity, int current_level, int max_levels) {      
    raw_intensity = std::max(MIN_DB, raw_intensity);
    raw_intensity = std::min(MAX_DB, raw_intensity);

    int level = current_level;
    double bounded = raw_intensity - MIN_DB;
    double divisor = (MAX_DB - MIN_DB) / max_levels;

    double lower_threshold = current_level * divisor - HYSTERESIS;
    double upper_threshold = (current_level + 1) * divisor + HYSTERESIS;
    if (   (bounded < lower_threshold) 
        || (bounded > upper_threshold)) {
            level = bounded / divisor;   
    }

    level = std::max(0, level);
    level = std::min(max_levels, level);

    return level;
}

int apply_rate_limit(int current_level, int target_level) {
    // int limit = apply_rise_limit(current_level, target_level);
    // return apply_fall_limit(current_level, target_level);
    // rise limit
    int limit = current_level + RISE_LIMIT;
    if (target_level > limit ) {
        return limit;
    }

    // fall limit
    limit = current_level - 1;
    if (target_level < limit ) {
        return limit;
    }
    return target_level;
}

// Limits the next level to N above the current
int apply_rise_limit(int current_level, int target_level) {
    int limit = current_level + RISE_LIMIT;
    if (target_level > limit ) {
        return limit;
    }
    return target_level;
}

// Limits the next level to N above the current
int apply_fall_limit(int current_level, int target_level) {
    int limit = current_level - 1;
    if (target_level < limit ) {
        return limit;
    }
    return target_level;
}