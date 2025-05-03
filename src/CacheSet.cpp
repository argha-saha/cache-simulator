#include "CacheSet.h"

CacheSet::CacheSet(uint32_t ways, const EvictionPolicyInterface& policy)
    : associativity(ways), blocks(ways) 
{
    if (ways == 0) {
        throw std::invalid_argument("CacheSet: Associativity cannot be zero.");
    }

    eviction_policy = policy.clone();

    if (!eviction_policy) {
        throw std::runtime_error("CacheSet: Failed to clone eviction policy.");
    }

    eviction_policy->initialize(associativity);
}

std::optional<uint32_t> CacheSet::find_block(uint64_t tag) const {
    for (uint32_t i = 0; i < associativity; ++i) {
        if (blocks[i].is_valid() && blocks[i].get_tag() == tag) {
            return i;
        }
    }

    return std::nullopt;
}

uint32_t CacheSet::get_victim_index() const {
    if (!eviction_policy) {
        throw std::runtime_error("CacheSet: Eviction policy was not initialized.");
    }

    return eviction_policy->get_victim(*this);
}

CacheBlock& CacheSet::get_block(uint32_t index) {
    try {
        return blocks.at(index);
    } catch (const std::out_of_range&) {
        throw std::out_of_range("CacheSet: Index out of range.");
    }
}

void CacheSet::access_block(uint32_t index) {
    if (index >= associativity) {
        throw std::out_of_range("CacheSet: Index out of range.");
    }

    if (!eviction_policy) {
        throw std::runtime_error("CacheSet: Eviction policy was not initialized.");
    }

    eviction_policy->on_access(*this, index);
}

void CacheSet::replace_block(uint32_t index, uint64_t new_tag, bool dirty) {
    if (index >= associativity) {
        throw std::out_of_range("CacheSet: Index out of range.");
    }

    if (!eviction_policy) {
        throw std::runtime_error("CacheSet: Eviction policy was not initialized.");
    }

    CacheBlock& block = blocks[index];

    block.set_valid(true);
    block.set_tag(new_tag);
    block.set_dirty(dirty);

    eviction_policy->on_fill(*this, index);
}