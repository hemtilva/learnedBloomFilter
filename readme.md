# High-Performance Learned Bloom Filter (C++ / Pybind11)

Standard Bloom filters guarantee zero false negatives but their memory footprint scales linearly with large datasets. Learned Bloom Filters (LBF) shrink this footprint using machine learning models. However, Python-based ML inference introduces severe latency overhead, often negating the speed advantages. This project bridges that gap by training a logistic regression model in Python and porting the entire inference pipeline directly into native C++ to achieve heavy space compression with hardware-level throughput.

---

### **System Architecture**

* **Zero-Copy Native C++ Inference:** Eliminated massive Python-to-C++ context-switching overhead by executing zero-copy dot-product evaluations natively in C++.
* **Lean Sparse Representation:** Replaced heavy Compressed Sparse Row (CSR) matrices with a lightweight `std::vector` to track active hash bins during real-time evaluations.
* **Native Rolling N-Grams:** Implemented a custom C++ extractor using polynomial hashing to evaluate 3-gram and 4-gram sequences in real-time.
* **Dynamic Fallback Routing:** Routed predictive model misclassifications to a fallback C++ standard Bloom Filter to guarantee zero false negatives.
* **Optimized Accuracy:** Overcame a 1% False Positive Rate (FPR) performance floor by expanding the hashing space to >262k bins and injecting balanced class weighting to counter a 23:1 dataset imbalance.
* **Isolated Benchmarking:** Designed an end-to-end benchmark entirely in C++ to isolate true nanosecond-level CPU latency.

---

### **Performance Metrics**

On a highly imbalanced dataset of 450,000 URLs, overall memory footprint was compressed by 71% (from 0.34 MB to 0.098 MB). The C++ inference engine sustains a throughput of roughly 3.2 to 3.4 Million Queries Per Second (MPS). Amortized execution time is roughly 293 to 308 ns per query.

| Target FPR | Space Compression | Latency Overhead (vs Baseline) |
| :--- | :--- | :--- |
| **0.10** | 4.85x | 3.68x |
| **0.05** | 3.54x | 3.47x |
| **0.01** | 2.83x | 3.39x |

> **Note on Latency:** By migrating inference out of Python, this LBF restricts its latency overhead penalty to a strict ~3.5x multiplier while delivering up to nearly 5x memory compression.

---

### **Visualizing the Trade-offs**

![0.10 FPR Performance](visuals/memory_latency_FPR_Threshold.png_0.1_4096.png)

![0.05 FPR Performance](visuals/memory_latency_FPR_Threshold.png_0.05_8192.png)

![0.01 FPR Performance](visuals/memory_latency_FPR_Threshold.png_0.01_16384.png)
