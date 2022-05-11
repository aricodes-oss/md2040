#pragma once
#include "pico/stdio.h"
#include "pico/stdlib.h"

enum Direction { UP, DOWN, LEFT, RIGHT, NONE };
enum Button { A, B, C, START, X, Y, Z };

class ControllerState {
public:
  bool a;
  bool b;
  bool c;

  bool start;

  bool x;
  bool y;
  bool z;

  bool up;
  bool down;
  bool left;
  bool right;

  void press_dir(Direction dir);
  void release_dir(Direction dir);

private:
  bool up_pressed;
  bool down_pressed;
  bool left_pressed;
  bool right_pressed;

  Direction first_vertical;
  Direction first_horizontal;

  void update_pad();
};
