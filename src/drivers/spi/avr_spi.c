#include <drivers/spi/avr_spi.h>
#include <sched/wait.h>

#include <drivers/gpio/pin.h>

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

static inline void avr_spi_sendbyte(uint8_t tx) { SDPR = tx; }
static inline void avr_spi_txidlewait() { while(!(SPSR & 0x80)); }
static inline uint8_t avr_spi_recvbyte() { uint8_t rx = SPDR; return rx; }

void avr_spi_select (struct spi_device *dev, void *slct, int en) {
	struct avr_spi_device * avr_spi = mcast_out(struct avr_spi_device, dev, spi);
	struct gpio_pin * npin = (struct gpio_pin *) slct;
	struct gpio_pin * opin = (struct gpio_pin *) dev->current_select;

	if (en == false) {
		assert(npin == opin);
		gpio_pin_clr(opin);
		dev->current_select = NULL;
		return;	
	}

	if (opin != NULL) {
		gpio_pin_clr(opin);
	}
	gpio_pin_set(npin);
	
	dev->current_select = slct;
	return;
}

void avr_spi_exchange (struct spi_device *dev, const void *txbuf, void *rxbuf, size_t len) {
	int sts;
	struct avr_spi_device * spi = mcast_out(dev, struct avr_spi_device, spi);

	while (spi->in_work) {
		sts = wait_current_schedee(&spi->wait_list, false);
		if (sts == -1) {
			return EAGAIN;
		}
	}

	dev->txbuf = txbuf;
	dev->rxbuf = rxbuf;
	dev->counter = 0;
	dev->len = len;

	avr_spi_sendbyte(txbuf[0]);
	avr_spi_enable_irq(dev);

	//Ставим поток в начало очереди, чтобы обработчик прерывания снял его оттуда позже.
	wait_current_schedee(&spi->wait_list, true);

	//Отдаем управление следующему процессу, если такой находится.
	spi->in_work = 0;
	unwait_one(&spi->wait_list);
}


void avr_spi_irq_handler(struct spi_device * dev) {
	struct avr_spi_device * spi = mcast_out(dev, struct avr_spi_device, spi);
	rxbuf[counter] = avr_spi_recvbyte(spi);
	
	if (counter != len) {
		avr_spi_sendbyte(txbuf[++counter]);
	} else {
		avr_spi_irq_disable(spi);
	}

	//Восстанавливаем обменивающийся процесс.
	unwait_one(&spi->wait_list);
}

void link_avr_spi() {

}