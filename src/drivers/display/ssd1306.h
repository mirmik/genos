#ifndef GENOS_DRIVERS_H
#define GENOS_DRIVERS_H

#include <drivers/i2c/i2c_client.h>

class ssd1306 : public genos::i2c_client
{
public:
	ssd1306() {}
};

#endif