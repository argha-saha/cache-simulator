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