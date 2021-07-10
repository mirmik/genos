#ifndef GENOS_DRIVERS_BL_HM_10S_A_H
#define GENOS_DRIVERS_BL_HM_10S_A_H

namespace genos
{
	class hm_10s_a
	{
		genos::uartring * serial;

	public:
		hm_10s_a(genos::uartring * serial) :
			serial(serial)
		{}

		void begin()
		{
		};
	};
}

#endif