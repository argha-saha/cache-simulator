#pragma once

#include <cstdint>
#include <memory>

class CacheSet;
class CacheLevel;

/**
 * @brief Interface defining the rules for cache eviction policies.
 * Eviction policy implementations manage their own state and behavior.
 */
class EvictionPolicyInterface {
public:
    virtual ~EvictionPolicyInterface() = default;

    virtual void initialize(uint32_t associativity) = 0;
    virtual void on_access(const CacheSet& set, uint32_t index) = 0;
    virtual void on_fill(const CacheSet& set, uint32_t index) = 0;
    virtual uint32_t get_victim(const CacheSet& set) = 0;
    virtual std::unique_ptr<EvictionPolicyInterface> clone() const = 0;
};

/**
 * @brief Interface defining the rules for handling cache write hits.
 * Determines the action to take when a write hit occurs (e.g., mark a block dirty).
 */
class WritePolicyInterface {
public:
    virtual ~WritePolicyInterface() = default;
    virtual void on_write_hit(
        class CacheLevel& current_level, 
        uint64_t address, 
        uint32_t block_index
    ) = 0;
};

/** 
 * @brief Interface defining the rules for handling cache write misses.
 * Determines if a block should be allocated on a write miss (write-allocate vs no-write-allocate). 
 */
class AllocationPolicyInterface {
public:
    virtual ~AllocationPolicyInterface() = default;
    virtual bool should_allocate_on_miss(CacheLevel& level, uint64_t address) = 0;
};

/** 
 * @brief Available eviction policies (e.g. LRU) 
 */
enum class EvictionPolicyType {
    LRU,
    RANDOM,
    FIFO
};

/** 
 * @brief Available write policies (write-back and write-through) 
 */
enum class WritePolicyType {
    WRITE_BACK,
    WRITE_THROUGH
};

/** 
 * @brief Available allocation policies (write-allocate and no-write-allocate) 
 */
enum class AllocationPolicyType {
    WRITE_ALLOCATE,
    NO_WRITE_ALLOCATE
};

/** 
 * @brief Creates an instance of a specific eviction policy.
 * @param policy The EvictionPolicyType to create.
 * @return A unique pointer to the created EvictionPolicyInterface.
 */
std::unique_ptr<EvictionPolicyInterface> create_eviction_policy(EvictionPolicyType policy);

/** 
 * @brief Creates an instance of a specific write policy.
 * @param policy The WritePolicyType to create.
 * @return A unique pointer to the created WritePolicyInterface.
 */
std::unique_ptr<WritePolicyInterface> create_write_policy(WritePolicyType policy);

/** 
 * @brief Creates an instance of a specific allocation policy.
 * @param policy The AllocationPolicyType to create.
 * @return A unique pointer to the created AllocationPolicyInterface.
 */
std::unique_ptr<AllocationPolicyInterface> create_allocation_policy(AllocationPolicyType policy);