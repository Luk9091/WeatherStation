#ifndef __BMP180__H
#define __BMP180__H

#include <pico/stdio.h>
#include <pico/stdlib.h>
#include <hardware/gpio.h>
#include <hardware/i2c.h>

#include "../i2c.h"
#define BMP180_ADDRESS 0x77

void BMP180_Init();
float BMP180_getTemperature();

// Get pressure mode:
// mode 0: 1 time sampling
// mode 1: 2 time sampling
// mode 2: 3 time sampling
// mode 3: 3 time sampling
uint BMP180_getPressure(uint8_t mode);


#endif