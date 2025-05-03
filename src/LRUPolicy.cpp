#include "LRUPolicy.h"

void LRUPolicy::initialize(uint32_t ways) {
    associativity = ways;
    lru_stack.resize(associativity, 0);
    counter = 0;
}