#include <iostream>
#include "sand_calculator.hpp"
#include "omp.h"


void calculate_sand_CUDA(float* host_input);

void SandCalculator<SandCalculatorType::Stupid>::calculateSand(std::vector<float>& input) {
    std::vector<float> result(input.size(), 0.0);

    for (int x = 0; x < 160; ++x) {
        result[x] = input[x];
    }

    for (int y = 1; y < 120; ++y) {
        for (int x = 1; x < 159; ++x) {
            if (input[y * 160 + x] != 0.0) {
                if (input[(y - 1) * 160 + x] == 0.0) {
                    result[(y - 1) * 160 + x] = 1.0;
                } else if (input[(y - 1) * 160 + x - 1] == 0.0) {
                    result[(y - 1) * 160 + x - 1] = 1.0;
                } else if (input[(y - 1) * 160 + x + 1] == 0.0) {
                    result[(y - 1) * 160 + x + 1] = 1.0;
                } else {
                    result[y * 160 + x] = 1.0;
                }
            }
        }
    }

    input = result;
}

void SandCalculator<SandCalculatorType::CUDA>::calculateSand(std::vector<float>& input) {
    calculate_sand_CUDA(input.data());
}

void SandCalculator<SandCalculatorType::OpenMP>::calculateSand(std::vector<float>& input) {
    std::vector<float> result(input.size(), 0.0);

    int thread_id;

    #pragma omp parallel private(thread_id) shared(input, result)
    {
        thread_id = omp_get_thread_num();
        int threads_amount = omp_get_max_threads();

        #pragma omp for
        for (int x = 0; x < 160; ++x) {
            result[x] = input[x];
        }

        for (int y = 1; y < 120; ++y) {
            #pragma omp for
            for (int x = 1; x < 159; ++x) {
                if (input[y * 160 + x] != 0.0) {
                    if (input[(y - 1) * 160 + x] == 0.0) {
                        result[(y - 1) * 160 + x] = 1.0;
                    } else if (input[(y - 1) * 160 + x - 1] == 0.0) {
                        result[(y - 1) * 160 + x - 1] = 1.0;
                    } else if (input[(y - 1) * 160 + x + 1] == 0.0) {
                        result[(y - 1) * 160 + x + 1] = 1.0;
                    } else {
                        result[y * 160 + x] = 1.0;
                    }
                }
            }

            #pragma omp barrier
        }
    }

    input = result;
}
