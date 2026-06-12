#pragma once

#include <pybind11/numpy.h>
#include <string>
#include <vector>

namespace py = pybind11;

py::array_t<float> extract_features(
    const std::vector<std::string>& strings,
    int vector_size
);