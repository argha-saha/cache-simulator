#pragma once

#include <memory>
#include <optional>
#include <stdexcept>
#include <vector>

#include "CacheBlock.h"
#include "Policies.h"

/** 
 * @brief Represents a single set within a cache level. 
 */
class CacheSet {
public:
    /** 
     * @brief Constructs a new cache set. 
     */
    CacheSet(uint32_t ways, const EvictionPolicyInterface& policy);

    /** 
     * @brief Default destructor. 
     */
    ~CacheSet() = default;

    /** 
     * @brief Searches for a block that matches a given tag in this set. 
     */
    std::optional<uint32_t> find_block(uint64_t tag) const;

    /** 
     * @brief Determines the index of the block that will be evicted. 
     */
    uint32_t get_victim_index() const;

    /** 
     * @brief Gets reference to a block within the set. 
     */
    CacheBlock& get_block(uint32_t index);

    /** 
     * @brief Notifies the eviction policy of the block that has been accessed. 
     */
    void access_block(uint32_t index);

    /**
     * @brief Updates the fields of the block.
     */
    void update_block(uint32_t index, uint64_t new_tag, bool set_dirty = false);

    /**
     * @brief Provides a read-only reference to a block within the set.
     */
    const CacheBlock& get_block(uint32_t index) const;

    /**
     * @brief Returns the associativity of the block.
     */
    uint32_t get_associativity() const;

    /**
     * @brief Finds the first invalid block in the set.
     */
    std::optional<uint32_t> find_invalid_block() const;

    /** 
     * @brief Returns a list of the blocks in the set.
     */
    const std::vector<CacheBlock>& get_blocks() const;

    /** 
     * @brief Replaces the contents of a block within this set. 
     */
    void replace_block(uint32_t index, uint64_t new_tag, bool dirty);

    /** 
     * @brief Checks if any block within the set is invalid. 
     */
    bool has_invalid_block() const;

    /**
     * @brief Get the index of the first invalid block.
     */
    std::optional<uint32_t> get_invalid_block_index() const;

private:
    uint32_t associativity;
    std::vector<CacheBlock> blocks;
    std::unique_ptr<EvictionPolicyInterface> eviction_policy;
};