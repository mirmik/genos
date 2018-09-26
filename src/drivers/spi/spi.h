#ifndef GENOS_DRIVERS_SPI_H
#define GENOS_DRIVERS_SPI_H

#include <sys/cdefs.h>

struct spi_operations;

enum spi_mode_e {
	SPI_MODE_0,
	SPI_MODE_1,
	SPI_MODE_2,
	SPI_MODE_3
};

struct spi_device {
    const struct spi_operations * spi_op;
    void * current_sellected;

    uint8_t mode;
};

struct spi_operations {
	void (*select)   (struct spi_device *dev, void *slct, int en);
	void (*exchange) (struct spi_device *dev, const void *txbuf, void *rxbuf, size_t len);
};

__BEGIN_DECLS

__END_DECLS

#endif