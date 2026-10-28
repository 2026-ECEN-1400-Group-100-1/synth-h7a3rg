#include "sintab.h"

#include <math.h>

int16_t SIN_TABLE[] = {};

void generate_sintab() {
    for (int i = 0; i < SIN_TABLE_RES; ++i) {
        SIN_TABLE[i] = sin(2 * M_PI * ((float)i / (float)SIN_TABLE_RES));
    }
}
