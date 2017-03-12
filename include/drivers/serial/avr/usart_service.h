#ifndef GENOS_AVR_USART_SERVICE_H
#define GENOS_AVR_USART_SERVICE_H

#include <compiler.h>
#include <kernel/ioservice/ioservice.h>

struct avr_usart_ioservice {
	struct ioservice iosrvs;
};

__BEGIN_DECLS 

static int usart_io_write(struct ioservice* srvs, const char* data, size_t sz) {

}

static int usart_io_read(struct ioservice* srvs, char* data, size_t sz) {

}

static qid_t create_avr_usart_service(const usart_data* udata, size_t txbufsz, size_t rxbufsz) {

}

__END_DECLS

#endif