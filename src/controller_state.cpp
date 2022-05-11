#include "controller_state.hpp"
#include "pico/stdio.h"
#include "pico/stdlib.h"

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

  if (!up_pressed && !down_pressed) {
    first_vertical = NONE;
  }

  if (!left_pressed && !right_pressed) {
    first_vertical = NONE;
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
