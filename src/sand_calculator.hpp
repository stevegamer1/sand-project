#pragma once
#include <vector>

enum class SandCalculatorType {
    CUDA,
    OpenMP,
    Stupid
};

template <SandCalculatorType typ>
class SandCalculator;

template<>
class SandCalculator<SandCalculatorType::Stupid> {
public:
    static void calculateSand(std::vector<float>& input);
};

template<>
class SandCalculator<SandCalculatorType::CUDA> {
public:
    static void calculateSand(std::vector<float>& input);
};

template<>
class SandCalculator<SandCalculatorType::OpenMP> {
public:
    static void calculateSand(std::vector<float>& input);
};
