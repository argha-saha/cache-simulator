#include "Application.h"

#include <exception>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <nlohmann/json.hpp>

#include "AccessType.h"
#include "CacheConfig.h"
#include "MemorySystem.h"
#include "Policies.h"

AccessType R = AccessType::R;
AccessType READ = AccessType::READ;
AccessType W = AccessType::W;
AccessType WRITE = AccessType::WRITE;

using json = nlohmann::json;

EvictionPolicyType Application::parse_eviction_policy(const std::string& policy_str) const {
    std::string policy_str_upper = policy_str;
    if (policy_str_upper == "LRU") return EvictionPolicyType::LRU;
    if (policy_str_upper == "FIFO") return EvictionPolicyType::FIFO;
    if (policy_str_upper == "Random") return EvictionPolicyType::RANDOM;
    throw std::invalid_argument("Invalid eviction policy: " + policy_str);
}

WritePolicyType Application::parse_write_policy(const std::string& policy_str) const {
    std::string policy_str_upper = policy_str;
    if (policy_str_upper == "WRITE_BACK") return WritePolicyType::WRITE_BACK;
    if (policy_str_upper == "WRITE_THROUGH") return WritePolicyType::WRITE_THROUGH;
    throw std::invalid_argument("Invalid write policy: " + policy_str);
}

AllocationPolicyType Application::parse_allocation_policy(const std::string& policy_str) const {
    std::string policy_str_upper = policy_str;
    if (policy_str_upper == "WRITE_ALLOCATE") return AllocationPolicyType::WRITE_ALLOCATE;
    if (policy_str_upper == "NO_WRITE_ALLOCATE") return AllocationPolicyType::NO_WRITE_ALLOCATE;
    throw std::invalid_argument("Invalid allocation policy: " + policy_str);
}

bool Application::load_config(const std::string& config_file_path) {
    std::ifstream config_file(config_file_path);

    if (!config_file.is_open()) {
        std::cerr << "Error: Failed to parse JSON configuration file: " << config_file_path << std::endl;
        return false;
    }

    json json_config;

    try {
        // Parse the entire file into a JSON object
        config_file >> json_config;
    } catch (json::parse_error& e) {
        std::cerr << "Error: Failed to parse JSON configuration file: " << config_file_path << std::endl;
        std::cerr << "Parse error: " << e.what() << std::endl;
        return false;
    }

    config_file.close();

    loaded_cache_configs.clear();

    try {
        if (json_config.contains("cache_levels") && json_config["cache_levels"].is_array()) {
            for (const auto& level : json_config["cache_levels"]) {
                CacheConfig new_config;

                if (level.contains("name") && level["name"].is_string()) {
                    new_config.name = level["name"].get<std::string>();
                } else {
                    throw std::invalid_argument("Cache level must have a 'name' field.");
                }

                if (level.contains("capacity") && level["capacity"].is_number()) {
                    new_config.capacity = level["capacity"].get<uint64_t>();
                } else {
                    throw std::invalid_argument("Cache level must have a 'capacity' field.");
                }

                if (level.contains("block_size") && level["block_size"].is_number()) {
                    new_config.block_size = level["block_size"].get<uint32_t>();
                } else {
                    throw std::invalid_argument("Cache level must have a 'block_size' field.");
                }

                if (level.contains("associativity") && level["associativity"].is_number()) {
                    new_config.associativity = level["associativity"].get<uint32_t>();
                } else {
                    throw std::invalid_argument("Cache level must have an 'associativity' field.");
                }

                if (level.contains("eviction_policy") && level["eviction_policy"].is_string()) {
                    new_config.eviction_policy = parse_eviction_policy(level["eviction_policy"].get<std::string>());
                } else {
                    throw std::invalid_argument("Cache level must have an 'eviction_policy' field.");
                }

                if (level.contains("write_policy") && level["write_policy"].is_string()) {
                    new_config.write_policy = parse_write_policy(level["write_policy"].get<std::string>());
                } else {
                    throw std::invalid_argument("Cache level must have a 'write_policy' field.");
                }

                if (level.contains("allocation_policy") && level["allocation_policy"].is_string()) {
                    new_config.allocation_policy = parse_allocation_policy(level["allocation_policy"].get<std::string>());
                } else {
                    throw std::invalid_argument("Cache level must have an 'allocation_policy' field.");
                }

                // Add the new configuration to the loaded cache configurations
                loaded_cache_configs.push_back(new_config);
            }
        } else {
            std::cerr << "Error: JSON configuration file does not contain 'cache_levels' array." << std::endl;
            return false;
        }
    } catch (const json::exception& e) {
        std::cerr << "Error: Failed to parse cache levels from JSON configuration file: " << config_file_path << std::endl;
        std::cerr << "JSON error: " << e.what() << std::endl;
        return false;
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: Invalid argument while parsing cache levels: " << e.what() << std::endl;
        return false;
    } catch (const std::exception& e) {
        std::cerr << "Error: An unexpected error occurred while parsing cache levels: " << e.what() << std::endl;
        return false;
    }

    return true;
}

void Application::run(int argc, char** argv) {
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

        if (argc < 2) {
            // Miss
            simulator.execute_access(R, 0x1000);

            // Hit (Same block as previous access)
            simulator.execute_access(W, 0x1004);

            simulator.print_statistics();
        } else {
            std::string trace_file_path = argv[1];
            std::cout << "\n=== Running Simulation from Trace File ===\n";

            simulator.run_trace(trace_file_path);
            simulator.print_statistics();
        }
    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
    }
}