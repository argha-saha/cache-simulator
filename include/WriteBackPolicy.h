#pragma once

#include "Policies.h"

/**
 * @brief Implements the write-back policy for cache writes.
 */
class WriteBackPolicy final : public WritePolicyInterface {
public:
    virtual void on_write_hit(
        CacheLevel& current_level, 
        uint64_t address, 
        uint32_t block_index
    ) override;
};