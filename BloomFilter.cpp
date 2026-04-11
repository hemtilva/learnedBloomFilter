#include "BloomFilter.h"
using namespace std;

BloomFilter::BloomFilter(size_t bits, size_t hashes){    
    bitSize = bits;
    hashCount = hashes;

    size_t words = (bits+63)/64;
    filterArray.resize(words,0);
}

uint64_t BloomFilter::rotl(uint64_t x, int r){
    return (x << r) | (x >> (64 - r));
}

uint64_t BloomFilter::xxhash64(const std::string& s, uint64_t seed = 0){
    const uint64_t PRIME1 = 11400714785074694791ULL;
    const uint64_t PRIME2 = 14029467366897019727ULL;
    const uint64_t PRIME3 =  1609587929392839161ULL;
    const uint64_t PRIME4 =  9650029242287828579ULL;
    const uint64_t PRIME5 =  2870177450012600261ULL;

    const uint8_t* p = (const uint8_t*)s.data();
    const uint8_t* end = p + s.size();

    uint64_t h;

    if (s.size() >= 32) {

        uint64_t v1 = seed + PRIME1 + PRIME2;
        uint64_t v2 = seed + PRIME2;
        uint64_t v3 = seed + 0;
        uint64_t v4 = seed - PRIME1;

        const uint8_t* limit = end - 32;

        while (p <= limit) {

            uint64_t k1 = *(uint64_t*)p; p += 8;
            v1 += k1 * PRIME2;
            v1 = rotl(v1,31);
            v1 *= PRIME1;

            uint64_t k2 = *(uint64_t*)p; p += 8;
            v2 += k2 * PRIME2;
            v2 = rotl(v2,31);
            v2 *= PRIME1;

            uint64_t k3 = *(uint64_t*)p; p += 8;
            v3 += k3 * PRIME2;
            v3 = rotl(v3,31);
            v3 *= PRIME1;

            uint64_t k4 = *(uint64_t*)p; p += 8;
            v4 += k4 * PRIME2;
            v4 = rotl(v4,31);
            v4 *= PRIME1;
        }

        h = rotl(v1,1) + rotl(v2,7) + rotl(v3,12) + rotl(v4,18);

    } else {
        h = seed + PRIME5;
    }

    h += s.size();

    while (p + 8 <= end) {
        uint64_t k1 = *(uint64_t*)p;
        k1 *= PRIME2;
        k1 = rotl(k1,31);
        k1 *= PRIME1;
        h ^= k1;

        h = rotl(h,27) * PRIME1 + PRIME4;
        p += 8;
    }

    if (p + 4 <= end) {
        h ^= (*(uint32_t*)p) * PRIME1;
        h = rotl(h,23) * PRIME2 + PRIME3;
        p += 4;
    }

    while (p < end) {
        h ^= (*p) * PRIME5;
        h = rotl(h,11) * PRIME1;
        p++;
    }

    h ^= h >> 33;
    h *= PRIME2;
    h ^= h >> 29;
    h *= PRIME3;
    h ^= h >> 32;

    return h;
}

void BloomFilter::setBit(size_t index){
    size_t indInArray = index >> 6;
    size_t offset  = index & 63;

    filterArray[indInArray] |= (1ULL << offset);
}

bool BloomFilter::getBit(size_t index) const{
    size_t indInArray = index >> 6;
    size_t offset  = index & 63;

    return (filterArray[indInArray] >> offset) & 1;
}

void BloomFilter::AddToFilter(const string& data){
    uint64_t h1 = xxhash64(data, 0);
    uint64_t h2 = xxhash64(data, 1);

    for(size_t i=0;i<hashCount;i++){
        size_t idx = (h1 + i*h2) % bitSize;
        setBit(idx);
    }
}

bool BloomFilter::IsInFilter(const string& data){
    uint64_t h1 = xxhash64(data, 0);
    uint64_t h2 = xxhash64(data, 1);

    for(size_t i=0;i<hashCount;i++){
        size_t idx = (h1 + i*h2) % bitSize;
        if(!getBit(idx)) return false;
    }
    return true;
}

size_t BloomFilter::getMemoryCost(){
    return filterArray.capacity()*sizeof(uint64_t);
}