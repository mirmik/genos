#ifndef GENOS_DRIVERS_SPI_H
#define GENOS_DRIVERS_SPI_H

#include <sys/cdefs.h>
#include <sched/flags.h>

struct spi_operations;

enum spi_mode_e {
	SPI_MODE_0,
	SPI_MODE_1,
	SPI_MODE_2,
	SPI_MODE_3
};

struct spi_device {
    const struct spi_operations * spi_op;
};

struct spi_operations {
	int (*select)   (struct spi_device *spi, void *slct, int en);
	int (*exchange) (struct spi_device *spi, const void *txbuf, void *rxbuf, int len, int flags);
};

__BEGIN_DECLS

static inline void spi_device_init(struct spi_device * spi, const struct spi_operations * ops) {
	spi->spi_op = ops;
}

static inline int spi_select(struct spi_device * spi, void* slct, int en) { 
	return spi->spi_op->select(spi, slct, en); 
}

static inline int spi_exchange(struct spi_device *spi, const void *txbuf, void *rxbuf, int len, int flags) { 
	return spi->spi_op->exchange(spi, txbuf, rxbuf, len, flags); 
}

__END_DECLS

#endif