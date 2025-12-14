# Implementing a 2-Stage Recursive Model Index (RMI)

**Author:** Yamin Zhang  
**Course:** CIS 6500  
**Date:** December 2025  

---

## Overview

This project explores **Learned Index Structures**, specifically a **2-Stage Recursive Model Index (RMI)** implemented from scratch in **C++**.  
Rather than treating indexing as a traditional search problem, RMIs treat it as a **prediction problem**, using machine learning models to estimate the position of a key in a sorted array.

The implementation is benchmarked against **binary search (`std::lower_bound`)** on large real-world datasets from the **SOSD benchmark**.

---

## Key Idea

If a model can accurately predict the position of a key in a sorted array, it can significantly reduce search time by skipping repeated comparisons. This approach can outperform traditional data structures on smooth data distributions.

---

## Architecture

This project implements a **2-Stage Recursive Model Index (RMI)** for sorted 64-bit integers.

### Stage 1 – Root Model
- A simple scaling function
- Partitions the key space into **N buckets**
- Predicts which Stage 2 model is responsible for a query

### Stage 2 – Leaf Models
- A vector of **N simple linear regression models**
- Each model is trained on a subset of the data
- Predicts the approximate position of a key in the sorted array

---

## Requirements

- C++17 compatible compiler
- CMake
- Python 3
- SOSD benchmark datasets (uint64 binary files)

---

## Data Acquisition

This project uses datasets from the **SOSD (Search on Sorted Data) benchmark**.

1. Visit: https://github.com/learnedsystems/SOSD  
2. Download the required `uint64` binary datasets  
3. Place the files in the `data/` directory at the project root  

---

## Build Instructions

```bash
mkdir build
cd build
cmake ..
make