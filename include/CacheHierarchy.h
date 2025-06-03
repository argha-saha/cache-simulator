#pragma once

#include "CacheConfig.h"
#include "MemoryAccess.h"
#include "Policies.h"

class CacheHierarchy : public MemoryAccessInterface {
public:
    CacheHierarchy() = default;
    virtual ~CacheHierarchy() = default;

    // Prevent copying and moving
    CacheHierarchy(const CacheHierarchy&) = delete;
    CacheHierarchy& operator=(const CacheHierarchy&) = delete;
    CacheHierarchy(CacheHierarchy&&) = delete;
    CacheHierarchy& operator=(CacheHierarchy&&) = delete;

    void add_level(CacheConfig config);

    void cpu_read(uint64_t address);

    void cpu_write(uint64_t address);

    void access_memory(uint64_t address, bool write) override;

    void print_statistics() const;

private:
    std::vector<std::unique_ptr<CacheLevel>> levels;
    uint64_t main_memory_reads = 0;
    uint64_t main_memory_writes = 0;
};