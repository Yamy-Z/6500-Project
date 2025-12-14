#include <iostream>
#include <chrono>
#include <algorithm>
#include <random>
#include <string>
#include <vector>
#include <exception>
#include "dataloader.h"
#include "rmi.h"
#include "binary_search.h"

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <data_file> <num_models>" << std::endl;
        return 1;
    }
    
    std::string filename = argv[1];
    size_t num_models = std::stoull(argv[2]);

    std::vector<uint64_t> data;
    try {
        data = load_data(filename);
    } catch (const std::exception& e) {
        std::cerr << "Load failed: " << e.what() << std::endl;
        return 1;
    }
    if (data.empty()) {
        std::cerr << "Error: dataset " << filename << " is empty\n";
        return 1;
    }

    // RMI 
    TwoStageRMI rmi(num_models);
    auto t1 = std::chrono::high_resolution_clock::now();
    rmi.train(data);
    auto t2 = std::chrono::high_resolution_clock::now();
    double train_time = std::chrono::duration<double>(t2 - t1).count();

    //Baseline
    BinarySearchIndex bsi;
    bsi.train(data); // No-op

    // Sample keys for lookup 
    std::mt19937 rng(42);
    std::vector<uint64_t> keys;
    size_t num_lookups = 100000;
    std::uniform_int_distribution<size_t> dist(0, data.size() - 1);
    for(size_t i=0; i<num_lookups; ++i) keys.push_back(data[dist(rng)]);

    // RMI Lookup
    auto start_rmi = std::chrono::high_resolution_clock::now();
    size_t checksum_rmi = 0;
    for (const uint64_t &key : keys) {
        if (rmi.lookup(key, data) != -1) checksum_rmi++;
    }
    auto end_rmi = std::chrono::high_resolution_clock::now();
    double rmi_time = std::chrono::duration<double, std::nano>(end_rmi - start_rmi).count() / keys.size();

    // Binary Search Baseline
    auto start_bs = std::chrono::high_resolution_clock::now();
    size_t checksum_bs = 0;
    for (const uint64_t &key : keys) {
        if (bsi.lookup(key, data) != -1) checksum_bs++;
    }
    auto end_bs = std::chrono::high_resolution_clock::now();
    double bs_time = std::chrono::duration<double, std::nano>(end_bs - start_bs).count() / keys.size();

    if (checksum_rmi != checksum_bs) {
        std::cerr << "WARNING: mismatch between RMI and BS results: "
                << checksum_rmi << " vs " << checksum_bs << std::endl;
    }
    // Output CSV-style for the Python script to parse
    std::cout << "RESULT," << filename << "," << num_models << "," 
              << rmi.get_max_error() << "," << rmi_time << "," << bs_time << std::endl;
    
    return 0;
}