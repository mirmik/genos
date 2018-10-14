#ifndef GENOS_AVR_SPI_H
#define GENOS_AVR_SPI_H

#include <stdint.h>
#include <stdlib.h>
#include <drivers/spi/spi.h>

#include <gxx/datastruct/dlist.h>

struct avr_spi_device : public spi_device {
//	struct spi_device spi;

	void * current_select = nullptr;
    
    //const uint8_t *txbuf;
	//uint8_t *rxbuf;
	/*size_t len;
	size_t counter;

	uint8_t mode;

	int32_t frequency;
	struct dlist_head wait_list; 
	uint8_t in_work;*/


	int select   		(void *slct, int en) 						override;
	int enable 	 		(bool en = true) 							override;
	int exchange 		(const void *txbuf, void *rxbuf, int len) 	override;
	int exbyte 			(int byte) 									override;
	int setfrequency	(uint32_t freq) 							override;

	void init(const char * name) override;

};

#endif 