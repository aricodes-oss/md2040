#include "output_modes.hpp"
#include "controller_state.hpp"
#include "hardware/gpio.h"
#include "pico/stdio.h"
#include "pico/stdlib.h"
#include "pins.hpp"

void _set(uint pin, bool status) { gpio_put(pin, !status); }

void normal(ControllerState *state) {
  _set(D0_PIN, (*state).up);
  _set(D1_PIN, (*state).down);
  _set(D2_PIN, (*state).left);
  _set(D3_PIN, (*state).right);
  _set(D4_PIN, (*state).b);
  _set(D5_PIN, (*state).c);
}

void third(ControllerState *state) {
  _set(D0_PIN, false);
  _set(D1_PIN, false);
  _set(D2_PIN, true);
  _set(D3_PIN, true);
  _set(D4_PIN, (*state).a);
  _set(D5_PIN, (*state).start);
}

void low(ControllerState *state) {
  _set(D0_PIN, (*state).up);
  _set(D1_PIN, (*state).down);
  _set(D2_PIN, true);
  _set(D3_PIN, true);
  _set(D4_PIN, (*state).a);
  _set(D5_PIN, (*state).start);
}

void extended(ControllerState *state) {
  _set(D0_PIN, (*state).z);
  _set(D1_PIN, (*state).y);
  _set(D2_PIN, (*state).x);
  _set(D3_PIN, false);
  _set(D4_PIN, false);
  _set(D5_PIN, false);
}
