#pragma once

#include <cstdint>
#include <iostream>

struct CacheStatistics {
    uint64_t read_hits = 0;
    uint64_t read_misses = 0;
    uint64_t write_hits = 0;
    uint64_t write_misses = 0;
    uint64_t evictions = 0;
    uint64_t dirty_evictions = 0;

    /** 
     * @brief Calculates the total number of accesses for reads and writes.
     * @return Total number of accesses.
     */
    uint64_t total_accesses() const;

    /**
     * @brief Calculates the read hit rate (read hits / total read accesses).
     * @return Read hit rate as a double.
     */
    double read_hit_rate() const;

    /**
     * @brief Calculates the write hit rate (write hits / total write accesses).
     * @return Write hit rate as a double.
     */
    double write_hit_rate() const;

    /**
     * @brief Calculates the overall hit rate (total hits / total accesses).
     * @return Overall hit rate as a double.
     */
    double overall_hit_rate() const;

    /**
     * @brief Prints the statistics to the console.
     */
    void print(const std::string& cache_name) const;
};