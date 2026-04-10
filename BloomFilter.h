#include <cstdlib>
#include <cstdint>
#include <vector>
#include <bitset>
#include <string>

#ifndef BLOOM_H
#define BLOOM_H

class BloomFilter{
    private:
        std::vector<uint64_t> filterArray;
        size_t bitSize;
        size_t hashCount;

        static inline uint64_t rotl(uint64_t x, int r);
        static uint64_t xxhash64(const std::string& s, uint64_t seed);

        void setBit(size_t index);
        bool getBit(size_t index) const;
    public:
        BloomFilter(size_t bits, size_t hashes);
        BloomFilter(size_t approxElementCount, double fpr);
        bool IsInFilter(const std::string& data);
        void AddToFilter(const std::string& data);

};

#endif