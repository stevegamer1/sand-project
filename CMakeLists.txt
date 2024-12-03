cmake_minimum_required(VERSION 3.16)
project(sand_project LANGUAGES C CXX CUDA)

add_subdirectory(glfw-3.4)
add_subdirectory(src)

set_target_properties(main PROPERTIES POSITION_INDEPENDENT_CODE ON)  # CUDA needs this

target_include_directories(main PUBLIC include)

set(LIBS glfw)
target_link_libraries(main PUBLIC glfw)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(main PUBLIC OpenMP::OpenMP_CXX)
endif()

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set(TEST_FILES
    src/sand_calculator.cpp
    src/sand_calculator_CUDA.cu
    src/tests.cpp)

add_executable(SandTest ${TEST_FILES})
target_include_directories(SandTest PUBLIC include)
target_link_libraries(SandTest PUBLIC
    OpenMP::OpenMP_CXX
    gtest_main)
set_target_properties(SandTest PROPERTIES POSITION_INDEPENDENT_CODE ON)  # CUDA needs this

add_test(
    NAME SandTest
    COMMAND SandTest
)
