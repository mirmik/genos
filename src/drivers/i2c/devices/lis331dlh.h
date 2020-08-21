#ifndef LIS331DLH_H
#define LIS331DLH_H

// Source: Amperka troyka library

#include "genos/drivers/i2c/sysfs_i2c.h"

#define LIS331DLH_I2C_ADDRESS     0b0011000
#define LIS331DLH_I2C_ADDRESS_V2  0b0011001

enum LIS331DLH_Register
{
    CTRL_REG1 = 0x20,
    CTRL_REG2 = 0x21,
    CTRL_REG3 = 0x22,
    CTRL_REG4 = 0x23,
    CTRL_REG5 = 0x24,
    OUT_X = 0x28,
    OUT_Y = 0x2A,
    OUT_Z = 0x2C
}

enum LIS331DLH_Range : uint8_t
{
    RANGE_2G = 2,
    RANGE_4G = 4,
    RANGE_8G = 8
};

class LIS331DLH_I2C
{
    i2c_master * i2c;

    uint8_t ctrl_reg1_mirror = 0;
    uint8_t ctrl_reg4_mirror = 0;
    
    float _mult;

public:
    LIS331DLH_I2C(genos::i2c_master * i2c, uint8_t addr = LIS331DLH_I2C_ADDRESS);

    void begin();

    void sleep(bool enable);
    void setRange(uint8_t range);

    // считать значение в G
    float readGX();
    float readGY();
    float readGZ();

    // считать значение в единицах си
    float readAX();
    float readAY();
    float readAZ();

    // считать сразу несколько значений
    void readGXYZ(float *ax, float *ay, float *az);
    void readAXYZ(float *gx, float *gy, float *gz);
};

#endif
