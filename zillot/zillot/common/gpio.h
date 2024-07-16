#ifndef ZILLOT_COMMON_GPIO_H
#define ZILLOT_COMMON_GPIO_H

#include <stdint.h>

namespace zillot 
{
	class gpio 
	{
	public:
		virtual void write(int mask, int val) = 0;
		virtual int read(int mask) = 0;
		virtual void toggle(int mask) = 0;
		virtual void pull_up(int mask, int lvl) = 0;
		virtual int setup(int mask, uint32_t mode) = 0;	
	};
}

#endif