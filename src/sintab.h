#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <math.h>

#define SIN_TABLE_RES 1024

extern int16_t SIN_TABLE[SIN_TABLE_RES];

/// @brief Generates the sin lookup table
/// @note Only needs to be run once
void generate_sintab(void);

/// @brief Looks up a value in the sin lookup table
inline int16_t sintab(int16_t val) {
    return SIN_TABLE[val % SIN_TABLE_RES];
}

/// @brief Looks up a radian value in the sin lookup table
inline float sintab_rad(float val) {
    // take it over the max val for a signed 16 bit integer
    return SIN_TABLE[(size_t)roundf(val / (2 * M_PI) * SIN_TABLE_RES)] / 32768;
}

#ifdef __cplusplus
}
#endif
