#ifndef GENOS_DRIVERS_SPI_H
#define GENOS_DRIVERS_SPI_H

#include <sys/cdefs.h>

struct spi_operations;

struct spi_device {
    const struct spi_operations * spi_op;
};

struct spi_operations {
	void (*select)   (struct spi_device *dev, void *slct, int en);
	void (*exchange) (struct spi_device *dev, const void *txbuf, void *rxbuf, size_t len);
};

__BEGIN_DECLS

__END_DECLS

#endif