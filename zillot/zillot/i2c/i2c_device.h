#ifndef ZILLOT_I2C_i2c_bus_device_H
#define ZILLOT_I2C_i2c_bus_device_H

#include <igris/compiler.h>
#include <igris/sync/semaphore.h>
#include <stdint.h>

struct i2c_bus_device;

struct i2c_bus_device_operations
{
    int (*writeread)(struct i2c_bus_device *,
                     uint8_t devaddr,
                     const void *wbuf,
                     size_t wlen,
                     void *rbuf,
                     size_t rlen);

    int (*write)(struct i2c_bus_device *,
                 uint8_t devaddr,
                 const void *buffer,
                 size_t buflen);

    int (*read)(struct i2c_bus_device *,
                uint8_t devaddr,
                const void *buffer,
                size_t buflen);

    int (*write_memory)(struct i2c_bus_device *,
                        uint8_t devaddr,
                        uint8_t memaddr,
                        const void *buffer,
                        size_t buflen);
};

struct i2c_bus_device
{
    const struct i2c_bus_device_operations *ops;
    igris_sem_t sem;
};

__BEGIN_DECLS

int i2c_writeread(struct i2c_bus_device *,
                  uint8_t devaddr,
                  const void *wbuf,
                  size_t wlen,
                  void *rbuf,
                  size_t rlen);

void i2c_lock_bus(struct i2c_bus_device *);
void i2c_unlock_bus(struct i2c_bus_device *);

__END_DECLS

#define I2C_INIT(name, ops)                                                    \
    {                                                                          \
        ops, SEMAPHORE_INIT(name.sem, 1)                                       \
    }

#endif