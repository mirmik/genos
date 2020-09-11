#ifndef LIS331DLH_H
#define LIS331DLH_H

// Source: Amperka troyka library

#include "drivers/i2c/linux_i2c.h"

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
};

enum LIS331DLH_Range : uint8_t
{
    RANGE_2G = 2,
    RANGE_4G = 4,
    RANGE_8G = 8
};

class LIS331DLH_I2C : public genos::i2c_client
{
    genos::i2c_master * i2c;

    uint8_t ctrl_reg1_mirror = 0;
    uint8_t ctrl_reg4_mirror = 0;

    uint8_t _addr;
    float _mult;

    float x_mul=1;
    float y_mul=1;
    float z_mul=1;

    float x_bias=0;
    float y_bias=0;
    float z_bias=0;

public:
    LIS331DLH_I2C(genos::i2c_master * i2c, uint8_t addr = LIS331DLH_I2C_ADDRESS);

    void begin();

    void sleep(bool enable);
    void setRange(uint8_t range);

    float readGX();
    float readGY();
    float readGZ();

    float readAX();
    float readAY();
    float readAZ();

    float readGX_calib();
    float readGY_calib();
    float readGZ_calib();

    float readAX_calib();
    float readAY_calib();
    float readAZ_calib();

    void readGXYZ(float *ax, float *ay, float *az);
    void readAXYZ(float *gx, float *gy, float *gz);
    void readXYZ(int16_t *x, int16_t *y, int16_t *z);

    int16_t readAxis(uint8_t reg);
    uint8_t readByte(uint8_t reg);
    int16_t readX();
    int16_t readY();
    int16_t readZ();

	void set_calibration_x(float a, float b) { x_bias = (a+b)/2; x_mul = 1/((b-a)/2); }
	void set_calibration_y(float a, float b) { y_bias = (a+b)/2; y_mul = 1/((b-a)/2); }
        void set_calibration_z(float a, float b) { z_bias = (a+b)/2; z_mul = 1/((b-a)/2); }
};

#endif
