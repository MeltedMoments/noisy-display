// test_sensitivity.cpp

#include <stdio.h>
#include <unity.h>
#include "sensitivity.h"

// constexpr int MAX_LEVELS = 8;

// struct NoiseLevelTestCase {
//     double intensity;
//     int current_level;
//     int expected_level;
// };
void test_next_sensitivity_cycles_from_max_to_low() {

    int level = calc_noise_level(MIN_DB - 15.0, 0, MAX_LEVELS);
    TEST_ASSERT_EQUAL_INT(0, level);
}

int main() {
    UNITY_BEGIN();

    RUN_TEST(test_level_below_minimum_is_zero);

    return UNITY_END();
}