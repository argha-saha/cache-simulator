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

void MemorySystem::execute_access(uint64_t address, char access_type) {
    char type = std::toupper(access_type);

    if (type == 'R') {
        hierarchy.cpu_read(address);
    } else if (type == 'W') {
        hierarchy.cpu_write(address);
    } else {
        std::cerr << "MemorySystem: Invalid access type '" << access_type << "'. Use 'R' for read or 'W' for write." << std::endl;
    }
}

void MemorySystem::print_statistics() const {
    hierarchy.print_statistics();
}