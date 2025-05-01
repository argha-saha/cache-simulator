#include "Utils.h"

bool is_power_of_two(uint64_t n) {
    return (n > 0) && ((n & (n - 1)) == 0);
}

uint32_t log2(uint64_t n) {
    if (n == 0) {
        throw std::invalid_argument("log2: n must be greater than 0");
    }

    if (!is_power_of_two(n)) {
        throw std::invalid_argument("log2: n must be a power of 2");
    }

    return static_cast<uint32_t>(63 - __builtin_clzll(n));
}