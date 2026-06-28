#pragma once

#include <vector>
#include <string>
#include "BloomFilter.h"

class LearnedBloomFilter {
private:
    std::vector<double> weights;
    double bias;
    double threshold;
    int num_bins;
    BloomFilter backup_bf;

public:
    LearnedBloomFilter(std::vector<double> w, double b, double t, int bins, size_t backup_size, double backup_fpr);

    void insert(const std::string& key);
    void addToBackup(const std::string& key);
    bool contains(const std::string& key);
    
    double getMemoryCost() const; 
};