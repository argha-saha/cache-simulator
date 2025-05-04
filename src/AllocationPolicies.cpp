#include "AllocationPolicies.h"

bool WriteAllocatePolicy::should_allocate_on_miss(CacheLevel& level, uint64_t address) {
    // Write-allocate policy means allocate a block on a write miss
    return true;
}

bool NoWriteAllocatePolicy::should_allocate_on_miss(CacheLevel& level, uint64_t address) {
    // No write-allocate policy means we do not allocate a block on a write miss
    return false;
}