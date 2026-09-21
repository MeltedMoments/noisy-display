/** 
 *  sensitivity.cpp
*/

#include "sensitivity.h"
  
Sensitivity next_sensitivity(Sensitivity current) {
    switch (current) {
        case Sensitivity::LOW_LEVEL:
            return Sensitivity::MEDIUM_LEVEL;
        case Sensitivity::MEDIUM_LEVEL:
            return Sensitivity::HIGH_LEVEL;
        case Sensitivity::HIGH_LEVEL:
            return Sensitivity::LOW_LEVEL;
    }
    // Fallback
    return Sensitivity::MEDIUM_LEVEL;
}

const char* sensitivity_name(Sensitivity sensitivity) {
    switch (sensitivity) {
        case Sensitivity::LOW_LEVEL:
            return "Low";
        case Sensitivity::MEDIUM_LEVEL:
            return "Medium";
        case Sensitivity::HIGH_LEVEL:
            return "High";
    }
    // Fallback
    return "UNKNOWN";
}
