#include "FIFOPolicy.h"

void FIFOPolicy::initialize(uint32_t ways) {
    associativity = ways;
    fifo.clear();
}

void FIFOPolicy::on_access(const CacheSet& set, uint32_t index) {}

void FIFOPolicy::on_fill(const CacheSet& set, uint32_t index) {}

uint32_t FIFOPolicy::get_victim_index(const CacheSet& set) {}

std::unique_ptr<EvictionPolicyInterface> FIFOPolicy::clone() const {}