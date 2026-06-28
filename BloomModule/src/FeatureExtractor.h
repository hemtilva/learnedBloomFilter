#pragma once

#include <pybind11/numpy.h>
#include <string>
#include <vector>

namespace py = pybind11;

std::vector<int> extract_single_token_hashes(const std::string& text, int num_bins);
py::tuple extract_token_hashes(const std::vector<std::string>& strings, int num_bins);

std::vector<int> extract_single_sparse_ngrams(const std::string& text, int num_bins);
py::tuple extract_sparse_ngrams(const std::vector<std::string>& strings, int num_bins);

py::array_t<float> extract_basic_features(const std::vector<std::string>& strings, int vector_size);