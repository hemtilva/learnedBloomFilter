#include "BloomFilter.h"
#include "Evaluator.h"
#include <iostream>
#include <chrono>
#include <fstream>
#include <set>
using namespace std;
using namespace std::chrono;

int main(){
    Evaluator eval;
    Result res = eval.runSingleExperiment(1000000,0.01, "data/real.txt", "data/fake.txt");
    cout << res << endl;
}