#include "controller_state.hpp"
#include "pico/stdio.h"
#include "pico/stdlib.h"
#include "pins.hpp"

void ControllerState::press_dir(Direction dir) {
  switch (dir) {
  case UP:
    up_pressed = true;
    break;
  case DOWN:
    down_pressed = true;
    break;
  case LEFT:
    left_pressed = true;
    break;
  case RIGHT:
    right_pressed = true;
    break;
  }

  if (dir == UP || dir == DOWN) {
    if (first_vertical == NONE) {
      first_vertical = dir;
    }
  }

  if (dir == LEFT || dir == RIGHT) {
    if (first_horizontal == NONE) {
      first_horizontal = dir;
    }
  }

  update_pad();
}

void ControllerState::release_dir(Direction dir) {
  switch (dir) {
  case UP:
    up_pressed = false;
    break;
  case DOWN:
    down_pressed = false;
    break;
  case LEFT:
    left_pressed = false;
    break;
  case RIGHT:
    right_pressed = false;
    break;
  }

  // Adjusting first selection if user goes DOWN->SOCD->UP
  // so that down will count as the second input again.
  // There's probably a cleaner way to do this XOR pattern
  // but I find that C gets harder to read the more
  // "expressive" you get with it.
  if (up_pressed && !down_pressed) {
    first_vertical = UP;
  } else if (!up_pressed && down_pressed) {
    first_vertical = DOWN;
  }

  if (left_pressed && !right_pressed) {
    first_horizontal = LEFT;
  } else if (!left_pressed && right_pressed) {
    first_horizontal = RIGHT;
  }

  // If no directions are pressed, clear all 2IP state
  if (!up_pressed && !down_pressed) {
    first_vertical = NONE;
  }

  if (!left_pressed && !right_pressed) {
    first_horizontal = NONE;
  }

  update_pad();
}

void ControllerState::update_pad() {
  up = up_pressed;
  down = down_pressed;
  left = left_pressed;
  right = right_pressed;

  if (up_pressed && down_pressed) {
    up = first_vertical == DOWN;
    down = first_vertical == UP;
  }

  if (left_pressed && right_pressed) {
    left = first_horizontal == RIGHT;
    right = first_horizontal == LEFT;
  }
}

void ControllerState::update_high_state(uint32_t *state) {
  set_bit(state, D0_PIN, up);
  set_bit(state, D1_PIN, down);
  set_bit(state, D2_PIN, left);
  set_bit(state, D3_PIN, right);
  set_bit(state, D4_PIN, b);
  set_bit(state, D5_PIN, c);
}

void ControllerState::update_low_state(uint32_t *state) {
  set_bit(state, D0_PIN, up);
  set_bit(state, D1_PIN, down);
  set_bit(state, D2_PIN, true);
  set_bit(state, D3_PIN, true);
  set_bit(state, D4_PIN, a);
  set_bit(state, D5_PIN, start);
}
