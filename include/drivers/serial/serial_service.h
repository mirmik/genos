#ifndef GENOS_SERIAL_SERVICE_H
#define GENOS_SERIAL_SERVICE_H

#include <kernel/service/service.h>
#include <drivers/serial/serial_driver.h>

namespace genos {

	class serial_service : public service {
		serial_driver* driver;

	public:

		serial_service(serial_driver* driver) : driver(driver) {}; 
	};

}

#endif