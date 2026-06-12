#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "BloomFilter.h"
#include "Evaluator.h"

namespace py = pybind11;

PYBIND11_MODULE(cppbloom, m){
    m.doc() = "Bloom Filter Library in C++";
    
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