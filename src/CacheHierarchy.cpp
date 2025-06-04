#include "CacheHierarchy.h"

#include <iostream>

void CacheHierarchy::add_level(CacheConfig config) {
    CacheLevel* previous_level = levels.empty() ? nullptr : levels.back().get();
    auto new_level = std::make_unique<CacheLevel>(config, nullptr);
    new_level->set_memory_accessor(this);

    if (previous_level) {
        previous_level->set_next_level(new_level.get());
    }

    levels.push_back(std::move(new_level));
}

void CacheHierarchy::cpu_read(uint64_t address) {
    if (levels.empty()) {
        access_memory(address, false);
    } else {
        levels.front()->read(address);
    }
}

void CacheHierarchy::cpu_write(uint64_t address) {
    if (levels.empty()) {
        access_memory(address, true);
    } else {
        levels.front()->write(address);
    }
}

void CacheHierarchy::access_memory(uint64_t address, bool write) {
    if (write) {
        ++main_memory_writes;
    } else {
        ++main_memory_reads;
    }
}

void CacheHierarchy::print_statistics() const {
    std::cout << "\n=== Cache Hierarchy Statistics ===\n";

    if (levels.empty()) {
        std::cout << "No cache levels defined.\n";
    } else {
        for (const auto& level_ptr : levels) {
            if (level_ptr) {
                level_ptr->get_statistics().print(level_ptr->get_name());
            }
        }
    }

    std::cout << "\n=== Main Memory Accesses ===\n";
    std::cout << "Reads: " << main_memory_reads << std::endl;
    std::cout << "Writes: " << main_memory_writes << std::endl;
}

std::optional<CacheStatistics> CacheHierarchy::get_cache_level_statistics(const std::string& level_name) const {
    for (const auto& level_ptr : levels) {
        if (level_ptr && level_ptr->get_name() == level_name) {
            return level_ptr->get_statistics();
        }
    }

    return std::nullopt;
}

std::pair<uint64_t, uint64_t> CacheHierarchy::get_main_memory_statistics() const {
    return {main_memory_reads, main_memory_writes};
}