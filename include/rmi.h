#include <vector>
#include <cstdint>
#include <iostream>
#include "linear.h"

class TwoStageRMI {
public:
    LinearModel stage1_model;
    std::vector<LinearModel> stage2_models;
    size_t num_stage2_models;
    std::vector<size_t> bucket_errors; // Stored for the search phase
    size_t max_error = 0;              // Reporting: Global max error

    TwoStageRMI(size_t stage2_count) : num_stage2_models(stage2_count) {}

    void train(const std::vector<uint64_t>& data);
    
    size_t lookup(uint64_t key, const std::vector<uint64_t>& data) const;
    size_t get_max_error() const {  return max_error; }
    size_t get_model_size() const { 
        return sizeof(stage1_model) + 
               (stage2_models.size() * sizeof(LinearModel)) + 
               (bucket_errors.size() * sizeof(size_t)); 
    }
};