#pragma once

#include <vector>

#include "AccessType.h"
#include "CacheConfig.h"
#include "CacheHierarchy.h"

class MemorySystem {
public:
    // Default constructor and destructor
    MemorySystem() = default;
    virtual ~MemorySystem() = default;

    // Prevent copying and moving
    MemorySystem(const MemorySystem&) = delete;
    MemorySystem& operator=(const MemorySystem&) = delete;
    MemorySystem(MemorySystem&&) = delete;
    MemorySystem& operator=(MemorySystem&&) = delete;

    /**
     * @brief Configures the memory system with a hierarchy of cache levels.
     * @param configurations A vector of CacheConfig objects defining the cache hierarchy.
     */
    void configure_cache(const std::vector<CacheConfig>& configurations);

    /**
     * @brief Executes a memory access operation (read or write).
     * @param address The memory address to access.
     * @param access_type 'r' for read, 'w' for write.
     */
    void execute_access(AccessType access_type, uint64_t address);

    void print_statistics() const;

private:
    CacheHierarchy hierarchy;
};