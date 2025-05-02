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

uint32_t CacheSet::get_victim() const {
    if (!eviction_policy) {
        throw std::runtime_error("CacheSet: Eviction policy was not initialized.");
    }

    return eviction_policy->get_victim(*this);
}