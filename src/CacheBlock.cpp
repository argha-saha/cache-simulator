#include "CacheBlock.h"

uint64_t CacheBlock::get_tag() const {
    return tag;
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