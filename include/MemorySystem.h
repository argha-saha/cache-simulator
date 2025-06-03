#pragma once

#include "CacheConfig.h"
#include "CacheHierarchy.h"
#include <vector>

class MemorySystem {
public:
    MemorySystem() = default;
    virtual ~MemorySystem() = default;

    // Prevent copying and moving
    MemorySystem(const MemorySystem&) = delete;
    MemorySystem& operator=(const MemorySystem&) = delete;
    MemorySystem(MemorySystem&&) = delete;
    MemorySystem& operator=(MemorySystem&&) = delete;

    void configure_cache(const std::vector<CacheConfig>& configurations);

    void execute_access(uint64_t address, char access_type);

    void print_statistics() const;

private:
    CacheHierarchy hierarchy;
};