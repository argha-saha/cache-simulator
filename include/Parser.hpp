#pragma once

#include <cctype>
#include <stdexcept>
#include <string>
#include <unordered_map>

template <typename EnumType>
EnumType parse_policy(
    const std::string& policy_str, 
    const std::unordered_map<std::string, EnumType>& policy_map,
    const std::string& error_msg
) 
{
    std::string key = policy_str;
    std::transform(key.begin(), key.end(), key.begin(), ::toupper);

    auto it = policy_map.find(key);

    if (it != policy_map.end()) {
        return it->second;
    } else {
        throw std::invalid_argument(error_msg + ": " + policy_str);
    }
}