#include "LearnedBloomFilter.h"
#include "FeatureExtractor.h"
#include <cmath>

LearnedBloomFilter::LearnedBloomFilter(std::vector<double> w, double b, double t, int bins, size_t backup_size, double backup_fpr) 
    : weights(w), bias(b), threshold(t), num_bins(bins), backup_bf(backup_size, backup_fpr) {
}

void LearnedBloomFilter::insert(const std::string& key) {
    std::vector<int> active_indices = extract_single_sparse_ngrams(key, num_bins);
    double score = bias;
    for (int index : active_indices) {
        score += weights[index];
    }
    
    double prob = 1.0 / (1.0 + std::exp(-score));

    if (prob < threshold) {
        backup_bf.AddToFilter(key);
    }
}

void LearnedBloomFilter::addToBackup(const std::string& key) {
    backup_bf.AddToFilter(key);
}

bool LearnedBloomFilter::contains(const std::string& key) {
    std::vector<int> active_indices = extract_single_sparse_ngrams(key, num_bins);

    double score = bias;
    for (int index : active_indices) {
        score += weights[index];
    }

    double prob = 1.0 / (1.0 + std::exp(-score));

    if (prob >= threshold) {
        return true; 
    }

    return backup_bf.IsInFilter(key);
}

double LearnedBloomFilter::getMemoryCost() const {
    double model_cost = (weights.size() * sizeof(double)) + (sizeof(double) * 2);
    return model_cost + backup_bf.getMemoryCost();
}