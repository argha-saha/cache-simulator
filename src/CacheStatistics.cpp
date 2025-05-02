#include "CacheStatistics.h"

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