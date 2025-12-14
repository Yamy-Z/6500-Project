#include <numeric>
#include <cmath>
#include "linear.h"

LinearModel train_linear_model(const std::vector<uint64_t>& data, const std::vector<size_t>& indices) {
    LinearModel model;

    if (indices.empty()) return model;
    if (indices.size() == 1) {
        // Special case: 1 point. Horizontal line passing through y.
        model.slope = 0.0;
        model.intercept = (double)indices[0];
        return model;
    }

    double n = (double)indices.size();
    double sum_x = 0.0;
    double sum_y = 0.0;
    double sum_xy = 0.0;
    double sum_xx = 0.0;

    for (size_t i : indices) {
        double x = (double)data[i]; // The Key
        double y = (double)i;       // The Position (Index)

        sum_x += x;
        sum_y += y;
        sum_xy += x * y;
        sum_xx += x * x;
    }
    // Formula: m = (N * sum(xy) - sum(x)*sum(y)) / (N * sum(xx) - sum(x)^2)
    double numerator = n * sum_xy - sum_x * sum_y;
    double denominator = n * sum_xx - sum_x * sum_x;

    if (std::abs(denominator) < 1e-9) {
        // vertical line case
        model.slope = 0.0;
        model.intercept = sum_y / n;
    } else {
        model.slope = numerator / denominator;
        model.intercept = (sum_y - model.slope * sum_x) / n;
    }

    return model;
};
