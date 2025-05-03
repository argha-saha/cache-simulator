#include "FIFOPolicy.h"

void FIFOPolicy::initialize(uint32_t ways) {
    associativity = ways;
    fifo.clear();
}

void FIFOPolicy::on_access(const CacheSet& set, uint32_t index) {}

void FIFOPolicy::on_fill(const CacheSet& set, uint32_t index) {
    bool found = false;
    
    for (const auto& i : fifo) {
        if (i == index) {
            found = true;
            break;
        }
    }

    if (!found) {
        fifo.push_back(index);
    }
}

uint32_t FIFOPolicy::get_victim_index(const CacheSet& set) {}

std::unique_ptr<EvictionPolicyInterface> FIFOPolicy::clone() const {}