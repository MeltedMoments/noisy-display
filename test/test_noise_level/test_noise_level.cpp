// test_noise_level

#include <unity.h>
#include "noise_level.h"

constexpr int MAX_LEVELS = 8;
void test_level_below_minimum_is_zero() {
    
    int level = calc_noise_level(40.0, 0, MAX_LEVELS);
    TEST_ASSERT_EQUAL_INT(0, level);
}

int main() {
    UNITY_BEGIN();

    RUN_TEST(test_level_below_minimum_is_zero);

    return UNITY_END();
}