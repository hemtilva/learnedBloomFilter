#include "BloomFilter.h"
#include "Evaluator.h"
#include <iostream>
#include <chrono>
using namespace std;
using namespace std::chrono;

int main(){
    Evaluator eval;

    cout << "elements, fpr: ";
    size_t n;cin >> n;
    double fpr;cin >> fpr;

    Result res = eval.runSingleExperiment(n,fpr,500);

    cout << "\ntime taken: "<< res.timeTakenInMs <<"ms"<< endl;
    cout << "misses: " << res.falseNegative << "\nfakes: " << res.falsePositive << endl;
}