cmake_minimum_required(VERSION 3.15)

project(PybindDemo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PYBIND11_FINDPYTHON ON)

find_package(Python REQUIRED COMPONENTS Interpreter Development)
find_package(pybind11 REQUIRED)

pybind11_add_module(
    cppbloom
    src/binding.cpp
    src/BloomFilter.cpp
    src/Evaluator.cpp
    src/FeatureExtractor.cpp
)

target_compile_options(cppbloom PRIVATE -O3)