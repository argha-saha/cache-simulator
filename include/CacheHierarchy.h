#pragma once

#include <memory>

#include "CacheConfig.h"
#include "CacheLevel.h"
#include "MemoryAccess.h"
#include "Policies.h"

class CacheHierarchy final : public MemoryAccessInterface {
public:
    // Default constructor
    CacheHierarchy() = default;

    // Prevent copying and moving
    CacheHierarchy(const CacheHierarchy&) = delete;
    CacheHierarchy& operator=(const CacheHierarchy&) = delete;
    CacheHierarchy(CacheHierarchy&&) = delete;
    CacheHierarchy& operator=(CacheHierarchy&&) = delete;

    void access_memory(uint64_t address, bool write) override;

    /**
     * @brief Adds a new cache level to the hierarchy.
     * @param config Configuration for the new cache level.
     */
    void add_level(CacheConfig config);

    /**
     * @brief Initiates a CPU read request.
     * @param address Memory address to read from.
     */
    void cpu_read(uint64_t address);

    /**
     * @brief Initiates a CPU write request.
     * @param address Memory address to write to.
     */
    void cpu_write(uint64_t address);

    /**
     * @brief Prints statistics for all cache levels and main memory.
     */
    void print_statistics() const;

private:
    std::vector<std::unique_ptr<CacheLevel>> levels;
    uint64_t main_memory_reads = 0;
    uint64_t main_memory_writes = 0;
};