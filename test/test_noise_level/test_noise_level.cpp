// test_noise_level
#include <stdio.h>
#include <unity.h>
#include "noise_level.h"

constexpr int MAX_LEVELS = 8;

struct NoiseLevelTestCase {
    double intensity;
    int current_level;
    int expected_level;
};

struct SwingLevelTestCase {
    int current;
    int target;
    int expected;
};

void assert_noise_level(const NoiseLevelTestCase &test_case) {
    int actual = calc_noise_level(
        test_case.intensity,
        test_case.current_level,
        MAX_LEVELS
    );

    char message[100];
    snprintf(
        message,
        sizeof(message),
        "intensity: %.2f current: %d",
        test_case.intensity,
        test_case.current_level
    );
    TEST_ASSERT_EQUAL_INT_MESSAGE(
        test_case.expected_level, 
        actual,
        message
    );
}

void assert_swing_limit(const SwingLevelTestCase &test_case, int actual) {
    char message[100];
    snprintf(
        message,
        sizeof(message),
        "current: %d target: %d",
        test_case.current,
        test_case.target
    );
    TEST_ASSERT_EQUAL_INT_MESSAGE(
        test_case.expected, 
        actual,
        message
    );
}

void assert_rise_limit(const SwingLevelTestCase &test_case) {
    int actual = apply_rise_limit(
        test_case.current,
        test_case.target
    );

    assert_swing_limit(test_case, actual);
}

void assert_fall_limit(const SwingLevelTestCase &test_case) {
    int actual = apply_fall_limit(
        test_case.current,
        test_case.target
    );
    assert_swing_limit(test_case, actual);

    // char message[100];
    // snprintf(
    //     message,
    //     sizeof(message),
    //     "current: %d target: %d",
    //     test_case.current,
    //     test_case.target
    // );
    // TEST_ASSERT_EQUAL_INT_MESSAGE(
    //     test_case.expected, 
    //     actual,
    //     message
    // );
}

template <size_t N>
void assert_noise_level_matrix(const NoiseLevelTestCase (&cases)[N]) {
    for (const auto &test_case : cases) {
        assert_noise_level(test_case);
    }
}

template <size_t N>
void assert_rise_limit_matrix(const SwingLevelTestCase (&cases)[N]) {
    for (const auto &test_case : cases) {
        assert_rise_limit(test_case);
    }
}

template <size_t N>
void assert_fall_limit_matrix(const SwingLevelTestCase (&cases)[N]) {
    for (const auto &test_case : cases) {
        assert_fall_limit(test_case);
    }
}

void test_level_below_minimum_is_zero() {
    int level = calc_noise_level(MIN_DB - 15.0, 0, MAX_LEVELS);
    TEST_ASSERT_EQUAL_INT(0, level);
}

void test_level_above_maximum_is_maximum() {
    int level = calc_noise_level((MAX_DB + 10.0), MAX_LEVELS, MAX_LEVELS);
    TEST_ASSERT_EQUAL_INT(MAX_LEVELS, level);
}

void test_basic_matrix() {
    NoiseLevelTestCase cases[] = {
        {MIN_DB - 10.0, 0, 0},             // below MIN_DB clamps to minimum       
        {MIN_DB,        0, 0} ,            // exactly minimum                      
        {MIN_DB + 1.0,  0, 0},             // remains level 0                      
        {MAX_DB,        0, MAX_LEVELS},    // maximum                              
        {MAX_DB + 10.0, 0, MAX_LEVELS},    // above maximum clamps to maximum              {55, 0, 0},
        // {56, 0, 0},
        // {64, 0, 3},
    };
    assert_noise_level_matrix(cases);
}

void test_first_implementation() {
    double divisor = (MAX_DB - MIN_DB) / MAX_LEVELS;

    NoiseLevelTestCase cases[] = {
        {MIN_DB + divisor + 1.0, 0, 0},     // not sufficiently above level-0 hysteresis       
        {MIN_DB + divisor + 2.0, 0, 0},     // exactly upper hysteresis threshold
        {MIN_DB + divisor + 4.0, 0, 2},     // leaves 0 — and skips 1
        {MIN_DB + (3*divisor),   0, 3},     // jumps directly upwards        
        {MIN_DB + divisor - 0.1, 0, 0},
        {MIN_DB + divisor,       0, 0},
        {MIN_DB + divisor + 0.5, 0, 0},
    };
    assert_noise_level_matrix(cases);
}

void test_hysteresis_matrix() {
    NoiseLevelTestCase cases[] = {
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
    };
    assert_noise_level_matrix(cases);
}

void test_rise_limit_matrix() {
    SwingLevelTestCase cases[] = {
        {0, 0, 0},             // unchanged            
        {2, 2, 2},             // unchanged            
        {2, 3, 3},             // rise one             
        {2, 4, 3},             // rise limited         
        {2, 8, 3},             // large spike limited  
        {7, 8, 8},             // rise to maximum      
        {5, 4, 4},             // fall one             
        {5, 2, 2},             // large fall immediate 
        {8, 0, 0},             // fall to minimum      
    };
    assert_rise_limit_matrix(cases);
}

void test_fall_limit_matrix() {
    SwingLevelTestCase cases[] = {
        {0, 0, 0},             // unchanged            
        {3, 3, 3},             // unchanged            
        {3, 2, 2},             // fall one             
        {3, 1, 2},             // fall limited         
        {8, 1, 7},             // large spike limited  
        {1, 0, 0},             // fall to minimum      
        {5, 6, 6},             // rise one             
        {5, 2, 4},             // large fall immediate 
        {8, 0, 7},             // fall to minimum              
    };
    assert_fall_limit_matrix(cases);
}

void test_single_loud_spike_does_not_max() {
    int level = 1;
    level = apply_rise_limit(level, 1);   // quiet
    level = apply_rise_limit(level, 1);   // quiet
    level = apply_rise_limit(level, 8);   // bang!

    TEST_ASSERT_EQUAL_INT(2, level);
    
    level = apply_rise_limit(level, 1);   // quiet
    TEST_ASSERT_EQUAL_INT(1, level);
}

void test_sustained_loud_spike_maxes() {
    int level = 1;
    level = apply_rise_limit(level, 8);   // bang!
    TEST_ASSERT_EQUAL_INT(2, level);
    level = apply_rise_limit(level, 8);   // bang!
    TEST_ASSERT_EQUAL_INT(3, level);
    level = apply_rise_limit(level, 8);   // bang!
    TEST_ASSERT_EQUAL_INT(4, level);
    level = apply_rise_limit(level, 8);   // bang!
    TEST_ASSERT_EQUAL_INT(5, level);
}

int main() {
    UNITY_BEGIN();

    RUN_TEST(test_level_below_minimum_is_zero);
    RUN_TEST(test_level_above_maximum_is_maximum);
    RUN_TEST(test_basic_matrix);
    RUN_TEST(test_first_implementation);
    RUN_TEST(test_hysteresis_matrix);
    RUN_TEST(test_rise_limit_matrix);
    RUN_TEST(test_fall_limit_matrix);
    RUN_TEST(test_single_loud_spike_does_not_max);
    RUN_TEST(test_sustained_loud_spike_maxes);

    return UNITY_END();
}