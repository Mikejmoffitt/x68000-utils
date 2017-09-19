#ifndef JOY_H
#define JOY_H

#include <stdint.h>

#define KEY_UP 1
#define KEY_DOWN 2
#define KEY_LEFT 4
#define KEY_RIGHT 8
#define KEY_A 16
#define KEY_B 32

const volatile uint8_t *joy1 = (volatile uint8_t *)0xE9A001;
const volatile uint8_t *joy2 = (volatile uint8_t *)0xE9A003;

#endif // JOY_H
