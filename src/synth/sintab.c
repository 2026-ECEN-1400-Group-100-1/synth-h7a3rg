#include "sintab.h"

#include <math.h>

uint16_t SIN_TABLE[] = {};

void generate_sintab() {
    for (int i = 0; i < SIN_TABLE_RES; ++i) {
        // NOLINTBEGIN - ignore narrowing conversion warning, we're doing it on purpose
        SIN_TABLE[i] =
            (sin(2.0 * M_PI * ((double)i / (double)SIN_TABLE_RES)) + 1) * (SIN_MAX / 2);
        // NOLINTEND
    }
}
