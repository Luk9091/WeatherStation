#include <stdio.h>
#include <stdlib.h>
#include <pico/stdio.h>
#include <pico/cyw43_arch.h>
#include <hardware/gpio.h>

#include "DHT/dht.h"
#include "I2C/i2c.h"
#include "I2C/Baro/bmp180.h"

int main(){
    stdio_init_all();
    I2C_Init();
    // I2C_scan();
    if(cyw43_arch_init()){
        printf("Cannot run WiFi module\n");
        return -1;
    }
    
    BMP180_Init();

    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
    gpio_init(DHT_PIN);
    dht_result TH;
    
    while(1){
        if (!DHT_read(&TH)) {
            printf("Temperature: %-2iC\n", TH.temperature);
            printf("Humidity: %-2i%%\n", TH.humidity);
        }
        printf("Better temp: %-2.1f\n", BMP180_getTemperature());
        printf("Pressure: %-2i\n", BMP180_getPressure(0));
        sleep_ms(500);
    }

    return 0;
}
