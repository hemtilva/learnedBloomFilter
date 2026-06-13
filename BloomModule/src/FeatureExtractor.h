#pragma once

#include <pybind11/numpy.h>
#include <string>
#include <vector>

namespace py = pybind11;

py::tuple extract_sparse_ngrams(const std::vector<std::string>& strings, int num_bins);

py::array_t<float> extract_basic_features(const std::vector<std::string>& strings, int vector_size);