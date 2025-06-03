#include "CacheBlock.h"

uint64_t CacheBlock::get_tag() const {
    return tag;
}

uint64_t CacheBlock::get_address(uint64_t set_index, uint32_t index_bits, uint32_t offset_bits) const {
    if (!valid) {
        return std::numeric_limits<uint64_t>::max();
    }

    // Reconstruct the address: (tag << (index_bits + offset_bits)) | (index << offset_bits)
    uint64_t reconstructed_address = tag;
    uint32_t index_plus_offset_bits = index_bits + offset_bits;

    if (index_plus_offset_bits > 0 && index_plus_offset_bits < 64) {
        reconstructed_address <<= index_plus_offset_bits;
    } else if (reconstructed_address >= 64) {
        reconstructed_address = 0;
    }

    if (index_bits > 0) {
        uint64_t index_part = set_index;

        if (offset_bits > 0 && offset_bits < 64) {
            index_part <<= offset_bits;
        } else if (offset_bits >= 64) {
            index_part = 0;
        }

        reconstructed_address |= index_part;
    }

    return reconstructed_address;
}

bool CacheBlock::is_valid() const {
    return valid;
}

bool CacheBlock::is_dirty() const {
    return dirty;
}

void CacheBlock::set_tag(uint64_t new_tag) {
    tag = new_tag;
}

void CacheBlock::set_valid(bool new_valid) {
    valid = new_valid;
}

void CacheBlock::set_dirty(bool new_dirty) {
    dirty = new_dirty;
}

void CacheBlock::reset() {
    tag = 0;
    valid = false;
    dirty = false;
}