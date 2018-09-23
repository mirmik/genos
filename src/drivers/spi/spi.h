#ifndef GENOS_DRIVERS_SPI_H
#define GENOS_DRIVERS_SPI_H

struct spi_device {
    uint8_t chip_s



    .chip_select    = 0,
    .max_speed_hz   = 1 * 1000 * 1000,
    .bus_num        = 1,
    .mode           = SPI_MODE_3,
};

#endif