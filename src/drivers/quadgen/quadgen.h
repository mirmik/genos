#ifndef GENOS_DRIVERS_DUALGEN_H
#define GENOS_DRIVERS_DUALGEN_H

#include <igris/util/graycode.h>
#include <drivers/gpio/pin.h>

// Квадратурный генератор, работающий на дифференциальную пару для случая, 
// когда выходы аппаратно отображены в дифферентциальную пару.

// Ну, или просто квадратурный генератор.

namespace genos 
{
	class quadgen
	{
		genos::gpio_pin apin; 
		genos::gpio_pin bpin;

	public:
		uint8_t state = 0;

	public:
		quadgen(
			genos::gpio_pin apin, 
			genos::gpio_pin bpin
		) 
			: apin(apin), bpin(bpin)
		{}

		quadgen() {}

		quadgen(const quadgen& q) :
			apin(q.apin), bpin(q.bpin), state(q.state)
		{}

		void set() 
		{
			uint8_t setcode = igris::graycode(state);
			
			bool a = (bool)(setcode & 0b01);
			bool b = (bool)(setcode & 0b10);

			apin.set(a);
			bpin.set(b);
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