#ifndef LINEAR_H
#define LINEAR_H

#include <numeric>
#include <cstdint>
#include <cstddef>
#include <vector>

class LinearModel {
    public:
    double slope = 0.0;
    double intercept = 0.0;
    size_t predict(uint64_t key) const {
        double pos = slope * key + intercept;
        return pos < 0 ? 0 : static_cast<size_t>(pos);
    }
};

LinearModel train_linear_model(const std::vector<uint64_t>& keys, const std::vector<size_t>& positions);
#endif // LINEAR_H