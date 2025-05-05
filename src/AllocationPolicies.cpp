#include "AllocationPolicies.h"
#include "CacheLevel.h"

bool WriteAllocatePolicy::should_allocate_on_miss(CacheLevel& level, uint64_t address) {
    // Write-allocate policy means allocate a block on a write miss
    return true;
}

bool NoWriteAllocatePolicy::should_allocate_on_miss(CacheLevel& level, uint64_t address) {
    CacheLevel* next_cache_level = level.get_next_level();
    MemoryAccessInterface* memory_accessor = level.get_memory_accessor();

    if (next_cache_level) {
        next_cache_level->write(address);
    } else if (memory_accessor) {
        memory_accessor->access_memory(address, true);
    } else {
        std::cerr << "No next cache level or memory accessor available for write miss." << std::endl;
    }

    return false;
}