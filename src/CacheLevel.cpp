#include "CacheLevel.h"
#include "Utils.h"

CacheLevel::CacheLevel(CacheConfig config, CacheLevel* next_level)
    : name(config.name),
      capacity(config.capacity),
      block_size(config.block_size),
      write_policy(create_write_policy(config.write_policy)),
      allocation_policy(create_allocation_policy(config.allocation_policy)),
      next_level(next_level),
      memory_accessor(nullptr)
{
    // Capacity and block size must be greater than zero
    if (config.capacity == 0 || config.block_size == 0) {
        throw std::invalid_argument("CacheLevel: Invalid cache size parameters.");
    }

    // Associativity must be greater than zero
    if (config.associativity == 0) {
        throw std::invalid_argument("CacheLevel: Associativity must be greater than zero.");
    }

    // Block size must be a power of two
    if (!is_power_of_two(config.block_size)) {
        throw std::invalid_argument("CacheLevel: Block size must be a power of two.");
    }

    // Total size must be divisible by block size
    if (config.capacity & config.block_size != 0) {
        throw std::invalid_argument("CacheLevel: Cache capacity must be divisible by block size.");
    }

    const uint64_t num_blocks = config.capacity / config.block_size;

    if (config.associativity >= num_blocks) {
        // Fully associative cache
        num_sets = 1;
        associativity = static_cast<uint32_t>(num_blocks);
    } else {
        // Direct-mapped or set-associative cache
        associativity = config.associativity;

        // Number of blocks must be divisible by associativity
        if (num_blocks % config.associativity != 0) {
            throw std::invalid_argument("CacheLevel: Number of blocks must be divisible by associativity.");
        }

        num_sets = static_cast<uint32_t>(num_blocks / config.associativity);

        if (!is_power_of_two(num_sets)) {
            throw std::invalid_argument("CacheLevel: Number of sets must be a power of two.");
        }
    }

    block_offset_bits = log2(config.block_size);

    // Fully-associative cache has no index bits
    if (num_sets == 1) {
        index_bits = 0;
    } else {
        index_bits = log2(num_sets);
    }

    // Create eviction policy prototype
    std::unique_ptr<EvictionPolicyInterface> eviction_policy = create_eviction_policy(config.eviction_policy);
    if (!eviction_policy) {
        throw std::runtime_error("CacheLevel: Failed to create eviction policy.");
    }

    sets.reserve(num_sets);

    // Create each CacheSet
    // CacheSet constructor will clone the eviction policy
    for (uint32_t i = 0; i < num_sets; ++i) {
        sets.emplace_back(associativity, *eviction_policy);
    }
}