#include "MemoryAddress.h"
#include "Utils.h"

MemoryAddress::MemoryAddress(uint64_t addr, uint32_t block_size, uint32_t num_sets)
    : address(addr) {
    if (block_size == 0 || !is_power_of_two(block_size)) {
        throw std::invalid_argument("Block size must be a positive power of two.");
    }

    if (num_sets == 0) {
        throw std::invalid_argument("Number of sets must be greater than 0.");
    }

    if (!is_power_of_two(num_sets) && num_sets != 1) {
        throw std::invalid_argument("Number of sets must be a 1 or a power of 2.");
    }

    // Fully associative cache does not have index bits
    uint32_t index_bits = (num_sets == 1) ? 0 : log2(num_sets);
    uint32_t block_offset_bits = log2(block_size);

    uint64_t index_mask = (index_bits == 0) ? 0 : ((1ULL << index_bits) - 1);

    if (index_bits > 0 && index_bits < 64) {
        index = (addr >> block_offset_bits) & index_mask;
    } else if (index_bits == 0) {
        index = 0;
    } else {
        throw std::invalid_argument("Index bits exceed address size.");
    }

    block_offset = static_cast<uint32_t>(addr & ((1ULL << block_offset_bits) - 1));

    uint32_t tag_mask = index_bits + block_offset_bits;
    
    if (tag_mask < 64) {
        tag = addr >> tag_mask;
    } else {
        throw std::invalid_argument("Tag bits exceed address size.");
    }
}