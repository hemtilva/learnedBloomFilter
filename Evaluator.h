#include <cstdlib>
#include <cstdint>
#include <vector>
#include <bitset>
#include <string>
#include <random>

#ifndef EVAL_H
#define EVAL_H

struct Result{
    size_t insertCount = 0;
    size_t negativeQueryCount = 0;
    size_t truePositive = 0;
    size_t falsePositive = 0;
    size_t trueNegative = 0;
    size_t falseNegative = 0;

    size_t timeTakenInMs = 0;
};

class Evaluator{
private:
    static std::mt19937 rng;
public:
    std::vector<std::string> generateRandomStrings(size_t count, size_t maxLenOfStrings, const std::string& path);
    Result runSingleExperiment(size_t elemenCount, double fpr,const std::string& pathReal,const std::string& pathFake);
    size_t computeTheoryBits(size_t elementCount, double fpr);
    size_t optimalHashes(size_t bits, size_t elementCount);
};

#endif