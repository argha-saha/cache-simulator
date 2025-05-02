#pragma once

#include <cstdint>

class CacheBlock {
public:
    CacheBlock() = default;

    uint64_t get_tag() const;
    bool is_valid() const;
    bool is_dirty() const;
    void set_tag(uint64_t new_tag);
    void set_valid(bool new_valid);
    void set_dirty(bool new_dirty);

private:
    uint64_t tag = 0;
    bool valid = false;
    bool dirty = false;
};