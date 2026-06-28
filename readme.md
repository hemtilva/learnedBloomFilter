# High-Performance Learned Bloom Filter (C++ / Pybind11)

[cite_start]Standard Bloom filters guarantee zero false negatives but their memory footprint scales linearly with large datasets[cite: 56]. [cite_start]Learned Bloom Filters (LBF) shrink this footprint using machine learning models[cite: 57]. [cite_start]However, Python-based ML inference introduces severe latency overhead, often negating the speed advantages[cite: 58]. [cite_start]This project bridges that gap by training a logistic regression model in Python and porting the entire inference pipeline directly into native C++ to achieve heavy space compression with hardware-level throughput[cite: 59].

---

### **System Architecture**

* [cite_start]**Zero-Copy Native C++ Inference:** Eliminated massive Python-to-C++ context-switching overhead by executing zero-copy dot-product evaluations natively in C++[cite: 60, 61].
* [cite_start]**Lean Sparse Representation:** Replaced heavy Compressed Sparse Row (CSR) matrices with a lightweight `std::vector` to track active hash bins during real-time evaluations[cite: 62, 63].
* [cite_start]**Native Rolling N-Grams:** Implemented a custom C++ extractor using polynomial hashing to evaluate 3-gram and 4-gram sequences in real-time[cite: 64].
* [cite_start]**Dynamic Fallback Routing:** Routed predictive model misclassifications to a fallback C++ standard Bloom Filter to guarantee zero false negatives[cite: 65].
* [cite_start]**Optimized Accuracy:** Overcame a 1% False Positive Rate (FPR) performance floor by expanding the hashing space to >262k bins and injecting balanced class weighting to counter a 23:1 dataset imbalance[cite: 66, 67].
* [cite_start]**Isolated Benchmarking:** Designed an end-to-end benchmark entirely in C++ to isolate true nanosecond-level CPU latency[cite: 68].

---

### **Performance Metrics**

[cite_start]On a highly imbalanced dataset of 450,000 URLs, overall memory footprint was compressed by 71% (from 0.34 MB to 0.098 MB)[cite: 69]. [cite_start]The C++ inference engine sustains a throughput of roughly 3.2 to 3.4 Million Queries Per Second (MPS)[cite: 70]. [cite_start]Amortized execution time is roughly 293 to 308 ns per query[cite: 71].

| Target FPR | Space Compression | Latency Overhead (vs Baseline) |
| :--- | :--- | :--- |
| **0.10** | 4.85x | [cite_start]3.68x [cite: 72] |
| **0.05** | 3.54x | [cite_start]3.47x [cite: 73] |
| **0.01** | 2.83x | [cite_start]3.39x [cite: 73] |

> [cite_start]**Note on Latency:** By migrating inference out of Python, this LBF restricts its latency overhead penalty to a strict ~3.5x multiplier while delivering up to nearly 5x memory compression[cite: 75].

---

### **Visualizing the Trade-offs**

![Description of image](visuals/memory_latency_FPR_Threshold.png_0.1_4096.png)

![Description of image](visuals/memory_latency_FPR_Threshold.png_0.05_8192.png)

![Description of image](visuals/memory_latency_FPR_Threshold.png_0.01_16384.png)
