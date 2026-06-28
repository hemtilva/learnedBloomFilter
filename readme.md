# High-Performance Learned Bloom Filter (C++ / Pybind11)

[cite_start]Standard Bloom filters are a foundational probabilistic data structure, guaranteeing zero false negatives while testing set membership. [cite: 310] [cite_start]However, as datasets grow to millions of records, their memory footprint scales linearly and aggressively. [cite: 311] [cite_start]Learned Bloom Filters (LBF) solve this by using machine learning models to "memorize" the dataset, drastically shrinking the required memory footprint. [cite: 312] [cite_start]Unfortunately, LBFs often fail in real-world systems because Python-based ML inference introduces massive latency overhead, negating the speed advantage of a standard filter. [cite: 313] 

This project bridges that gap. [cite_start]By training a sparse logistic regression model in Python and porting the entire inference, feature extraction, and fallback routing pipeline directly into native C++, this architecture achieves heavy space compression while maintaining real-time hardware-level throughput. [cite: 314]

---

### **System Architecture & Technical Innovations**

* [cite_start]**Zero-Copy Native C++ Inference:** Eliminated massive Python-to-C++ context-switching overhead by porting the inference engine directly into C++. [cite: 315] [cite_start]The system executes zero-copy dot-product evaluations natively, maximizing CPU cache efficiency. [cite: 316]
* [cite_start]**Lean Sparse Feature Representation:** Stripped out Scikit-Learn’s heavy Compressed Sparse Row (CSR) matrix allocations (which require data, indices, and indptr arrays). [cite: 317] [cite_start]The engine utilizes a lightweight `std::vector` to track only active hash bins, drastically reducing memory overhead during real-time evaluations. [cite: 318]
* [cite_start]**Native Rolling N-Gram Extraction:** Implemented a custom C++ feature extractor using polynomial hashing (bitwise XOR and modulo operations) to evaluate rolling 3-gram and 4-gram URL sequences in real-time, completely bypassing high-level Python tokenizers. [cite: 319]
* [cite_start]**Dynamic Fallback Routing:** Achieved guaranteed system accuracy (zero false negatives) by dynamically routing predictive model misclassifications to a secondary, fallback C++ standard Bloom Filter. [cite: 320]
* [cite_start]**Breaking the Hash Collision Ceiling:** Diagnosed a strict 1% False Positive Rate (FPR) performance floor caused by physical hash collisions between malicious and benign URLs. [cite: 321] [cite_start]Resolved this by expanding the sparse double-hashing space from 16k to >262k bins and injecting balanced class weighting to counter a severe 23:1 dataset imbalance. [cite: 322]
* [cite_start]**Hardware-Isolated Benchmarking:** Designed an end-to-end benchmark that evaluates the exact same lifecycle—Read, Insert All, Query All—entirely in C++, completely isolating true CPU nanosecond-level execution latency. [cite: 323]

---

### **Performance Metrics**

[cite_start]On a highly imbalanced dataset of 450,000 URLs, the total memory footprint was successfully compressed by **71%** (shrinking from 0.34 MB down to 0.098 MB). [cite: 324] 

[cite_start]Despite the mathematical overhead of extracting rolling n-grams and computing dot-products, the C++ inference engine sustains a blistering throughput of roughly **3.2 to 3.4 Million Queries Per Second (MPS)**. [cite: 325]

[cite_start]**Amortized Execution Time:** ~293 to 308 ns per query. [cite: 326]

| Target FPR | Space Compression | Latency Overhead (vs Baseline) |
| :--- | :--- | :--- |
| **0.10** | 4.85x | [cite_start]3.68x | [cite: 327]
| **0.05** | 3.54x | [cite_start]3.47x | [cite: 328]
| **0.01** | 2.83x | [cite_start]3.39x | [cite: 328]

> [cite_start]**Note on Latency:** A standard Bloom Filter evaluates in roughly ~80-100 ns. [cite: 329] [cite_start]By migrating inference completely out of Python, this LBF restricts its latency overhead penalty to a strict ~3.5x multiplier while delivering up to nearly 5x memory compression. [cite: 330]

---

### **Visualizing the Trade-offs**

[cite_start]The following charts demonstrate the relationship between threshold tuning, memory consumption, and execution latency across different False Positive Rate targets. [cite: 331]

![0.10 FPR Performance](visuals/memory_latency_FPR_Threshold.png_0.1_4096.png)

![0.05 FPR Performance](visuals/memory_latency_FPR_Threshold.png_0.05_8192.png)

![0.01 FPR Performance](visuals/memory_latency_FPR_Threshold.png_0.01_16384.png)
