#pragma once

#include <optional>
#include <vector>

#include "AccessType.h"
#include "CacheConfig.h"
#include "CacheHierarchy.h"
#include "CacheStatistics.h"

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

    /**
     * @brief Prints the statistics of the memory system, including hit/miss rates and access counts.
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

    /**
     * @brief Runs a trace file to simulate memory accesses.
     * @param trace_file_path Path to the trace file.
     */
    void run_trace(const std::string& trace_file_path);

private:
    CacheHierarchy hierarchy;
};