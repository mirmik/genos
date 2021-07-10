#ifndef GENOS_ABSTRACT_GPIO_PIN_H
#define GENOS_ABSTRACT_GPIO_PIN_H

namespace genos 
{
	class abstract_gpio_pin
	{
	public:
		virtual int set(unsigned char level) = 0;
		virtual int get() = 0;
		virtual int toggle() = 0;
		virtual int mode(uint32_t mode, int arg) = 0;
	};
}

#endif