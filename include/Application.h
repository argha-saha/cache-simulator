#pragma once

#include "CacheConfig.h"
#include "Policies.h"
#include <string>
#include <vector>

class Application {
public:
    /**
     * @brief Runs the application with the given command line arguments.
     */
    void run(int argc, char** argv);

private:
    /**
     * @brief Parses the eviction policy from a string.
     * @param policy_str The string representation of the eviction policy (e.g., "LRU", "FIFO", "RANDOM").
     * @return The parsed EvictionPolicyType.
     */
    EvictionPolicyType parse_eviction_policy(const std::string& policy_str) const;

    /**
     * @brief Parses the write policy from a string.
     * @param policy_str The string representation of the write policy (e.g., "WRITE_BACK", "WRITE_THROUGH").
     * @return The parsed WritePolicyPolicyType.
     */
    WritePolicyType parse_write_policy(const std::string& policy_str) const;

    /**
     * @brief Parses the allocation policy from a string.
     * @param policy_str The string representation of the allocation policy (e.g., "WRITE_ALLOCATE", "NO_WRITE_ALLOCATE").
     * @return The parsed AllocationPolicyType.
     */
    AllocationPolicyType parse_allocation_policy(const std::string& policy_str) const;

    /**
     * @brief Loads the cache configuration from a JSON file.
     * @param config_file_path The path to the JSON configuration file.
     * @return True if the configuration was loaded successfully, false otherwise.
     */
    bool load_config(const std::string& config_file_path);

    std::vector<CacheConfig> loaded_cache_configs;
};