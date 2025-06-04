#include "Application.h"

#include <exception>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "AccessType.h"
#include "CacheConfig.h"
#include "MemorySystem.h"
#include "Parser.hpp"
#include "Policies.h"

AccessType R = AccessType::READ;
AccessType READ = AccessType::READ;
AccessType W = AccessType::WRITE;
AccessType WRITE = AccessType::WRITE;

using json = nlohmann::json;

EvictionPolicyType Application::parse_eviction_policy(const std::string& policy_str) const {
    static const std::unordered_map<std::string, EvictionPolicyType> policy_map = {
        {"LRU", EvictionPolicyType::LRU},
        {"FIFO", EvictionPolicyType::FIFO},
        {"RANDOM", EvictionPolicyType::RANDOM}
    };

    return parse_policy(policy_str, policy_map, "Invalid eviction policy.");
}

WritePolicyType Application::parse_write_policy(const std::string& policy_str) const {
    static const std::unordered_map<std::string, WritePolicyType> policy_map = {
        {"WRITE_BACK", WritePolicyType::WRITE_BACK},
        {"WRITE_THROUGH", WritePolicyType::WRITE_THROUGH}
    };

    return parse_policy(policy_str, policy_map, "Invalid write policy.");
}

AllocationPolicyType Application::parse_allocation_policy(const std::string& policy_str) const {
    static const std::unordered_map<std::string, AllocationPolicyType> policy_map = {
        {"WRITE_ALLOCATE", AllocationPolicyType::WRITE_ALLOCATE},
        {"NO_WRITE_ALLOCATE", AllocationPolicyType::NO_WRITE_ALLOCATE}
    };

    return parse_policy(policy_str, policy_map, "Invalid allocation policy.");
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
        std::string config_file_path;
        std::string trace_file_path;

        // Positional arguments
        if (argc > 1) {
            if (!load_config(argv[1])) {
                std::cerr << "Error: Failed to load cache configuration from file: " << argv[1] << std::endl;

                CacheConfig l1_config(
                    "L1",
                    64 * 1024, // 64 KB
                    64,      // 64 bytes block size
                    4,    // 4-way set associative
                    EvictionPolicyType::LRU,
                    WritePolicyType::WRITE_BACK,
                    AllocationPolicyType::WRITE_ALLOCATE
                );

                loaded_cache_configs = {l1_config};
            }

            if (argc > 2) {
                trace_file_path = argv[2];
            } else {
                std::cerr << "Warning: Trace file path not provided. Defaulting to hardcoded trace." << std::endl;
            }
        } else {
            std::cout << "No configuration file provided. Using hardcoded default L1 cache." << std::endl;
            std::cout << "Usage: " << argv[0] << " [config_file_path] [trace_file_path]" << std::endl;

            CacheConfig l1_config(
                "L1",
                64 * 1024, // 64 KB
                64,      // 64 bytes block size
                4,    // 4-way set associative
                EvictionPolicyType::LRU,
                WritePolicyType::WRITE_BACK,
                AllocationPolicyType::WRITE_ALLOCATE
            );

            loaded_cache_configs = {l1_config};
        }

        if (loaded_cache_configs.empty()) {
            std::cerr << "Error: No cache configurations loaded." << std::endl;
            return;
        }

        simulator.configure_cache(loaded_cache_configs);

        if (!trace_file_path.empty()) {
            simulator.run_trace(trace_file_path);
        } else {
            std::cout << "\nRunning Hardcoded Trace...\n";
            simulator.execute_access(R, 0x1000);
            simulator.execute_access(W, 0x1004);
        }

        simulator.print_statistics();
    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
    }
}