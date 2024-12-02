#include <vector>
#include "gtest/gtest.h"
#include "sand_calculator.hpp"


#define EXPECT_VECTORS_EQ(v1, v2) { \
    EXPECT_EQ(v1.size(), v2.size()); \
    for (size_t i = 0; i < v1.size(); ++i) { \
        EXPECT_EQ(v1[i], v2[i]) << "Where i = " << i; \
    } \
}


template<SandCalculatorType CalcType>
void TestSand() {
    // No sand.
    std::vector<float> field(160 * 120);
    std::vector<float> expected(160 * 120);
    SandCalculator<CalcType>::calculateSand(field);
    EXPECT_VECTORS_EQ(expected, field);

    // Sand falls one step.
    int y = 1;
    int x = 80;
    field[160 * y + x] = 1.0;
    SandCalculator<CalcType>::calculateSand(field);
    expected[160 * (y - 1) + x] = 1.0;
    EXPECT_VECTORS_EQ(expected, field);

    // Sand collision with other sand and floor (y=0).
    field[160 * y + x] = 1.0;
    SandCalculator<CalcType>::calculateSand(field);
    expected[160 * (y - 1) + (x - 1)] = 1.0;
    EXPECT_VECTORS_EQ(expected, field);

    // Sand collision, but sand falls to the right.
    field[160 * y + x] = 1.0;
    SandCalculator<CalcType>::calculateSand(field);
    expected[160 * (y - 1) + (x + 1)] = 1.0;
    EXPECT_VECTORS_EQ(expected, field);

    // Sand collision and the new particle stays where it was.
    field[160 * y + x] = 1.0;
    SandCalculator<CalcType>::calculateSand(field);
    expected[160 * y + x] = 1.0;
    EXPECT_VECTORS_EQ(expected, field);

    // Nothing happens.
    SandCalculator<CalcType>::calculateSand(field);
    EXPECT_VECTORS_EQ(expected, field);
}


TEST(StupidSand, Basic) {
    TestSand<SandCalculatorType::Stupid>();
}


TEST(CUDASand, Basic) {
    TestSand<SandCalculatorType::CUDA>();
}


TEST(OpenMPSand, Basic) {
    TestSand<SandCalculatorType::OpenMP>();
}
