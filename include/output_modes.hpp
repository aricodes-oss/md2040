#pragma once
#include "controller_state.hpp"
#include "hardware/gpio.h"
#include "pico/stdio.h"
#include "pico/stdlib.h"

void normal(ControllerState *state);
void third(ControllerState *state);
void low(ControllerState *state);
void extended(ControllerState *state);
