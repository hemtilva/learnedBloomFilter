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
    os <<"\nTime taken: "<<res.timeTakenInNs <<" ns"<< endl;
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
    
    BloomFilter bf = BloomFilter(elemenCount,fpr);

    for(auto i:real){
        bf.AddToFilter(i);
        res.insertCount++;
    }

    auto start = steady_clock().now();
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
    auto duration = duration_cast<nanoseconds>(end-start);

    res.truePositive = elemenCount-missCount;
    res.falsePositive = fakeCount;
    res.trueNegative = elemenCount-fakeCount;
    res.falseNegative = missCount;

    res.timeTakenInNs = duration.count();
    res.memoryNeeded = bf.getMemoryCost();

    return res;
}

Result Evaluator::runLearnedExperiment(
        const std::vector<double>& weights, 
        double bias, 
        double threshold, 
        int num_bins,
        size_t backup_size,
        double backup_fpr,
        const std::string& pos_file,
        const std::string& neg_file) {
    
    Result res;
    res.insertCount = 0;
    res.falsePositive = 0;
    res.trueNegative = 0;

    std::vector<std::string> positive_urls;
    std::ifstream p_file(pos_file);
    std::string line;
    while (std::getline(p_file, line)) {
        if (!line.empty()) positive_urls.push_back(line);
    }

    std::vector<std::string> negative_urls;
    std::ifstream n_file(neg_file);
    while (std::getline(n_file, line)) {
        if (!line.empty()) negative_urls.push_back(line);
    }

    res.negativeQueryCount = negative_urls.size();

    LearnedBloomFilter lbf(weights, bias, threshold, num_bins, backup_size, backup_fpr);

    auto start_time = std::chrono::high_resolution_clock::now();

    for (const auto& url : positive_urls) {
        lbf.insert(url);
        res.insertCount++;
    }

    for (const auto& url : negative_urls) {
        if (lbf.contains(url)) {
            res.falsePositive++;
        } else {
            res.trueNegative++;
        }
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    
    res.timeTakenInNs = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
    res.memoryNeeded = lbf.getMemoryCost(); 

    return res;
}