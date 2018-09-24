#include <drivers/spi/avr_spi.h>
#include <sched/wait.h>

struct avr_spi_device {
	struct spi_device spi;
	int32_t frequency;

	struct dlist_head wait_list; 

	const void *txbuf;
	void *rxbuf;
	size_t len;
	size_t counter;

	uint8_t in_work;
};

void avr_spi_select (struct spi_device *dev, void *slct, int en) {

}

void avr_spi_exchange (struct spi_device *dev, const void *txbuf, void *rxbuf, size_t len) {
	int sts;
	struct avr_spi_device * spi = mcast_out(dev, struct avr_spi_device, spi);

	while (spi->in_work) {
		sts = wait_current_schedee(&spi->wait_list, false);
		if (sts == -1) 
			return EAGAIN;
	}



	//wait_current_schedee();
}


void avr_spi_irq_handler() {

}

void link_avr_spi() {

}