#pragma once

#include "hardware/pin.hh"

const hardware::Pin LOOP_LED_PINS[] = {
    {GPIOA, GPIO_PIN_0},
    {GPIOA, GPIO_PIN_1},
    {GPIOA, GPIO_PIN_2},
    {GPIOC, GPIO_PIN_15},
    {GPIOC, GPIO_PIN_14}, // NOTE: loop 5 is on the GPIO2 out
};

const hardware::Pin TEMPO_PIN = {GPIOC, GPIO_PIN_13};
