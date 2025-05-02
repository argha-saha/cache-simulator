#pragma once

#include <cstdint>

class MemoryAccessInterface {
public:
    virtual ~MemoryAccessInterface() = default;
    virtual void access_memory(uint64_t address, bool write) = 0;
};