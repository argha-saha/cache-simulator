#pragma once

#include <cstdint>

class CacheBlock final {
public:
    // Default constructor and destructor
    CacheBlock() = default;
    ~CacheBlock() = default;

    /**
     * @brief Reconstructs the address from the block's tag, set index, and offset bits.
     * @param set_index The index of the set in which this block resides.
     * @param index_bits The number of bits used for the index.
     * @param offset_bits The number of bits used for the offset within the block.
     * @return The reconstructed address.
     */
    uint64_t get_address(uint64_t set_index, uint32_t index_bits, uint32_t offset_bits) const;

    // Getters
    uint64_t get_tag() const { return tag; }
    bool is_valid() const { return valid; }
    bool is_dirty() const { return dirty; }

    // Setters
    void set_tag(uint64_t new_tag) { tag = new_tag; }
    void set_valid(bool new_valid) { valid = new_valid; }
    void set_dirty(bool new_dirty) { dirty = new_dirty; }

private:
    uint64_t tag = 0;
    bool valid = false;
    bool dirty = false;
};