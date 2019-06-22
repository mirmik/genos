#include <drivers/spi/avr_spi.h>
#include <genos/wait.h>

#include <drivers/gpio/pin.h>
#include <avr/io.h>

#include <stdint.h>
#include <errno.h>
#include <assert.h>

#include <periph/regs/avr/spi.h>

#if 0

void avr_spi_device::init(const char * name) {
	avr_spi_init_master();
	//avr_spi_set_divider(2);
}

int avr_spi_device::setfrequency (uint32_t freq) 
{
	
}

int avr_spi_device::enable (bool en) {
	return avr_spi_enable(en);
} 

int avr_spi_device::exbyte (int byte) 
{
	avr_spi_sendbyte(byte);
	avr_spi_txidlewait();
	return avr_spi_recvbyte();
}

int avr_spi_device::select (void *slct, int en) 
{
	struct gpio_pin * npin = (struct gpio_pin *) slct;
	struct gpio_pin * opin = (struct gpio_pin *) current_select;

	if (en == 0) {
		assert(npin == opin);
		gpio_pin_write(opin, 1);
		current_select = nullptr;
		return 0;	
	}

	if (opin != nullptr) {
		gpio_pin_write(opin, 1);
	}
	gpio_pin_write(npin, 0);
		
	current_select = slct;
	return 0;
}

int avr_spi_device::exchange (const void *_txbuf, void *_rxbuf, int len) 
{
	int sts;
	uint8_t * rxbuf = (uint8_t *) _rxbuf;
	uint8_t * txbuf = (uint8_t *) _txbuf;

	//if (flags && NOSCHED) {
	while(len--) {
		avr_spi_sendbyte(*txbuf++);
		avr_spi_txidlewait();
		*rxbuf++ = avr_spi_recvbyte();
	}
	return 0;
	//}

	/*while (in_work) {
		sts = wait_current_schedee(&spi->wait_list, 0);
		if (sts == -1) {
			return EAGAIN;
		}
	}

	spi->txbuf = txbuf;
	spi->rxbuf = rxbuf;
	spi->counter = 0;
	spi->len = len;

	avr_spi_sendbyte(spi->txbuf[0]);
	avr_spi_irq_enable();

	//Ставим поток в начало очереди, чтобы обработчик прерывания снял его оттуда позже.
	wait_current_schedee(&spi->wait_list, WAIT_PRIORITY);

	//Отдаем управление следующему процессу, если такой находится.
	spi->in_work = 0;
	unwait_one(&spi->wait_list);

	return 0;*/
}

/*
struct avr_spi_device : public spi_device {
	struct spi_device spi;

	void * current_select;
    
    const uint8_t *txbuf;
	uint8_t *rxbuf;
	size_t len;
	size_t counter;

	uint8_t mode;

	int32_t frequency;
	struct dlist_head wait_list; 
	uint8_t in_work;
};*/

/*static inline void avr_spi_sendbyte(uint8_t tx) { SPDR = tx; }
static inline void avr_spi_txidlewait() { while(!(SPSR & 0x80)); }
static inline uint8_t avr_spi_recvbyte() { uint8_t rx = SPDR; return rx; }
static inline uint8_t avr_spi_irq_enable() { panic("TODO"); return -1; }
static inline uint8_t avr_spi_irq_disable() { panic("TODO"); return -1; }

int avr_spi_select (struct spi_device *dev, void *slct, int en) {
	struct avr_spi_device * spi = mcast_out(dev, struct avr_spi_device, spi);
	struct gpio_pin * npin = (struct gpio_pin *) slct;
	struct gpio_pin * opin = (struct gpio_pin *) spi->current_select;

	if (en == 0) {
		assert(npin == opin);
		gpio_pin_write(opin, 0);
		spi->current_select = NULL;
		return 0;	
	}

	if (opin != NULL) {
		gpio_pin_write(opin, 0);
	}
	gpio_pin_write(npin, 1);
		
	spi->current_select = slct;
	return 0;
}

int avr_spi_exchange (struct spi_device *dev, const void *txbuf, void *rxbuf, int len, int flags) {
	int sts;
	struct avr_spi_device * spi = mcast_out(dev, struct avr_spi_device, spi);
	uint8_t * _rxbuf = (uint8_t *) rxbuf;
	uint8_t * _txbuf = (uint8_t *) txbuf;

	if (flags && NOSCHED) {
		while(len--) {
			avr_spi_sendbyte(*_txbuf++);
			avr_spi_txidlewait();
			*_rxbuf++ = avr_spi_recvbyte();
		}
		return 0;
	}

	while (spi->in_work) {
		sts = wait_current_schedee(&spi->wait_list, 0);
		if (sts == -1) {
			return EAGAIN;
		}
	}

	spi->txbuf = txbuf;
	spi->rxbuf = rxbuf;
	spi->counter = 0;
	spi->len = len;

	avr_spi_sendbyte(spi->txbuf[0]);
	avr_spi_irq_enable();

	//Ставим поток в начало очереди, чтобы обработчик прерывания снял его оттуда позже.
	wait_current_schedee(&spi->wait_list, WAIT_PRIORITY);

	//Отдаем управление следующему процессу, если такой находится.
	spi->in_work = 0;
	unwait_one(&spi->wait_list);

	return 0;
}


void avr_spi_irq_handler(struct spi_device * dev) {
	struct avr_spi_device * spi =  mcast_out(dev, struct avr_spi_device, spi);
	spi->rxbuf[spi->counter] = avr_spi_recvbyte(spi);
	
	if (spi->counter != spi->len) {
		avr_spi_sendbyte(spi->txbuf[++spi->counter]);
	} else {
		avr_spi_irq_disable(spi);
	}

	//Восстанавливаем обменивающийся процесс.
	unwait_one(&spi->wait_list);
}

struct avr_spi_device avr_spi;

const struct spi_operations avr_spi_operations = {
	.select = avr_spi_select,
	.exchange = avr_spi_exchange,
};

struct spi_device * get_avr_spi_device() {
	static char inited = 0;

	if (!inited) {
		avr_spi.current_select = NULL;
		spi_device_init(&avr_spi.spi, &avr_spi_operations);
	}

	return &avr_spi.spi;
}*/

#endif