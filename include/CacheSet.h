#pragma once

#include <memory>
#include <optional>
#include <stdexcept>
#include <vector>

#include "CacheBlock.h"
#include "Policies.h"

/** @brief Represents a single set within a cache level. */
class CacheSet {
public:
    ~CacheSet() = default;

    CacheSet(uint32_t ways, const EvictionPolicyInterface& policy);
    std::optional<uint32_t> find_block(uint64_t tag) const;
    uint32_t get_victim_index() const;
    CacheBlock& get_block(uint32_t index);
    void access_block(uint32_t index);
    void replace_block(uint32_t index, uint64_t new_tag, bool dirty);

private:
    uint32_t associativity;
    std::vector<CacheBlock> blocks;
    std::unique_ptr<EvictionPolicyInterface> eviction_policy;
};