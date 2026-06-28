#ifndef EVAL_H
#define EVAL_H

#include <cstdlib>
#include <cstdint>
#include <vector>
#include <bitset>
#include <string>
#include <random>
#include <iostream>
#include "LearnedBloomFilter.h"

struct Result{
    size_t insertCount = 0;
    size_t negativeQueryCount = 0;
    size_t truePositive = 0;
    size_t falsePositive = 0;
    size_t trueNegative = 0;
    size_t falseNegative = 0;

    size_t timeTakenInNs = 0;
    size_t memoryNeeded = 0;
};

std::ostream& operator<<(std::ostream& os,const Result& res);

class Evaluator{
private:
    static std::mt19937 rng;
public:
    std::vector<std::string> generateRandomStrings(size_t count, size_t maxLenOfStrings, const std::string& path);
    Result runSingleExperiment(size_t elemenCount, double fpr,const std::string& pathReal,const std::string& pathFake);
    Result runLearnedExperiment(
        const std::vector<double>& weights, 
        double bias, 
        double threshold, 
        int num_bins,
        size_t backup_size,
        double backup_fpr,
        const std::string& pos_file,
        const std::string& neg_file
    );
};

#endif