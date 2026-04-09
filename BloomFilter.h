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
        int ArraySize;
    public:
        BloomFilter(int ArraySize);
        bool IsInFilter(std::string query);
        void AddToFilter(std::string data);

};

#endif