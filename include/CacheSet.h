#pragma once

#include <memory>
#include <optional>
#include <vector>

#include "CacheBlock.h"
#include "Policies.h"

/** 
 * @brief Represents a single set within a cache level. 
 */
class CacheSet final {
public:
    /** 
     * @brief Constructs a new cache set.
     * @param ways The number of ways (associativity) in this set.
     * @param policy The eviction policy to be used for this set.
     */
    CacheSet(uint32_t ways, const EvictionPolicyInterface& policy);

    // Default destructor
    CacheSet() = delete;

    // Prevent copying and allow moving
    // TODO: Look into why deleting the move constructor and assignment operator causes issues
    CacheSet(const CacheSet&) = delete;
    CacheSet& operator=(const CacheSet&) = delete;
    CacheSet(CacheSet&&) = default;
    CacheSet& operator=(CacheSet&&) = default;

    /** 
     * @brief Searches for a block that matches a given tag in this set. 
     * @param tag The tag to search for.
     */
    std::optional<uint32_t> find_block(uint64_t tag) const;

    /** 
     * @brief Determines the index of the block that will be evicted. 
     * @return The index of the victim block.
     */
    uint32_t get_victim_index() const;

    /** 
     * @brief Gets reference to a block within the set. 
     * @param index The index of the block to retrieve.
     * @return Reference to the CacheBlock at the specified index.
     */
    CacheBlock& get_block(uint32_t index);

    /** 
     * @brief Notifies the eviction policy of the block that has been accessed. 
     * @param index The index of the block that was accessed.
     */
    void access_block(uint32_t index);

    /**
     * @brief Updates the fields of the block.
     * @param index The index of the block to update.
     * @param new_tag The new tag to set for the block.
     * @param set_dirty Whether to mark the block as dirty (default is false).
     */
    void update_block(uint32_t index, uint64_t new_tag, bool set_dirty = false);

    /**
     * @brief Provides a read-only reference to a block within the set.
     * @param index The index of the block to retrieve.
     * @return A const reference to the CacheBlock at the specified index.
     */
    const CacheBlock& get_block(uint32_t index) const;

    /**
     * @return The number of ways (associativity) of this cache set.
     */
    uint32_t get_associativity() const;

    /**
     * @brief Finds the first invalid block in the set.
     * @return An optional containing the index of the first invalid block, or std::nullopt if all blocks are valid.
     */
    std::optional<uint32_t> find_invalid_block() const;

    /** 
     * @return A read-only reference to the vector of blocks in this set.
     */
    const std::vector<CacheBlock>& get_blocks() const;

    /** 
     * @brief Replaces the contents of a block within this set. 
     * @param index The index of the block to replace.
     * @param new_tag The new tag to set for the block.
     * @param new_dirty Whether to mark the block as dirty (default is false).
     */
    void replace_block(uint32_t index, uint64_t new_tag, bool new_dirty);

    /** 
     * @brief Checks if any block within the set is invalid. 
     * @return True if there is at least one invalid block, false otherwise.
     */
    bool has_invalid_block() const;

    /**
     * @brief Get the index of the first invalid block.
     * @return An optional containing the index of the first invalid block, or std::nullopt if all blocks are valid.
     */
    std::optional<uint32_t> get_invalid_block_index() const;

private:
    uint32_t associativity;
    std::vector<CacheBlock> blocks;
    std::unique_ptr<EvictionPolicyInterface> eviction_policy;
};