#include <zillot/i2c/i2c_device.h>

int i2c_writeread(
    struct i2c_bus_device * dev,
    uint8_t devaddr,
    const void * wbuf, size_t wlen,
    void * rbuf, size_t rlen) 
{
    return dev->ops->writeread(dev, devaddr, wbuf, wlen, rbuf, rlen);	
}