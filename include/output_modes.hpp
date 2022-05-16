#pragma once
#include "controller_state.hpp"
#include "hardware/gpio.h"
#include "pico/stdio.h"
#include "pico/stdlib.h"

void send(uint32_t *state);
void extended(ControllerState *state);
