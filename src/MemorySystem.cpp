#include "MemorySystem.h"

#include <iostream>
#include <stdexcept>

void MemorySystem::configure_cache(const std::vector<CacheConfig>& configurations) {
    if (configurations.empty()) {
        throw std::invalid_argument("MemorySystem: No cache configurations provided.");
    }

    for (const auto& config : configurations) {
        try {
            hierarchy.add_level(config);
        } catch (const std::exception& e) {
            throw std::runtime_error("MemorySystem: Failed to add cache level: " + std::string(e.what()));
        }
    }
}

void MemorySystem::execute_access(AccessType access_type, uint64_t address) {
    switch (access_type) {
        case AccessType::R:
        case AccessType::READ:
            hierarchy.cpu_read(address);
            break;
        case AccessType::W:
        case AccessType::WRITE:
            hierarchy.cpu_write(address);
            break;
        default:
            std::cerr << "MemorySystem: Invalid access type. Use R for read or W for write." << std::endl;
            return;
    }
}

void MemorySystem::print_statistics() const {
    hierarchy.print_statistics();
}

std::optional<CacheStatistics> MemorySystem::get_cache_level_statistics(const std::string& level_name) const {
    return hierarchy.get_cache_level_statistics(level_name);
}

std::pair<uint64_t, uint64_t> MemorySystem::get_main_memory_statistics() const {
    return hierarchy.get_main_memory_statistics();
}