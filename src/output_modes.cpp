#include "output_modes.hpp"
#include "controller_state.hpp"
#include "hardware/gpio.h"
#include "pico/stdio.h"
#include "pico/stdlib.h"
#include "pins.hpp"

uint32_t output_mask = 0;

void _initialize_output_mask() {
  for (auto pin : OUTPUT_PINS) {
    output_mask |= (1UL << pin);
  }
}

uint32_t value_mask = 0;

void send(uint32_t *state) {
  if (output_mask == 0) {
    _initialize_output_mask();
  }

  gpio_put_masked(output_mask, *state);
}

void extended(ControllerState *state) {
  if (output_mask == 0) {
    _initialize_output_mask();
  }

  value_mask = 0;
  value_mask = (value_mask << D0_PIN) | (*state).z;
  value_mask = (value_mask << D1_PIN) | (*state).y;
  value_mask = (value_mask << D2_PIN) | (*state).x;
  value_mask = (value_mask << D3_PIN) | 0;
  value_mask = (value_mask << D4_PIN) | 0;
  value_mask = (value_mask << D5_PIN) | 0;
  gpio_put_masked(output_mask, value_mask);

  gpio_put(LED_PIN, true);
}
