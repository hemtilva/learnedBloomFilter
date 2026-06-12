#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#include "FeatureExtractor.h"
#include "BloomFilter.h"
#include "Evaluator.h"

namespace py = pybind11;

py::tuple extract_sparse_ngrams_cpp(const std::vector<std::string>& strings, int num_bins) {
    std::vector<float> data;
    std::vector<int> indices;
    std::vector<int> indptr;

    // CSR matrices always start with an indptr of 0
    indptr.push_back(0);

    for (const auto& s : strings) {
        size_t s_len = s.length();

        if (s_len >= 3) {
            // Fast Polynomial Hash for 3-grams
            for (size_t i = 0; i <= s_len - 3; i++) {
                uint32_t h = (static_cast<uint8_t>(s[i]) * 961) + 
                             (static_cast<uint8_t>(s[i+1]) * 31) + 
                             static_cast<uint8_t>(s[i+2]);
                
                indices.push_back(h % num_bins);
                data.push_back(1.0f);
            }
        }

        if (s_len >= 4) {
            // Fast Polynomial Hash for 4-grams (offset to prevent index overlap)
            for (size_t i = 0; i <= s_len - 4; i++) {
                uint32_t h = (static_cast<uint8_t>(s[i]) * 29791) + 
                             (static_cast<uint8_t>(s[i+1]) * 961) + 
                             (static_cast<uint8_t>(s[i+2]) * 31) + 
                             static_cast<uint8_t>(s[i+3]);
                
                indices.push_back((h ^ 0x5bd1e995) % num_bins);
                data.push_back(1.0f);
            }
        }

        // Record the end of this row
        indptr.push_back(indices.size());
    }

    // Safely copy C++ vectors to NumPy arrays to prevent memory leaks
    auto py_data = py::array_t<float>(data.size());
    std::memcpy(py_data.mutable_data(), data.data(), data.size() * sizeof(float));

    auto py_indices = py::array_t<int>(indices.size());
    std::memcpy(py_indices.mutable_data(), indices.data(), indices.size() * sizeof(int));

    auto py_indptr = py::array_t<int>(indptr.size());
    std::memcpy(py_indptr.mutable_data(), indptr.data(), indptr.size() * sizeof(int));

    return py::make_tuple(py_data, py_indices, py_indptr);
}

PYBIND11_MODULE(cppbloom, m){
    m.doc() = "Bloom Filter Library in C++";

    m.def("extract_features", &extract_features);
    m.def("extract_sparse_ngrams_cpp", &extract_sparse_ngrams_cpp);
    
    py::class_<BloomFilter>(m, "BloomFilter")
        .def(py::init<size_t,double>())
        .def("add",&BloomFilter::AddToFilter)
        .def("contains",&BloomFilter::IsInFilter)
        .def("__contains__",&BloomFilter::IsInFilter)
        .def("memory_cost",&BloomFilter::getMemoryCost);

    py::class_<Result>(m, "Result")
        .def_readonly("insertCount",&Result::insertCount)
        .def_readonly("negativeQueryCount",&Result::negativeQueryCount)
        .def_readonly("truePositive",&Result::truePositive)
        .def_readonly("falsePositive",&Result::falsePositive)
        .def_readonly("trueNegative",&Result::trueNegative)
        .def_readonly("falseNegative",&Result::falseNegative)
        .def_readonly("timeTakenInNs",&Result::timeTakenInNs)
        .def_readonly("memoryNeeded",&Result::memoryNeeded)
        .def("__repr__", [](const Result& r) {
            std::ostringstream oss;
            oss << r;
            return oss.str();
        });

    py::class_<Evaluator>(m,"Evaluator")
        .def(py::init<>())
        .def("generate_random_strings",&Evaluator::generateRandomStrings)
        .def("run_single_experiment",&Evaluator::runSingleExperiment);
}