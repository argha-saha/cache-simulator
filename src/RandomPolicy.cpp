#include "RandomPolicy.h"

RandomPolicy::RandomPolicy() : rng(std::random_device{}()) {}

void RandomPolicy::initialize(uint32_t ways) {
    associativity = ways;
    
    if (associativity > 0) {
        uni_dist = std::uniform_int_distribution<uint32_t>(0, associativity - 1);
    } else {
        throw std::invalid_argument("RandomPolicy: Associativity must be greater than zero.");
    }
}

void RandomPolicy::on_access(const CacheSet& set, uint32_t index) {}

void RandomPolicy::on_fill(const CacheSet& set, uint32_t index) {}

uint32_t RandomPolicy::get_victim_index(const CacheSet& set) {
    // Return index of the first invalid block if found
    auto invalid_index = set.get_invalid_block_index();
    if (invalid_index.has_value()) {
        return invalid_index.value();
    }

    // Random victim selection
    return uni_dist(rng);
}

std::unique_ptr<EvictionPolicyInterface> RandomPolicy::clone() const {
    return std::make_unique<RandomPolicy>(*this);
}