#include <algorithm>
#include <vector>
#include <cstdint>
#include <cstddef>

class BinarySearchIndex {
public:
    void train(const std::vector<uint64_t>& /*data*/) {
        // No-op
    }

    size_t lookup(uint64_t key, const std::vector<uint64_t>& data) const {
        auto it = std::lower_bound(data.begin(), data.end(), key);
        
        if (it != data.end() && *it == key) {
            return std::distance(data.begin(), it);
        }
        return -1; // Not found
    }
    
    size_t size_in_bytes() const {
        return 0; // It uses the raw data only
    }
};