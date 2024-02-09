#ifndef __DHT11__H
#define __DHT11__H

#include <stdio.h>
#include <stdlib.h>
#include <pico/stdlib.h>
#include <hardware/gpio.h>

#define DHT_PIN 18

typedef struct{
    int temperature;
    int humidity;
} dht_result;

void sayMiau();
int DHT_read(dht_result *result);

#endif