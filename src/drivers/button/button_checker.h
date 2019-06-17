#ifndef GENOS_BUTTON_CHECKER_H
#define GENOS_BUTTON_CHECKER_H

#include <drivers/gpio/pin.h>
#include <igris/math/schmidt.h>

// Простой драйвер кнопки. Не использует прерывания.
// Учитывает дребезг контактов.
// Расчитывает время нажатия.

namespace genos
{
	class button_checker
	{
		genos::pin pin;
		incremental_schmidt schmidt;

	public:
		button_checker() :
			schmidt(10, 20, 30)
		{}

		int check()
		{
			int val = pin.get();
			bool ret = schmidt.check(val ? +1 : -1);
		}
	};
}

#endif