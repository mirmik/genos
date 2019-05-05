#ifndef GENOS_AVR_REGS_SPI_H
#define GENOS_AVR_REGS_SPI_H

#include <sys/cdefs.h>

#include <igris/util/bits.h>
#include <igris/util/bug.h>

#if 0

#ifdef CHIP_ATMEGA2560
#define MISO_PB   (1<<3)
#define MOSI_PB   (1<<2)
#define SCK_PB    (1<<1)
//#define SS_PB     (1<<0)
#else
#warning "Unrecognized chip"
#endif

//#define RESET_PB 1

__BEGIN_DECLS

static inline void avr_spi_init_master() {

#ifdef CHIP_ATMEGA2560
	SPCR = 0;

	// Set MOSI, SCK as Output
    DDRB |= MOSI_PB | SCK_PB;
    
    // MISO as input
    DDRB &= ~(MISO_PB);
 
    // Enable SPI, Set as Master
    // Prescaler: Fosc/16, Enable Interrupts
    //The MOSI, SCK pins are as per ATMega8
    //SPCR=(1<<SPE)|(1<<MSTR)|(1<<SPR0);//|(1<<SPIE);
    SPCR = (1<<SPE) | (1<<MSTR) | (1<<CPHA) | (1<<CPOL);
    SPSR = (1<<SPI2X);
#else
#warning "Unrecognized chip"
#endif
}

static inline void avr_spi_sendbyte(uint8_t tx) 
{ 
	SPDR = tx; 
}

static inline void avr_spi_txidlewait() 
{ 
	while(!(SPSR & 0x80)); 
}

static inline uint8_t avr_spi_recvbyte() 
{ 
	uint8_t rx = SPDR; 
	return rx; 
}

static inline uint8_t avr_spi_enable(bool en) 
{ 
	bits_lvl(SPCR, 1<<SPE, en);
}

static inline uint8_t avr_spi_irq_enable(bool en) 
{ 
	bits_lvl(SPCR, 1<<SPIE, en);
}

static inline uint8_t avr_spi_set_divider(uint8_t div) {
	uint8_t spr0;
	uint8_t spr1;
	uint8_t sp2x;

	switch (div) {
		case 4:    sp2x = 0; spr1 = 0;  spr0 = 0;   break;
		case 16:   sp2x = 0; spr1 = 0;  spr0 = 1;   break;
		case 64:   sp2x = 0; spr1 = 1;  spr0 = 0;   break;
		case 128:  sp2x = 0; spr1 = 1;  spr0 = 1;   break;

		case 2:    sp2x = 1; spr1 = 0;  spr0 = 0;   break;
		case 8:    sp2x = 1; spr1 = 0;  spr0 = 1;   break;
		case 32:   sp2x = 1; spr1 = 1;  spr0 = 0;   break;
		//case 64:  sp2x = 1; spr1 = 1;  spr0 = 1;   break;

		BUG();
	}

	bits_lvl(SPCR, 1<<SPR0, spr0);
	bits_lvl(SPCR, 1<<SPR1, spr1);
	bits_lvl(SPSR, 1<<SPI2X, sp2x);
} 

__END_DECLS

#endif

#endif