#ifndef GENOS_DRIVERS_DISCRETE_POTENTIOMETR_H
#define GENOS_DRIVERS_DISCRETE_POTENTIOMETR_H

namespace genos 
{
	class discrete_potentiometr 
	{
	private:
		float maxval;
		int discs;
		float step;

		int curdisc = 0;

	public:
		discrete_potentiometr(float maxval, int32_t discs) 
			: maxval(maxval), discs(discs), step(maxval / discs) 
		{

		}

		int serve(float cur) 
		{
			if (cur > maxval) 
			{
				cur = maxval;
				curdisc = discs;
				return curdisc;
			}


			float curval = curdisc * step;
			float delta = abs(cur - curval);

			if (delta > step / 2)
				curdisc = cur / step + 0.5;
			
			return curdisc;
		}
	};
}


#endif