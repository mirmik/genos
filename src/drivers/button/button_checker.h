#ifndef GENOS_BUTTON_CHECKER_H
#define GENOS_BUTTON_CHECKER_H

#include <drivers/gpio/pin.h>
#include <igris/math/schmidt.h>

#include <systime/systime.h>

// Простой драйвер кнопки. Не использует прерывания.
// Учитывает дребезг контактов.
// Расчитывает время нажатия.

namespace genos
{
	class button_checker
	{
		incremental_schmidt schmidt;
		const genos::pin pin;

		bool last = true;
		time_t lastchange = 0;

	public:
		button_checker(const genos::pin pin) :
			schmidt(10, 20, 30, last), pin(pin)
		{}

		int check()
		{
			int val = pin.get();
			bool ret = schmidt.check(val ? +1 : -1);

			if (ret != last) 
			{
				lastchange = millis();
			}

			return ret;
		}
	};
}

#endif