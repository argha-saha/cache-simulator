#include "CacheSet.h"

#include <iostream>
#include <stdexcept>

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
    // std::cout << "DEBUG: CacheSet::find_block - Set (ways: " << associativity << "): Searching for Tag=0x"
    //           << std::hex << tag << std::dec << std::endl;

    for (uint32_t i = 0; i < associativity; ++i) {
        if (blocks[i].is_valid() && blocks[i].get_tag() == tag) {
            return i;
        }
    }

    // std::cout << "DEBUG: CacheSet::find_block() No match found." << std::endl;
    return std::nullopt;
}

uint32_t CacheSet::get_victim_index() const {
    if (!eviction_policy) {
        throw std::runtime_error("CacheSet: Eviction policy was not initialized.");
    }

    return eviction_policy->get_victim_index(*this);
}

CacheBlock& CacheSet::get_block(uint32_t index) {
    try {
        return blocks.at(index);
    } catch (const std::out_of_range&) {
        throw std::out_of_range("CacheSet: Index out of range.");
    }
}

uint32_t CacheSet::get_associativity() const {
    return associativity;
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

void CacheSet::replace_block(uint32_t index, uint64_t new_tag, bool new_dirty) {
    if (index >= associativity) {
        throw std::out_of_range("CacheSet: Index out of range.");
    }

    if (!eviction_policy) {
        throw std::runtime_error("CacheSet: Eviction policy was not initialized.");
    }

    CacheBlock& block = blocks[index];

    block.set_valid(true);
    block.set_tag(new_tag);
    block.set_dirty(new_dirty);

    // std::cout << "DEBUG: CacheSet::replace_block - Set (ways " << associativity << "): Replaced block " << index
    //           << " with Tag=0x" << std::hex << new_tag << std::dec << ", Valid=true" << std::endl;

    eviction_policy->on_fill(*this, index);
}

bool CacheSet::has_invalid_block() const {
    for (const auto& block : blocks) {
        if (!block.is_valid()) {
            return true;
        }
    }

    return false;
}

std::optional<uint32_t> CacheSet::get_invalid_block_index() const {
    for (uint32_t i = 0; i < associativity; ++i) {
        if (!blocks[i].is_valid()) {
            return i;
        }
    }

    return std::nullopt;
}