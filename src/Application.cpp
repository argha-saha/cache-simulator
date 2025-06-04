#include "Application.h"

#include <exception>
#include <iostream>
#include <vector>

#include "AccessType.h"
#include "CacheConfig.h"
#include "MemorySystem.h"
#include "Policies.h"

AccessType R = AccessType::R;
AccessType READ = AccessType::READ;
AccessType W = AccessType::W;
AccessType WRITE = AccessType::WRITE;

void Application::run() {
    try {
        MemorySystem simulator;

        CacheConfig l1_config(
            "L1",
            64 * 1024, // 64 KB
            64,      // 64 bytes block size
            4,    // 4-way set associative
            EvictionPolicyType::LRU,
            WritePolicyType::WRITE_BACK,
            AllocationPolicyType::WRITE_ALLOCATE
        );

        std::vector<CacheConfig> hierarchy_configs = {l1_config};
        simulator.configure_cache(hierarchy_configs);
        
        // Miss
        simulator.execute_access(R, 0x1000);

        // Hit (Same block as previous access)
        simulator.execute_access(W, 0x1004);

        simulator.print_statistics();
    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
    }
}