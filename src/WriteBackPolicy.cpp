#include "CacheLevel.h"
#include "WriteBackPolicy.h"

void WriteBackPolicy::on_write_hit(
    CacheLevel& current_level, 
    uint64_t address, 
    uint32_t block_index)
{
    // Decompose the address to find the correct set
    MemoryAddress mem_address(address, current_level.get_block_size(), current_level.get_num_sets());

    // Access the cache set
    CacheSet& set = current_level.get_set(mem_address.index);

    // Access the block in the set
    CacheBlock& block = set.get_block(block_index);

    // Mark the block as dirty
    block.set_dirty(true);

    // Update the eviction policy
    set.access_block(block_index);
}