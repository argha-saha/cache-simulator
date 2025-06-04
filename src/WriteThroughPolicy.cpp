#include "CacheLevel.h"
#include "MemoryAccess.h"
#include "WriteThroughPolicy.h"

void WriteThroughPolicy::on_write_hit(
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

    // In write-through policy, the block is considered clean after the write since the lower level is updated immediately
    block.set_dirty(false);

    // Update the eviction policy
    set.access_block(block_index);

    // Perform the write-through operation
    CacheLevel* next_cache_level = current_level.get_next_level();
    MemoryAccessInterface* memory_accessor = current_level.get_memory_accessor();

    // Prioritize writing to the next cache level if it exists
    if (next_cache_level) {
        next_cache_level->write(address);
    } else if (memory_accessor) {
        memory_accessor->access_memory(address, true);
    } else {
        std::cerr << "CacheLevel: No next level or memory accessor defined for write-through operation." << std::endl;
    }
}