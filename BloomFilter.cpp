#include "BloomFilter.h"
using namespace std;

BloomFilter::BloomFilter(int ArraySize){
    filterArray.resize(ArraySize);
}

void BloomFilter::AddToFilter(string data){
    int bitToChange = 0; //hash the data get bit
    int posInArray = bitToChange >> 6;
    int offset = bitToChange & 63;

    filterArray[posInArray] |= (1<<offset); 
}

bool BloomFilter::IsInFilter(string query){
    int bitToCheck = 0; //hash the query to know
    int posInArray = bitToCheck >> 6;
    int offset = bitToCheck & 63;

    if(filterArray[posInArray] & (1<<offset)){
        return true;
    }else{
        return false;
    }
}