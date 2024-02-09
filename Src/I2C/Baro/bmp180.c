#include "bmp180.h"


typedef struct{
    int16_t AC1;
    int16_t AC2;
    int16_t AC3;

    uint16_t AC4;
    uint16_t AC5;
    uint16_t AC6;

    int16_t B1;
    int16_t B2;
    int16_t MB;
    int16_t MC;
    int16_t MD;

} BMP180_calibration_t;
const uint8_t readReg = 0xF6;


BMP180_calibration_t BMP180_calibration;

void BMP180_Init(){
    union {
        BMP180_calibration_t calibration;
        uint16_t table[sizeof(BMP180_calibration_t)/2];
    } bmp180;
    uint8_t table[2];

    const uint8_t startReg = 0xAA;

    I2C_write(BMP180_ADDRESS, &startReg);

    // i2c_read_blocking(I2C_CHANNEL, BMP180_ADDRESS, bmp180.table, sizeof(BMP180_calibration_t), false);
    for (uint i = 0; i < sizeof(BMP180_calibration_t)/sizeof(table); i++){
        i2c_read_blocking(I2C_CHANNEL, BMP180_ADDRESS, table, 2, false);
        bmp180.table[i] = (table[0] << 8) | table[1];
    }

    BMP180_calibration = bmp180.calibration;
}

uint16_t BMP180_getRawTemperature(){
    uint16_t temp;
    int8_t tab[sizeof(int16_t)];


    I2C_writeReg(BMP180_ADDRESS, 0xF4, 0x2E);
    sleep_ms(4.5);
    i2c_write_blocking(I2C_CHANNEL, BMP180_ADDRESS, &readReg, 1, false);
    i2c_read_blocking(I2C_CHANNEL, BMP180_ADDRESS, tab, 2, false);
    temp = (tab[0] << 8) | tab[1];
    return temp;
}

uint BMP180_getRawPressure(uint8_t mode){
    uint pressure;
    uint8_t tab[3];

    I2C_writeReg(BMP180_ADDRESS, 0xF4, 0x34+(mode<<6));
    switch (mode){
        case 0: sleep_ms( 4.5); break;
        case 1: sleep_ms( 7.5); break;
        case 2: sleep_ms(13.5); break;
        case 3: sleep_ms(25.5); break;
    }
    i2c_write_blocking(I2C_CHANNEL, BMP180_ADDRESS, &readReg, 1, false);
    i2c_read_blocking(I2C_CHANNEL, BMP180_ADDRESS, tab, 3, false);

    pressure = ((tab[0] << 16) | (tab[1] << 8) | tab[2]) >> (8 - mode);
    return pressure;
}


float BMP180_getTemperature(){
    int rawTemp = BMP180_getRawTemperature();

    int X1 = (rawTemp - BMP180_calibration.AC6) * BMP180_calibration.AC5 / 32768;
    int X2 = (BMP180_calibration.MC * 2048) / (X1 + BMP180_calibration.MD);
    int temp = (X1 + X2 + 8) / 16;
    if (temp < -400 || temp > 850)
        return 0;
    return temp/10.0f;
}

uint BMP180_getPressure(uint8_t mode){
    int rawTemp = BMP180_getRawTemperature();
    int rawPressure = BMP180_getRawPressure(mode);
    uint pressure;

    int X1 = (rawTemp - BMP180_calibration.AC6) * BMP180_calibration.AC5 / 32768;
    int X2 = (BMP180_calibration.MC * 2048) / (X1 + BMP180_calibration.MD);
    int B5 = X1 + X2;
    int temp = (B5 + 8) / 16;
    if (temp < -400 || temp > 850)
        return -1;

    int B6 = B5 - 4000;
    X1 = (BMP180_calibration.B2 * ((B6*B6) >> 12)) / 2048;
    X2 = (BMP180_calibration.AC2 * B6) / 2048;
    int X3 = X1 + X2;
    int B3 = (((BMP180_calibration.AC1*4+X3) << mode) + 2) / 4;
    X1 = (BMP180_calibration.AC3 * B6) / 8192;
    X2 = BMP180_calibration.B1 * (B6*B6 / 4096) / 65536;
    X3 = (X1 + X2 + 2) / 4;
    int B4 = BMP180_calibration.AC4 * (unsigned int)(X3 + 32768) / 32768;
    int B7 = ((unsigned int)(rawPressure)-B3) * (50000 >> mode);

    pressure = (B7/B4) * 2;
    X1 = pressure*pressure / 512;
    X1 = (X1 * 3038) / 65536;
    X2 = (-7357 * pressure) / 65536;
    pressure = pressure + (X1 + X2 + 3791) / 16;

    return pressure;
}
