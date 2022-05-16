#pragma once
#include "pico/stdio.h"
#include "pico/stdlib.h"

#define A_PIN 0
#define B_PIN 1
#define C_PIN 2

#define START_PIN 3

#define UP_PIN 4
#define RIGHT_PIN 5
#define DOWN_PIN 6
#define LEFT_PIN 7

#define X_PIN 8
#define Y_PIN 9
#define Z_PIN 10

#define SEL_PIN 16

#define D0_PIN 17
#define D1_PIN 18
#define D2_PIN 19
#define D3_PIN 20
#define D4_PIN 21
#define D5_PIN 22

#define LED_PIN 25

extern uint8_t INPUT_PINS[11];

extern uint8_t OUTPUT_PINS[6];

void set_bit(uint32_t *mask, uint pin, bool status);
