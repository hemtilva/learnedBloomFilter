#include "BloomFilter.h"
#include <bits/stdc++.h>
using namespace std;

int main(){
    BloomFilter myFilter(5);
    cout << myFilter.IsInFilter("haha") << endl;
    myFilter.AddToFilter("haha");
    cout << myFilter.IsInFilter("haha") << endl;
}