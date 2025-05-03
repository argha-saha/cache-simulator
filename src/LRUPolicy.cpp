#include "LRUPolicy.h"

void LRUPolicy::initialize(uint32_t ways) {
    associativity = ways;
    lru_stack.resize(associativity, 0);
    counter = 0;
}

// Note: Don't need to check for valid block because CacheSet only calls this for valid blocks
void LRUPolicy::on_access(const CacheSet& set, uint32_t index) {
    if (index < associativity) {
        if (counter < std::numeric_limits<uint64_t>::max()) {
            counter++;
        } else {
            // Counter overflow handling
            std::cerr << "Counter overflow in LRU policy." << std::endl;
        }

        lru_stack[index] = counter;
    }
}

void LRUPolicy::on_fill(const CacheSet& set, uint32_t index) {
    on_access(set, index);
}

uint32_t LRUPolicy::get_victim(const CacheSet& set) {
    uint32_t victim_index = 0;
    uint64_t min_counter = std::numeric_limits<uint64_t>::max();
    bool found_valid_block = false;

    return victim_index;
}

std::unique_ptr<EvictionPolicyInterface> LRUPolicy::clone() {
    return std::make_unique<LRUPolicy>(*this);
}