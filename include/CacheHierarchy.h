#pragma once

#include <memory>
#include <optional>

#include "CacheConfig.h"
#include "CacheLevel.h"
#include "CacheStatistics.h"
#include "MemoryAccessInterface.h"
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

    /**
     * @brief Retrieves statistics for a specific cache level by name.
     * @param level_name Name of the cache level.
     * @return Optional CacheStatistics if the level exists, otherwise std::nullopt.
     */
    std::optional<CacheStatistics> get_cache_level_statistics(const std::string& level_name) const;

    /**
     * @brief Retrieves the main memory access statistics.
     * @return A pair containing the number of reads and writes to main memory.
     */
    std::pair<uint64_t, uint64_t> get_main_memory_statistics() const;

private:
    std::vector<std::unique_ptr<CacheLevel>> levels;
    uint64_t main_memory_reads = 0;
    uint64_t main_memory_writes = 0;
};