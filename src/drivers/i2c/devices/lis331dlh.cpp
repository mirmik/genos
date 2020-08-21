#include <Wire.h>
#include "lis331dlh.h"

#define ADR_FS_2        0x00
#define ADR_FS_4        0x10
#define ADR_FS_8        0x30

#define G               9.8

LIS331DLH_I2C::LIS331DLH_I2C(uint8_t addr) : AxisHw(addr) {
}

void LIS331DLH_I2C::begin() {
    ctrl_reg1_mirror |= (1 << 0);
    ctrl_reg1_mirror |= (1 << 1);
    ctrl_reg1_mirror |= (1 << 2);
    
    ctrl_reg1_mirror |= (1 << 5);
    
    setRange(RANGE_2G);
    
    char buf[2];
    buf[0] = CTRL_REG1;
    buf[1] = ctrl_reg1_mirror;
    i2c->write(buf, 2);
}

void LIS331DLH_I2C::setRange(uint8_t range) {
    switch (range) {
        case RANGE_2G: {
            ctrl_reg4_mirror = ADR_FS_2;
            _mult = RANGE_2G / 32767.0;
            break;
        }
        case RANGE_4G: {
            ctrl_reg4_mirror = ADR_FS_4;
            _mult = RANGE_4G / 32767.0;
            break;
        }
        case RANGE_8G: {
            ctrl_reg4_mirror = ADR_FS_8;
            _mult = RANGE_8G / 32767.0;
            break;
        }
        default: {
            _mult = RANGE_2G / 32767.0;    
        }
        break;
    }

    char buf[2];
    buf[0] = CTRL_REG4;
    buf[1] = ctrl_reg4_mirror;
    i2c->write(buf, 2);
}

void LIS331DLH_I2C::sleep(bool enable) {
    if (enable)
        ctrl_reg1_mirror &= ~(1 << 5);
    else
        ctrl_reg1_mirror |= (1 << 5);

    writeCtrlReg1();
}

float LIS331DLH_I2C::readGX() {
    return readX()*_mult;
}

float LIS331DLH_I2C::readGY() {
    return readY()*_mult;
}

float LIS331DLH_I2C::readGZ() {
    return readZ()*_mult;
}

float LIS331DLH_I2C::readAX() {
    return readGX() * G;
}

float LIS331DLH_I2C::readAY() {
    return readGY() * G;
}

float LIS331DLH_I2C::readAZ() {
    return readGZ() * G;
}

void LIS331DLH_I2C::readGXYZ(float *gx, float *gy, float *gz) {
    int16_t x, y, z;
    readXYZ(&x, &y, &z);
    *gx = x * _mult;
    *gy = y * _mult;
    *gz = z * _mult;
}

void LIS331DLH_I2C::readAXYZ(float *ax, float *ay, float *az) {
    readGXYZ(ax, ay, az);
    (*ax) *= G;
    (*ay) *= G;
    (*az) *= G;
}
