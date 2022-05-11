#include "controller_state.hpp"
#include "hardware/gpio.h"
#include "output_modes.hpp"
#include "pico/binary_info.h"
#include "pico/binary_info/code.h"
#include "pico/stdio.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include "pins.hpp"
#include <stdio.h>

ControllerState controller;

bool get_pin(uint32_t pin_state, int num) {
  return (1 & (pin_state >> num)) == 0;
}

void update_state(uint32_t previous, uint32_t current) {
  // Go through the buttons first since those aren't mutually exclusive
  controller.a = get_pin(current, A_PIN);
  controller.b = get_pin(current, B_PIN);
  controller.c = get_pin(current, C_PIN);

  controller.x = get_pin(current, X_PIN);
  controller.y = get_pin(current, Y_PIN);
  controller.z = get_pin(current, Z_PIN);

  controller.start = get_pin(current, START_PIN);

  // After that, we compare directional deltas
  bool up_pressed = get_pin(current, UP_PIN);
  bool down_pressed = get_pin(current, DOWN_PIN);
  bool left_pressed = get_pin(current, LEFT_PIN);
  bool right_pressed = get_pin(current, RIGHT_PIN);

  bool up_was_pressed = get_pin(previous, UP_PIN);
  bool down_was_pressed = get_pin(previous, DOWN_PIN);
  bool left_was_pressed = get_pin(previous, LEFT_PIN);
  bool right_was_pressed = get_pin(previous, RIGHT_PIN);

  if (up_pressed && !up_was_pressed) {
    controller.press_dir(UP);
  }

  if (!up_pressed && up_was_pressed) {
    controller.release_dir(UP);
  }

  if (down_pressed && !down_was_pressed) {
    controller.press_dir(DOWN);
  }

  if (!down_pressed && down_was_pressed) {
    controller.release_dir(DOWN);
  }

  if (left_pressed && !left_was_pressed) {
    controller.press_dir(LEFT);
  } else if (!left_pressed && left_was_pressed) {
    controller.release_dir(LEFT);
  }

  if (right_pressed && !right_was_pressed) {
    controller.press_dir(RIGHT);
  } else if (!right_pressed && right_was_pressed) {
    controller.release_dir(RIGHT);
  }
}

int main() {
  bi_decl(bi_program_description(
      "Turns a Pico into a 6-button Megadrive/Genesis controller"));
  stdio_init_all();
  printf("Initializing...\n");

  uint32_t prev_pins = 0xFFFF;
  uint32_t current_pins = 0xFFFF;

  gpio_init(SEL_PIN);
  gpio_set_dir(SEL_PIN, GPIO_IN);
  gpio_pull_up(SEL_PIN);
  gpio_put(SEL_PIN, false);

  // LED
  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);
  gpio_pull_down(LED_PIN);
  gpio_put(LED_PIN, false);

  for (auto in_pin : INPUT_PINS) {
    gpio_init(in_pin);
    gpio_set_dir(in_pin, GPIO_IN);
    gpio_pull_up(in_pin);
  }

  for (auto out_pin : OUTPUT_PINS) {
    gpio_init(out_pin);
    gpio_set_dir(out_pin, GPIO_OUT);
    gpio_pull_down(out_pin);
  }

  while (true) {
    current_pins = gpio_get_all();
    update_state(prev_pins, current_pins);

    if (get_pin(current_pins, SEL_PIN)) {
      gpio_put(LED_PIN, true);
      normal(&controller);
    } else {
      gpio_put(LED_PIN, false);
      normal(&controller);
    }

    prev_pins = current_pins;
  }

  return 0;
}
