#ifndef GENOS_BUTTON_CHECKER_H
#define GENOS_BUTTON_CHECKER_H

#include <igris/math/schmidt.h>
#include <drivers/gpio/pin.h>

#include <systime/systime.h>

#define BUTTON_PRESS_EVENT 1
#define BUTTON_LONG_PRESS_EVENT 2
#define BUTTON_RELEASE_EVENT 3
#define BUTTON_NONE 0

// Простой драйвер кнопки. Не использует прерывания.
// Учитывает дребезг контактов.
// Расчитывает время нажатия.

namespace genos
{
	class button_checker
	{
		incremental_schmidt schmidt;
		const genos::pin pin;

		time_t presstime = 0;
		time_t longtime = 300;

		bool unpressed_level;

	public:
		union
		{
			uint8_t sts;
			struct
			{
				uint8_t press : 1;
				uint8_t longpress : 1;
			};
		};

	public:
		button_checker(const genos::pin pin, uint8_t unpressed_level) :
			schmidt(10, 20, 30, unpressed_level), pin(pin), 
			unpressed_level(unpressed_level)
		{}

		int check()
		{
			int val = pin.get();
			bool ret = schmidt.check(val ? +1 : -1);

			if (unpressed_level) 
				ret = !ret;

			if (ret == true)
			{
				if (press == 0)
				{
					press = 1;
					presstime = millis();
					return BUTTON_PRESS_EVENT;
				}

				if (press && !longpress && millis() - presstime > longtime)
				{
					longpress = 1;
					return BUTTON_LONG_PRESS_EVENT;
				}
			}

			else 
			{
				if (press == 1) 
				{
					press = 0;
					longpress = 0;
					return BUTTON_RELEASE_EVENT;
				}
			}

			return BUTTON_NONE;
		}
	};
}

#endif