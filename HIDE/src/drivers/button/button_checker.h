#ifndef GENOS_BUTTON_CHECKER_H
#define GENOS_BUTTON_CHECKER_H

#include <igris/math/schmidt.h>
#include <drivers/gpio/pin.h>

#include <systime/systime.h>

#define BUTTON_PRESS_EVENT 0b001
#define BUTTON_LONG_PRESS_EVENT 0b010
#define BUTTON_RELEASE_EVENT 0b100
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
		button_checker(const genos::pin pin, uint8_t unpressed_level, uint8_t defence = 10) :
			schmidt(1*defence, 2*defence, 3*defence, unpressed_level), pin(pin), 
			unpressed_level(unpressed_level)
		{}

		int raw() 
		{
			return pin.get();	
		}

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