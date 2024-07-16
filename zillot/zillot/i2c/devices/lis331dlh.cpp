//#include <Wire.h>
#include "drivers/i2c/devices/lis331dlh.h"

#define ADR_FS_2        0x00
#define ADR_FS_4        0x10
#define ADR_FS_8        0x30

#define G               9.81

lis331dlh_device::lis331dlh_device(genos::i2c_client* i2c)
    : client(i2c) {}

void lis331dlh_device::begin()
{
    ctrl_reg1_mirror |= (1 << 0);
    ctrl_reg1_mirror |= (1 << 1);
    ctrl_reg1_mirror |= (1 << 2);

    ctrl_reg1_mirror |= (1 << 5);

    setRange(RANGE_2G);

    char buf[2];
    buf[0] = CTRL_REG1;
    buf[1] = ctrl_reg1_mirror;

    //i2c->set_slave_address(_addr);
    dprln("Here1");
    client->write(buf, 2);
    dprln("Here2");
}

void lis331dlh_device::setRange(uint8_t range)
{
    switch (range)
    {
    case RANGE_2G:
    {
        ctrl_reg4_mirror = ADR_FS_2;
        _mult = RANGE_2G / 32767.0;
        break;
    }
    case RANGE_4G:
    {
        ctrl_reg4_mirror = ADR_FS_4;
        _mult = RANGE_4G / 32767.0;
        break;
    }
    case RANGE_8G:
    {
        ctrl_reg4_mirror = ADR_FS_8;
        _mult = RANGE_8G / 32767.0;
        break;
    }
    default:
    {
        _mult = RANGE_2G / 32767.0;
    }
    break;
    }

    char buf[2];
    buf[0] = CTRL_REG4;
    buf[1] = ctrl_reg4_mirror;
    //i2c->set_slave_address(_addr);
    client->write(buf, 2);
}

void lis331dlh_device::sleep(bool enable)
{
    if (enable)
        ctrl_reg1_mirror &= ~(1 << 5);
    else
        ctrl_reg1_mirror |= (1 << 5);

    char buf[2];
    buf[0] = CTRL_REG1;
    buf[1] = ctrl_reg1_mirror;

    //client->set_slave_address(_addr);
    client->write(buf, 2);
}

float lis331dlh_device::readGX()
{
    return readX() * _mult;
}

float lis331dlh_device::readGY()
{
    return readY() * _mult;
}

float lis331dlh_device::readGZ()
{
    return readZ() * _mult;
}

float lis331dlh_device::readAX()
{
    return readGX() * G;
}

float lis331dlh_device::readAY()
{
    return readGY() * G;
}

float lis331dlh_device::readAZ()
{
    return readGZ() * G;
}

float lis331dlh_device::readGX_calib()
{
    return readX() * _mult * x_mul - x_bias;
}

float lis331dlh_device::readGY_calib()
{
    return readY() * _mult * y_mul - y_bias;
}

float lis331dlh_device::readGZ_calib()
{
    return readZ() * _mult * z_mul - z_bias;
}

float lis331dlh_device::readAX_calib()
{
    return readGX_calib() * G;
}

float lis331dlh_device::readAY_calib()
{
    return readGY_calib() * G;
}

float lis331dlh_device::readAZ_calib()
{
    return readGZ_calib() * G;
}

void lis331dlh_device::readGXYZ(float *gx, float *gy, float *gz)
{
    int16_t x, y, z;
    readXYZ(&x, &y, &z);
    *gx = x * _mult;
    *gy = y * _mult;
    *gz = z * _mult;
}

void lis331dlh_device::readAXYZ(float *ax, float *ay, float *az)
{
    readGXYZ(ax, ay, az);
    (*ax) *= G;
    (*ay) *= G;
    (*az) *= G;
}

int16_t lis331dlh_device::readX()
{
    return readAxis(OUT_X);
}

int16_t lis331dlh_device::readY()
{
    return readAxis(OUT_Y);
}

int16_t lis331dlh_device::readZ()
{
    return readAxis(OUT_Z);
}

int16_t lis331dlh_device::readAxis(uint8_t reg)
{
    return ((int16_t)readByte(reg + 1) << 8) | readByte(reg);
}

uint8_t lis331dlh_device::readByte(uint8_t reg)
{
    char buf[1];
    char c;

    buf[0] = reg;

    //i2c->set_slave_address(_addr);

    client->write(buf, 1);
    client->read(&c, 1);

    return c;
}


void lis331dlh_device::readXYZ(int16_t *x, int16_t *y, int16_t *z)
{
    char buf[1];
    buf[0] = OUT_X | (1 << 7);

    //i2c->set_slave_address(_addr);
    client->write(buf, 1);

    uint8_t burstSize = 6;
    uint8_t values[burstSize];
    client->read(values, 6);

    *x = *((int16_t*)&values[0]);
    *y = *((int16_t*)&values[2]);
    *z = *((int16_t*)&values[4]);
}
