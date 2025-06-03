#include "MemorySystem.h"
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