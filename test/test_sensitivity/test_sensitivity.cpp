// test_sensitivity.cpp

#include <stdio.h>
#include <unity.h>
#include "sensitivity.h"

void test_next_sensitivity_cycles_through_all_levels() {
    Sensitivity start = Sensitivity::LOW_LEVEL;
    
    Sensitivity sensitivity = start;
    
    sensitivity = next_sensitivity(sensitivity);
    TEST_ASSERT_EQUAL(Sensitivity::MEDIUM_LEVEL, sensitivity);
    sensitivity = next_sensitivity(sensitivity);
    TEST_ASSERT_EQUAL(Sensitivity::HIGH_LEVEL, sensitivity);
    
    // Should cycle back to the beginning
    sensitivity = next_sensitivity(sensitivity);
    TEST_ASSERT_EQUAL(start, sensitivity);
}

int main() {
    UNITY_BEGIN();

    RUN_TEST(test_next_sensitivity_cycles_through_all_levels);

    return UNITY_END();
}