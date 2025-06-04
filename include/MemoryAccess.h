#pragma once

#include <cstdint>

class MemoryAccessInterface {
public:
    // Default destructor
    virtual ~MemoryAccessInterface() = default;

    /**
     * @brief Accesses memory at the specified address.
     * @param address Memory address to access.
     * @param write True if this is a write operation, false for read.
     */
    virtual void access_memory(uint64_t address, bool write) = 0;
};