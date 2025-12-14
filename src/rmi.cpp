#include <vector>
#include <iostream>
#include <cmath>
#include "linear.h"
#include "rmi.h"

void TwoStageRMI::train(const std::vector<uint64_t>& data) {
    if (data.empty()) return;

    // map the key range [min_key, max_key] to [0, num_models)
    double min_key = (double)data.front();
    double max_key = (double)data.back();
    
    // Protect against division by zero if all keys are the same
    if (std::abs(max_key - min_key) < 1e-9) {
        stage1_model.slope = 0;
        stage1_model.intercept = 0;
    } else {
        stage1_model.slope = (double)num_stage2_models / (max_key - min_key);
        stage1_model.intercept = -stage1_model.slope * min_key;
    }

    // bucket[i] contains the INDICES of keys assigned to model i
    std::vector<std::vector<size_t>> buckets(num_stage2_models);

    for (size_t i = 0; i < data.size(); ++i) {
        size_t model_idx = stage1_model.predict(data[i]);
        
        // Clamp to valid model range (in case prediction overshoots)
        if (model_idx >= num_stage2_models) model_idx = num_stage2_models - 1;
        
        buckets[model_idx].push_back(i);
    }

    stage2_models.resize(num_stage2_models);
    for (size_t i = 0; i < num_stage2_models; ++i) {
        stage2_models[i] = train_linear_model(data, buckets[i]);
    }

    // Calculate bucket errors and global max error
    bucket_errors.assign(num_stage2_models, 0);

    for (size_t i = 0; i < data.size(); ++i) {
        size_t model_idx = stage1_model.predict(data[i]);
        if (model_idx >= num_stage2_models) model_idx = num_stage2_models - 1;
        
        size_t predicted_pos = stage2_models[model_idx].predict(data[i]);
        
        // Calculate Error
        size_t actual_pos = i;
        size_t error = (predicted_pos > actual_pos) ? (predicted_pos - actual_pos) : (actual_pos - predicted_pos);
        
        // Update specific bucket's error
        if (error > bucket_errors[model_idx]) {
            bucket_errors[model_idx] = error;
        }
        if (error > max_error) { // Update global max error
            max_error = error;
        }
    }
    
};

size_t TwoStageRMI::lookup(uint64_t key, const std::vector<uint64_t>& data) const {
    if (data.empty() || stage2_models.empty()) return static_cast<size_t>(-1);

    // 1. Predict Stage 2 Model
    size_t model_idx = stage1_model.predict(key);
    if (model_idx >= num_stage2_models) model_idx = num_stage2_models - 1;

    // 2. Predict Position
    size_t pred_pos = stage2_models[model_idx].predict(key);

    // 3. Last Mile Search (Binary Search)
    // We search within [pred_pos - max_error, pred_pos + max_error]
    size_t error = bucket_errors[model_idx];
    size_t search_min = (pred_pos > error) ? pred_pos - error : 0;
    size_t search_max = pred_pos + error + 1;
    if (search_max > data.size()) search_max = data.size();
    if (search_min >= data.size()) search_min = data.size() - 1;

    auto it = std::lower_bound(data.begin() + search_min, data.begin() + search_max, key);
    if (it != data.end() && *it == key) return std::distance(data.begin(), it);
    return -1; // Not found
};