#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define SIN_TABLE_RES 1024

extern int16_t SIN_TABLE[SIN_TABLE_RES];

/// @brief Generates the sin lookup table
/// @note Only needs to be run once
void generate_sintab(void);

inline int16_t sintab(int16_t val) {
    return SIN_TABLE[val % SIN_TABLE_RES];
}

#ifdef __cplusplus
}
#endif
