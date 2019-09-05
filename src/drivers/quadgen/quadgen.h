#ifndef GENOS_DRIVERS_QUADGEN_H
#define GENOS_DRIVERS_QUADGEN_H

#include <igris/util/graycode.h>
#include <drivers/gpio/pin.h>

namespace genos 
{
	class quadgen_differential
	{
		genos::gpio_pin apin0; 
		genos::gpio_pin apin1; 
		genos::gpio_pin bpin0; 
		genos::gpio_pin bpin1;

	public:
		uint8_t state = 0;

	public:
		quadgen_differential(
			genos::gpio_pin apin0, genos::gpio_pin apin1, 
			genos::gpio_pin bpin0, genos::gpio_pin bpin1
		) 
			: apin0(apin0), apin1(apin1), bpin0(bpin0), bpin1(bpin1)
		{}

		quadgen_differential() {}

		quadgen_differential(const quadgen_differential& q) :
			apin0(q.apin0), apin1(q.apin1), bpin0(q.bpin0), bpin1(q.bpin1),
			state(q.state)
		{}

		void set() 
		{
			uint8_t setcode = igris::graycode(state);

			bool a = (bool)(setcode & 0b01);
			bool b = (bool)(setcode & 0b10);

			apin0.set(a);
			apin1.set(!a);
			bpin0.set(b);
			bpin1.set(!b);
		}

		void inc() 
		{
			state = state == 3 ? 0 : state + 1;
			set();
		}

		void dec() 
		{
			state = state == 0 ? 3 : state - 1;
			set();
		}
	};
}

#endif