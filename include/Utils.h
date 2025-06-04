#pragma once

#include <cstdint>

/**
 * @brief Checks if a number is a power of two.
 * @param n The number to check.
 * @return True if n is a power of two, false otherwise.
 */
bool is_power_of_two(uint64_t n);

/**
 * @brief Calculates the base-2 logarithm of a number.
 * @param n The number to calculate the logarithm for.
 * @return The base-2 logarithm of n.
 */
uint32_t log2(uint64_t n);