#include "Evaluator.h"
#include "BloomFilter.h"
#include <random>
#include <cmath>
#include <iostream>
#include <fstream>
#include <chrono>
using namespace std;
using namespace std::chrono;

mt19937 Evaluator::rng(random_device{}());

ostream& operator<<(ostream& os,const Result& res){
    os <<"\nTime taken: "<<res.timeTakenInMs <<"ms"<< endl;
    os << "FP: " << res.falsePositive << endl;
    os << "Total inserts: " << res.insertCount << endl;
    os << "Negative Queries: " << res.negativeQueryCount << endl;
    os << "Memory needed: " << (1.0*res.memoryNeeded) / (1024*1024) <<" MB"<< endl;
    return os;
}

vector<string> Evaluator::generateRandomStrings(size_t count, size_t maxLenOfStrings, const string& path = ""){
    vector<string> result;
    result.reserve(count);

    const string charset = 
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "0123456789";

    uniform_int_distribution<size_t> lengthDist(50, max((size_t)50,maxLenOfStrings));
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

    if(path.size()){
        ofstream file(path);
        for(auto i: result){
            file << i << endl;
        }
        file.close();
    }

    return result;
}

Result Evaluator::runSingleExperiment(size_t elemenCount, double fpr, const string& pathReal,const string& pathFake){
    Result res;

    vector<string> real;
    vector<string> fake;
    real.reserve(elemenCount);
    fake.reserve(elemenCount);

    ifstream realFile(pathReal);
    string line;
    for (size_t i = 0;getline(realFile,line) && i < elemenCount; i++)
    {
        real.push_back(line);
    }
    realFile.close();

    ifstream fakeFile(pathFake);
    for (size_t i = 0;getline(fakeFile,line); i++)
    {
        fake.push_back(line);
    }
    fakeFile.close();
    

    auto start = steady_clock().now();
    BloomFilter bf = BloomFilter(computeTheoryBits(real.size(),fpr),7);

    for(auto i:real){
        bf.AddToFilter(i);
        res.insertCount++;
    }

    size_t missCount = 0;
    for(auto i:real){
        if(!bf.IsInFilter(i)) missCount++;
    }

    size_t fakeCount = 0;
    for(auto i:fake){
        res.negativeQueryCount++;
        if(bf.IsInFilter(i)) fakeCount++;
    }
    
    auto end = steady_clock().now();
    auto duration = duration_cast<milliseconds>(end-start);

    res.truePositive = elemenCount-missCount;
    res.falsePositive = fakeCount;
    res.trueNegative = elemenCount-fakeCount;
    res.falseNegative = missCount;

    res.timeTakenInMs = duration.count();
    res.memoryNeeded = bf.getMemoryCost();

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