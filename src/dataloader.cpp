#include <iostream>
#include <vector>
#include <fstream>
#include <cstdint>
#include <string>
#include <stdexcept>

std::vector<uint64_t> load_data(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Error: Could not open file " + filename);
    }

    // Read size header
    uint64_t size64 = 0;
    file.read(reinterpret_cast<char*>(&size64), sizeof(uint64_t));
    if (!file) {
        throw std::runtime_error("Error: Failed to read header from " + filename);
    }
    std::size_t size = static_cast<std::size_t>(size64);

    // Read data
    std::vector<uint64_t> data(size);
    file.read(reinterpret_cast<char*>(data.data()), size * sizeof(uint64_t));

    if (!file) {
        throw std::runtime_error("Error: Failed to read " + std::to_string(size) +
                                " elements from " + filename);
    }

    file.close();
    
    return data;
}