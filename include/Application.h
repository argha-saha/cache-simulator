#pragma once

#include "Policies.h"
#include <string>
#include <vector>

class Application {
public:
    void run(int argc, char** argv);

private:
    bool load_config(const std::string& config_file_path);
    
    EvictionPolicyType parse_eviction_policy(const std::string& policy_str) const;

    WritePolicyType parse_write_policy(const std::string& policy_str) const;

    AllocationPolicyType parse_allocation_policy(const std::string& policy_str) const;

    std::vector<struct CacheConfig> loaded_cache_configs;
};