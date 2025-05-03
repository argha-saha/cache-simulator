#include "LRUPolicy.h"

void LRUPolicy::initialize(uint32_t ways) {
    associativity = ways;
    lru_stack.resize(associativity, 0);
    counter = 0;
}

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