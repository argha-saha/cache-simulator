#include "Policies.h"
#include "LRUPolicy.h"
#include "RandomPolicy.h"
#include "FIFOPolicy.h"
#include "WriteBackPolicy.h"
#include "WriteThroughPolicy.h"
#include "AllocationPolicies.h"

std::unique_ptr<EvictionPolicyInterface> create_eviction_policy(EvictionPolicyType policy) {
    switch (policy) {
        case EvictionPolicyType::LRU:
            return std::make_unique<LRUPolicy>();
        case EvictionPolicyType::RANDOM:
            return std::make_unique<RandomPolicy>();
        case EvictionPolicyType::FIFO:
            return std::make_unique<FIFOPolicy>();
        default:
            throw std::invalid_argument("Unsupported eviction policy type.");
    }
}

std::unique_ptr<WritePolicyInterface> create_write_policy(WritePolicyType policy) {
    switch (policy) {
        case WritePolicyType::WRITE_BACK:
            return std::make_unique<WriteBackPolicy>();
        case WritePolicyType::WRITE_THROUGH:
            return std::make_unique<WriteThroughPolicy>();
        default:
            throw std::invalid_argument("Unsupported write policy type.");
    }
}

std::unique_ptr<AllocationPolicyInterface> create_allocation_policy(AllocationPolicyType policy) {
    switch (policy) {
        case AllocationPolicyType::WRITE_ALLOCATE:
            return std::make_unique<WriteAllocatePolicy>();
        case AllocationPolicyType::NO_WRITE_ALLOCATE:
            return std::make_unique<NoWriteAllocatePolicy>();
        default:
            throw std::invalid_argument("Unsupported allocation policy type.");
    }
}