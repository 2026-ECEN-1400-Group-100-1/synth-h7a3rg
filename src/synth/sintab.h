#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define SIN_TABLE_RES 4096
#define SIN_MAX 4095

extern uint16_t SIN_TABLE[SIN_TABLE_RES];

/// @brief Generates the sin lookup table
/// @note Only needs to be run once
void generate_sintab(void);

/// @brief Looks up a value in the sin lookup table
/// @param val The value to look up. Can be any uint16_t
/// @returns sin(2pi*val), shifted to a range of 0 through SIN_MAX
inline uint16_t sintab(uint16_t val) {
    return SIN_TABLE[val % SIN_TABLE_RES];
}

#ifdef __cplusplus
}
#endif
