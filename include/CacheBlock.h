#pragma once

#include <cstdint>
#include <limits>

class CacheBlock {
public:
    CacheBlock() = default;

    uint64_t get_tag() const;
    uint64_t get_address(uint64_t set_index, uint32_t index_bits, uint32_t offset_bits) const;
    bool is_valid() const;
    bool is_dirty() const;
    void set_tag(uint64_t new_tag);
    void set_valid(bool new_valid);
    void set_dirty(bool new_dirty);
    void reset();

private:
    uint64_t tag = 0;
    bool valid = false;
    bool dirty = false;
};