#include "MemorySystem.h"

#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

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

void MemorySystem::execute_access(AccessType access_type, uint64_t address) {
    switch (access_type) {
        case AccessType::R:
        case AccessType::READ:
            hierarchy.cpu_read(address);
            break;
        case AccessType::W:
        case AccessType::WRITE:
            hierarchy.cpu_write(address);
            break;
        default:
            std::cerr << "MemorySystem: Invalid access type. Use R for read or W for write." << std::endl;
            return;
    }
}

void MemorySystem::print_statistics() const {
    hierarchy.print_statistics();
}

std::optional<CacheStatistics> MemorySystem::get_cache_level_statistics(const std::string& level_name) const {
    return hierarchy.get_cache_level_statistics(level_name);
}

std::pair<uint64_t, uint64_t> MemorySystem::get_main_memory_statistics() const {
    return hierarchy.get_main_memory_statistics();
}

void MemorySystem::run_trace(const std::string& trace_file_path) {
    // Open the trace file
    std::ifstream trace_file(trace_file_path);

    if (trace_file.fail()) {
        throw std::runtime_error("MemorySystem: Failed to open trace file: " + trace_file_path);
    }

    std::cout << "Processing trace file: " << trace_file_path << std::endl;

    std::string line;
    int line_number = 0;
    char access_type_char = 0;
    uint64_t address = 0;

    // Trace file parsing loop
    while (std::getline(trace_file, line)) {
        ++line_number;

        std::stringstream line_stream(line);

        // Skip empty lines and comments
        if (line.empty()) continue;
        if (line[0] == '#') continue;
        if (line.substr(0, 2) == "//") continue;

        // Parse the access type and address
        if (line_stream >> access_type_char >> std::hex >> address) {
            AccessType current_access_type;
            char upper_access_type = std::toupper(static_cast<unsigned char>(access_type_char));
            // std::cout << upper_access_type << " " << std::hex << address << std::dec << std::endl;

            if (upper_access_type == 'R') {
                current_access_type = AccessType::READ;
            } else if (upper_access_type == 'W') {
                current_access_type = AccessType::WRITE;
            } else {
                std::cerr << "MemorySystem: Invalid access type '" << access_type_char << "' at line " << line_number << std::endl;
                continue; // Skip invalid line
            }

            execute_access(current_access_type, address);
        } else {
            std::cerr << "MemorySystem: Invalid format in trace file at line " << line_number << ": " << line << std::endl;
        }
    }

    std::cout << "Finished processing trace file: " << trace_file_path << std::endl;
}