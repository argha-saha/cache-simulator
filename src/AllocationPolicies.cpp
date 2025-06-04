#include "AllocationPolicies.h"

#include <iostream>

#include "CacheLevel.h"

bool WriteAllocatePolicy::should_allocate_on_miss(CacheLevel& level, uint64_t address) {
    // Write-allocate policy means allocate a block on a write miss
    return true;
}

bool NoWriteAllocatePolicy::should_allocate_on_miss(CacheLevel& level, uint64_t address) {
    // Get pointers to the next cache level and the memory accessor interface
    CacheLevel* next_cache_level = level.get_next_level();
    MemoryAccessInterface* memory_accessor = level.get_memory_accessor();

    // Prioritize writing to the next cache level if available
    if (next_cache_level) {
        next_cache_level->write(address);
    } else if (memory_accessor) {
        // Write directly to main memory
        memory_accessor->access_memory(address, true);
    } else {
        // No cache level or memory accessor available so raise an error
        std::cerr << "No next cache level or memory accessor available for write miss." << std::endl;
    }

    // No allocation on write miss
    return false;
}