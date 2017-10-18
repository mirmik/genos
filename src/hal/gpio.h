#ifndef GXX_HAL_GPIO_H
#define GXX_HAL_GPIO_H

#include <inttypes.h>

namespace hal {
	class gpio {
	public:
		class pin {
			virtual void set() = 0;
			virtual void clr() = 0;
			virtual void tgl() = 0;
			virtual void mode(int32_t settings) = 0; 
		};

		constexpr static uint8_t input = 0;
		constexpr static uint8_t output = 1;
	};
}

#endif