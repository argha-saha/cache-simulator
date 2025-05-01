#include "MemoryAddress.h"

MemoryAddress::MemoryAddress(uint64_t addr, uint32_t block_size, uint32_t num_sets)
    : address(addr) {
    // TODO: Check num_sets and block_size are powers of 2
}