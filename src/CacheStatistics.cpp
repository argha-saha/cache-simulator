#include "CacheStatistics.h"

#include <iostream>

uint64_t CacheStatistics::total_accesses() const {
    return read_hits + read_misses + write_hits + write_misses;
}

double CacheStatistics::read_hit_rate() const {
    uint64_t total_reads = read_hits + read_misses;
    return (total_reads == 0) ? 0.0 : (static_cast<double>(read_hits) / total_reads);
}

double CacheStatistics::write_hit_rate() const {
    uint64_t total_writes = write_hits + write_misses;
    return (total_writes == 0) ? 0.0 : (static_cast<double>(write_hits) / total_writes);
}

double CacheStatistics::overall_hit_rate() const {
    uint64_t accesses = total_accesses();
    return (accesses == 0) ? 0.0 : (static_cast<double>(read_hits + write_hits) / accesses);
}

void CacheStatistics::print(const std::string& cache_name) const {
    std::cout << "Cache Statistics for " << cache_name << std::endl;
    std::cout << "Total Accesses: " << total_accesses() << std::endl;
    std::cout << "Read Hits: " << read_hits << std::endl;
    std::cout << "Read Misses: " << read_misses << std::endl;
    std::cout << "Write Hits: " << write_hits << std::endl;
    std::cout << "Write Misses: " << write_misses << std::endl;
    std::cout << "Read Hit Rate: " << read_hit_rate() << std::endl;
    std::cout << "Write Hit Rate: " << write_hit_rate() << std::endl;
    std::cout << "Overall Hit Rate: " << overall_hit_rate() << std::endl;
    std::cout << "Evictions: " << evictions << std::endl;
    std::cout << "Dirty Evictions: " << dirty_evictions << std::endl;
}