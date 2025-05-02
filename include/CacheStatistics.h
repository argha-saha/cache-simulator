#pragma once

#include <cstdint>

struct CacheStatistics {
    uint64_t read_hits = 0;
    uint64_t read_misses = 0;
    uint64_t write_hits = 0;
    uint64_t write_misses = 0;
    uint64_t evictions = 0;
    uint64_t dirty_evictions = 0;

    double read_hit_rate() const;
    double write_hit_rate() const;
    double overall_hit_rate() const;
    uint64_t total_accesses() const;
};