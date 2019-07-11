#ifndef GENOS_DRIVERS_DISCRETE_POTENTIOMETR_H
#define GENOS_DRIVERS_DISCRETE_POTENTIOMETR_H

namespace genos 
{
	class discrete_potentiometr 
	{
		float maxval;
		float step;
		int discs;

		int curdisc = 0;

		discrete_potentiometr(float maxval, int32_t discs) 
			: maxval(maxval), discs(discs), step(maxval / discs) 
		{

		}

		int serve(float cur) 
		{
			float curval = curdisc * step;
			float delta = abs(cur - curval);

			if (delta > step)
				curdisc = cur / step + 0.5;
			
			return curdisc;
		}
	};
}


#endif