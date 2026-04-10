#include "Evaluator.h"
#include "BloomFilter.h"
#include <random>
#include <cmath>
#include <iostream>
#include <chrono>
using namespace std;
using namespace std::chrono;

mt19937 Evaluator::rng(random_device{}());

vector<string> Evaluator::generateRandomStrings(size_t count, size_t maxLenOfStrings){
    vector<string> result;
    result.reserve(count);

    const string charset = 
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "0123456789";

    uniform_int_distribution<size_t> lengthDist(1, maxLenOfStrings);
    uniform_int_distribution<size_t> charDist(0, charset.size() - 1);

    for(size_t i = 0; i < count; i++)
    {
        size_t len = lengthDist(rng);

        string s;
        s.reserve(len);

        for(size_t j = 0; j < len; j++)
        {
            s += charset[charDist(rng)];
        }

        result.push_back(move(s));
    }

    return result;
}

Result Evaluator::runSingleExperiment(size_t elemenCount, double fpr, size_t stringLen = 200){
    Result res;
    Evaluator eval;

    vector<string> real = eval.generateRandomStrings(elemenCount,stringLen);
    vector<string> fake = eval.generateRandomStrings(elemenCount,stringLen);

    auto start = steady_clock().now();
    BloomFilter bf = BloomFilter(elemenCount, fpr);

    for(auto i:real){
        bf.AddToFilter(i);
    }

    size_t missCount = 0;
    for(auto i:real){
        if(!bf.IsInFilter(i)) missCount++;
    }

    size_t fakeCount = 0;
    for(auto i:fake){
        if(bf.IsInFilter(i)) fakeCount++;
    }
    
    auto end = steady_clock().now();
    auto duration = duration_cast<milliseconds>(end-start);

    res.truePositive = elemenCount-missCount;
    res.falsePositive = fakeCount;
    res.trueNegative = elemenCount-fakeCount;
    res.falseNegative = missCount;

    res.timeTakenInMs = duration.count();

    return res;
}

size_t Evaluator::computeTheoryBits(size_t elementCount, double fpr){
    const double ln2 = log(2.0);
    double bits = -(elementCount * log(fpr)) / (ln2 * ln2);
    return static_cast<size_t>(ceil(bits));
}

size_t Evaluator::optimalHashes(size_t bits, size_t elements){
    return round((bits / (double)elements) * log(2));
}