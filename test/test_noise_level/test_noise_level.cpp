// test_noise_level
#include <stdio.h>
#include <unity.h>
#include "noise_level.h"

constexpr int MAX_LEVELS = 8;

struct TestCase {
    double intensity;
    int current_level;
    int expected_level;
};

void test_level_below_minimum_is_zero() {
    int level = calc_noise_level(MIN_DB - 15.0, 0, MAX_LEVELS);
    TEST_ASSERT_EQUAL_INT(0, level);
}

void test_level_above_maximum_is_maximum() {
    int level = calc_noise_level((MAX_DB + 10.0), MAX_LEVELS, MAX_LEVELS);
    TEST_ASSERT_EQUAL_INT(MAX_LEVELS, level);
}

void test_basic_matrix() {
    TestCase cases[] = {
        {MIN_DB - 10.0, 0, 0},             // below MIN_DB clamps to minimum       
        {MIN_DB,        0, 0} ,            // exactly minimum                      
        {MIN_DB + 1.0,  0, 0},             // remains level 0                      
        {MAX_DB,        0, MAX_LEVELS},    // maximum                              
        {MAX_DB + 10.0, 0, MAX_LEVELS},    // above maximum clamps to maximum              {55, 0, 0},
        // {56, 0, 0},
        // {64, 0, 3},
    };

    for (const auto &test_case : cases) {
        int actual = calc_noise_level(
            test_case.intensity,
            test_case.current_level,
            MAX_LEVELS
        );
        TEST_ASSERT_EQUAL_INT(test_case.expected_level, actual);
    }
}

void test_first_implementation() {
    double divisor = (MAX_DB - MIN_DB) / MAX_LEVELS;

    TestCase cases[] = {
        {MIN_DB + divisor + 1.0, 0, 0},     // not sufficiently above level-0 hysteresis       
        {MIN_DB + divisor + 2.0, 0, 0},     // exactly upper hysteresis threshold
        {MIN_DB + divisor + 3.0, 0, 2},     // finally leaves 0 — and skips 1
        {MIN_DB + (3*divisor)  , 0, 3},     // jumps directly upwards        

        {MIN_DB + divisor - 0.1, 0, 0},
        {MIN_DB + divisor,       0, 0},
        {MIN_DB + divisor + 0.5, 0, 1},
    };

    for (const auto &test_case : cases) {
        int actual = calc_noise_level(
            test_case.intensity,
            test_case.current_level,
            MAX_LEVELS
        );
        TEST_ASSERT_EQUAL_INT(test_case.expected_level, actual);
    }
}

void test_hysteresis_matrix() {
    TestCase cases[] = {
        {57, 2, 0}, 
        {59.4, 2, 1},  // just below lower threshold
        {59.5, 2, 2},  // exactly threshold: hold
        {59.6, 2, 2},  // just inside hysteresis        
        {59, 0, 0}, 
        {64, 2, 2}, 
        {67, 2, 2}, 
        {69.2, 2, 2},  // below threshold: hold
        {69.25, 2, 2}, // exactly threshold: hold
        {69.3, 2, 3},  // above threshold: rise        
        // {68, 2, 4}
    };

    for (const auto &test_case : cases) {
        int actual = calc_noise_level(
            test_case.intensity,
            test_case.current_level,
            MAX_LEVELS
        );

        char message[100];
        snprintf(
            message,
            sizeof(message),
            "intensity=%.2f current=%d",
            test_case.intensity,
            test_case.current_level
        );
        TEST_ASSERT_EQUAL_INT_MESSAGE(
            test_case.expected_level, 
            actual,
            message
        );
    }
}

int main() {
    UNITY_BEGIN();

    RUN_TEST(test_level_below_minimum_is_zero);
    RUN_TEST(test_level_above_maximum_is_maximum);
    RUN_TEST(test_basic_matrix);
    RUN_TEST(test_first_implementation);
    RUN_TEST(test_hysteresis_matrix);

    return UNITY_END();
}