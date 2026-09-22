// sensitivity.h
#pragma once

enum class Sensitivity {
    LOW_LEVEL,
    MEDIUM_LEVEL,
    HIGH_LEVEL
};

Sensitivity next_sensitivity(Sensitivity current);
const char* sensitivity_name(Sensitivity sensitivity);
