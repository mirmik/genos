#include <drivers/serial/tty.h>

#include "genos_configure.h"

#include <stdlib.h>
#include <assert.h>

#include <genos/panic.h>
#include <kernel/csection.h>

#include <util/math.h>

#define DEFAULT_RING_TTY_RX_BUFLEN 128
#define DEFAULT_RING_TTY_TX_BUFLEN 128

int Teletype::open() {
	rx_ring.reserve(DEFAULT_RING_TTY_TX_BUFLEN);
	tx_ring.reserve(DEFAULT_RING_TTY_RX_BUFLEN);
}

int Teletype::close() {
	rx_ring.invalidate();
	tx_ring.invalidate();
}

//int Teletype::putc(char c) {
//	write(&c,1);
//}

//int Teletype::getc() {
//	if (rx_ring.is_empty()) return -1;
//	auto save = ipl_save();
//	char c = rx_ring.getc();
//	ipl_restore(save);
//	return c;
//}

int Teletype::write (const char *buffer, int buflen, uint8_t opt) {
	
	char* end = (char*)buffer + buflen;
	size_t sended = 0; 
	
	if (buflen == 0) return 0;

	auto save = ipl_save();

	uint8_t empty = tx_ring.is_empty();
	uint8_t ready = uart->ready();

	if (empty && ready) {
		uart->putc(*buffer++);
		sended++;
	};

	while(buffer != end) {
		sended += tx_ring.putc(*(buffer++));
	};

	ipl_restore(save);
	return sended;

}

int Teletype::read(char* str, int len, uint8_t opt) {
	auto save = ipl_save();

	int total = min(len, rx_ring.avail());
	for (int i = 0; i < total; ++i) {
		*str++ = rx_ring.getc();
	}	
	
	ipl_restore(save);
	return total;
}

off_t Teletype::seek (off_t offset, int whence) {
	return 0;
}

void Teletype::tx_handler() {
	if (!uart->ready() || tx_ring.is_empty()) return;
	uart->putc(tx_ring.getc());
}

void Teletype::rx_handler() {
	rx_ring.putc(uart->getc());
}

size_t Teletype::avail() {
	return rx_ring.avail();
};
