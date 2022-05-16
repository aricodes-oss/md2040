#include "controller_state.hpp"
#include "hardware/gpio.h"
#include "output_modes.hpp"
#include "pico/binary_info.h"
#include "pico/binary_info/code.h"
#include "pico/multicore.h"
#include "pico/stdio.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include "pico/types.h"
#include "pins.hpp"
#include <cstdint>
#include <stdio.h>

uint8_t hf_cycles = 0;
absolute_time_t current_time = 0;
absolute_time_t previous_low_pulse = 0;

uint32_t low_state = 0;
uint32_t high_state = 0;

bool is_low = false;

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
  }

  if (!left_pressed && left_was_pressed) {
    controller.release_dir(LEFT);
  }

  if (right_pressed && !right_was_pressed) {
    controller.press_dir(RIGHT);
  }

  if (!right_pressed && right_was_pressed) {
    controller.release_dir(RIGHT);
  }
}

void sel_irq_handler(uint gpio, uint32_t events) {
  // For some reason the pin argument is ignored in
  // gpio_set_irq_enabled_with_callback, so we add our own logic here
  if (gpio != SEL_PIN) {
    gpio_acknowledge_irq(gpio, events);
    return;
  }

  if (events & GPIO_IRQ_EDGE_FALL || events & GPIO_IRQ_LEVEL_LOW) {
    send(&low_state);
    is_low = true;
  }

  if (events & GPIO_IRQ_EDGE_RISE || events & GPIO_IRQ_LEVEL_HIGH) {
    send(&high_state);
    is_low = false;
  }
  gpio_acknowledge_irq(gpio, events);
}

void core1() {
  uint32_t irq_events = GPIO_IRQ_LEVEL_LOW | GPIO_IRQ_LEVEL_HIGH;
  gpio_set_irq_enabled_with_callback(SEL_PIN, irq_events, true,
                                     &sel_irq_handler);
}

int main() {
  bi_decl(bi_program_description(
      "Turns a Pico into a 6-button Megadrive/Genesis controller"));
  stdio_init_all();
  printf("Initializing...\n");
  sleep_ms(250);
  multicore_launch_core1(core1);

  uint32_t prev_pins = 0xFFFF;
  uint32_t current_pins = 0xFFFF;

  // SEL pin is our only logic-shifted input
  gpio_init(SEL_PIN);
  gpio_set_dir(SEL_PIN, GPIO_IN);
  gpio_pull_up(SEL_PIN);

  // Marginally lower latency for inputs
  gpio_set_input_hysteresis_enabled(SEL_PIN, false);

  // LED for status, should just mirror the SEL line
  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);
  gpio_put(LED_PIN, false);

  for (auto in_pin : INPUT_PINS) {
    gpio_init(in_pin);
    gpio_set_dir(in_pin, GPIO_IN);
    gpio_pull_up(in_pin);
    gpio_set_input_hysteresis_enabled(in_pin, false);
  }

  for (auto out_pin : OUTPUT_PINS) {
    gpio_init(out_pin);
    gpio_set_dir(out_pin, GPIO_OUT);
    gpio_put(out_pin, true);
    gpio_set_slew_rate(out_pin, GPIO_SLEW_RATE_FAST);
  }

  while (true) {
    // Snapshot our GPIO state so that we can carve out the ones we want
    // with a bitmask, should be faster than retrieving them one-by-one
    current_pins = gpio_get_all();
    update_state(prev_pins, current_pins);

    controller.update_high_state(&high_state);
    controller.update_low_state(&low_state);

    // send(is_low ? &low_state : &high_state);

    prev_pins = current_pins;
  }

  return 0;
}
