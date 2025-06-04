#pragma once

#include "Policies.h"
#include "MemoryAddress.h"

class WriteThroughPolicy : public WritePolicyInterface {
public:
    virtual void on_write_hit(
        CacheLevel& current_level, 
        uint64_t address, 
        uint32_t block_index
    ) override;
};