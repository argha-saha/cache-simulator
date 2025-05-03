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

uint32_t FIFOPolicy::get_victim_index(const CacheSet& set) {
    // Check if there is an invalid block in the set
    auto invalid_index = set.get_invalid_block_index();
    if (invalid_index.has_value()) {
        uint32_t index = invalid_index.value();
        auto it = std::remove(fifo.begin(), fifo.end(), index);

        if (it != fifo.end()) {
            fifo.erase(it, fifo.end());
        }

        return index;
    }

    // If no invalid block is found, evict the oldest block in FIFO order
    if (!fifo.empty()) {
        // CacheSet logic should pop the front of the queue
        uint32_t victim_index = fifo.front();
        return victim_index;
    } else {
        if (associativity == 0) {
            std::cerr << "FIFOPolicy: Queue is empty but all lines are valid." << std::endl;
            return 0;
        } else {
            throw std::runtime_error("FIFOPolicy: No blocks available for eviction.");
        }
    }
}

std::unique_ptr<EvictionPolicyInterface> FIFOPolicy::clone() const {
    return std::make_unique<FIFOPolicy>(*this);
}