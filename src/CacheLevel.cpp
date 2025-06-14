#include "CacheLevel.h"

#include <iostream>
#include <limits>

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
    if (config.capacity % config.block_size != 0) {
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

void CacheLevel::handle_miss(uint64_t address, bool is_write) {
    // std::cout << "DEBUG: CacheLevel " << name << ": Entered handleMiss for addr 0x" << std::hex << address << std::dec
    //           << ", is_write=" << is_write << std::endl;

    // if (next_level) {
    //     std::cout << "DEBUG: CacheLevel " << name << ": next_level is NOT NULL. Calling next_level->read()." << std::endl;
    // } else {
    //     std::cout << "DEBUG: CacheLevel " << name << ": next_level IS NULL." << std::endl;
    // }

    // if (memory_accessor) {
    //     std::cout << "DEBUG: CacheLevel " << name << ": memory_accessor is NOT NULL." << std::endl;
    // } else {
    //     std::cout << "DEBUG: CacheLevel " << name << ": memory_accessor IS NULL." << std::endl;
    // }

    if (next_level) {
        // Read request to fetch data from the next level
        next_level->read(address);
    } else if (memory_accessor) {
        // Read request to fetch data from main memory
        memory_accessor->access_memory(address, false);
        fill(address);
    } else {
        throw std::runtime_error("CacheLevel: No next level or memory accessor defined for cache miss handling.");
    }
}

CacheSet& CacheLevel::get_set(uint32_t index) {
    try {
        return sets.at(index);
    } catch (const std::out_of_range&) {
        throw std::out_of_range("CacheLevel: Set index out of range.");
    }
}

void CacheLevel::evict_and_replace(const MemoryAddress& address, uint32_t set_index) {
    CacheSet& set = get_set(set_index);

    // 1. Determine the victim block
    uint32_t victim_index = set.get_victim_index();

    // 2. Get the state of the victim block before eviction
    const CacheBlock& victim_block = set.get_block(victim_index);
    const bool victim_valid = victim_block.is_valid();
    const bool victim_dirty = victim_block.is_dirty();

    // 3. Handle write-back
    if (victim_valid && victim_dirty) {
        ++statistics.evictions;
        ++statistics.dirty_evictions;

        // Reconstruct the address of the victim block
        uint64_t victim_address = victim_block.get_address(set_index, index_bits, block_offset_bits);

        if (victim_address != std::numeric_limits<uint64_t>::max()) {
            if (next_level) {
                // Write to the next cache level
                next_level->write(victim_address);
            } else if (memory_accessor) {
                // Write to main memory
                memory_accessor->access_memory(victim_address, true);
            } else {
                std::cerr << "CacheLevel: No next level or memory accessor defined for write-back." << std::endl;
            }
        } else {
            std::cerr << "CacheLevel: Invalid victim address during eviction." << std::endl;
        }
    } else if (victim_valid) {
        ++statistics.evictions;
    }

    // 4. Update the block's state
    set.replace_block(victim_index, address.tag, false);
}

bool CacheLevel::read(uint64_t address) {
    // Decompose the address
    MemoryAddress mem_address(address, block_size, num_sets);

    // Get the set associated with the index
    CacheSet& set = get_set(mem_address.index);

    // Try to find a block with a matching tag in this set
    std::optional<uint32_t> block_index = set.find_block(mem_address.tag);

    if (block_index.has_value()) {
        // Read hit
        ++statistics.read_hits;
        set.access_block(block_index.value());
        return true;
    } else {
        // Read miss
        ++statistics.read_misses;
        
        // Fetch data from lower memory level
        handle_miss(address, false);

        // std::cout << "DEBUG: CacheLevel " << name << ": After handling miss for addr 0x" << std::hex << address << std::dec
        //           << ". Re-checking for tag 0x" << mem_address.tag << " in set " << mem_address.index << std::endl;

        // TODO: Check if this is correct
        block_index = set.find_block(mem_address.tag);
        if (block_index.has_value()) {
            // If the block was found after fetching, access it
            set.access_block(block_index.value());
            return true;
        } else {
            std::cerr << "CacheLevel: Invalid fetch after read miss." << std::endl;
        }

        // Initially a miss
        return false;
    }
}

bool CacheLevel::write(uint64_t address) {
    // Decompose the address
    MemoryAddress mem_address(address, block_size, num_sets);

    // Get the set associated with the index
    CacheSet& set = get_set(mem_address.index);

    // Try to find a block with a matching tag in this set
    std::optional<uint32_t> block_index = set.find_block(mem_address.tag);

    if (block_index.has_value()) {
        // Write hit
        ++statistics.write_hits;
        write_policy->on_write_hit(*this, address, *block_index);
        return true;
    } else {
        // Write miss
        ++statistics.write_misses;
        
        if (allocation_policy->should_allocate_on_miss(*this, address)) {
            // 1. Fetch the block
            handle_miss(address, true);

            // 2. Perform write hit logic
            block_index = set.find_block(mem_address.tag);

            if (block_index.has_value()) {
                write_policy->on_write_hit(*this, address, *block_index);
            } else {
                std::cerr << "CacheLevel: Invalid fetch after write miss." << std::endl;
            }
        } else {
            // No-write-allocate
        }

        return false;
    }
}

void CacheLevel::fill(uint64_t address) {
    MemoryAddress mem_address(address, block_size, num_sets);
    evict_and_replace(mem_address, mem_address.index);
}