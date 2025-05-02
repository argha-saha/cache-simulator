#pragma once

#include <cstdint>
#include <memory>

class CacheSet;

class EvictionPolicyInterface {
public:
    virtual ~EvictionPolicyInterface() = default;

    virtual void initialize(uint32_t associativity) = 0;
    virtual void on_access(const CacheSet& set, uint32_t index) = 0;
    virtual void on_fill(const CacheSet& set, uint32_t index) = 0;
    virtual uint32_t get_victim(const CacheSet& set) = 0;
    virtual std::unique_ptr<EvictionPolicyInterface> clone() const = 0;
};