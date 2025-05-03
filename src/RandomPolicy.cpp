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