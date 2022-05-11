#include "pins.hpp"
#include "pico/stdio.h"
#include "pico/stdlib.h"

extern uint8_t INPUT_PINS[11] = {
    A_PIN,     B_PIN,     C_PIN,

    START_PIN,

    UP_PIN,    RIGHT_PIN, DOWN_PIN, LEFT_PIN,

    X_PIN,     Y_PIN,     Z_PIN,
};

extern uint8_t OUTPUT_PINS[6] = {
    D0_PIN, D1_PIN, D2_PIN, D3_PIN, D4_PIN, D5_PIN,
};
