#include <iostream>
#include "sand_calculator.hpp"


void checkErr() {
    cudaError_t err = cudaGetLastError();
    if (err != cudaSuccess) {
        std::cout << "CUDA ERROR: " << cudaGetErrorString(err) << "\n";
    }
}

__global__ void copy_low_row_CUDA(float* input, float* result) {
    int x = 1 + blockDim.x * blockIdx.x + threadIdx.x;
    result[x] = input[x];
}

__global__ void sand_kernel_CUDA(float* input, float* result) {
    int x = 1 + blockDim.x * blockIdx.x + threadIdx.x;
    int y = 1 + blockDim.y * blockIdx.y + threadIdx.y;
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


void calculate_sand_CUDA(float* host_input) {
    float* input;
    float* result;
    cudaMalloc(&input, 4 * 160 * 120);
    checkErr();
    cudaMalloc(&result, 4 * 160 * 120);
    checkErr();
    cudaMemcpy(input, host_input, 4 * 160 * 120, cudaMemcpyHostToDevice);
    checkErr();

    copy_low_row_CUDA<<<158, 1>>>(input, result);
    checkErr();
    sand_kernel_CUDA<<<158, 119>>>(input, result);
    checkErr();
    cudaDeviceSynchronize();
    checkErr();

    cudaMemcpy(host_input, result, 4 * 160 * 120, cudaMemcpyDeviceToHost);
    checkErr();
    cudaFree(input);
    checkErr();
    cudaFree(result);
    checkErr();
}
