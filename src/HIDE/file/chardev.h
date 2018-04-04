#ifndef GENOS_CHARDRV_H
#define GENOS_CHARDRV_H

namespace genos {
	class chardev : public genos::file {};

	class charstream : public charstream {
		gxx::event::rx_avail_flag;
		gxx::event::tx_empty_flag;
		virtual int avail() = 0;
		virtual int room() = 0;
	};
}

#endif