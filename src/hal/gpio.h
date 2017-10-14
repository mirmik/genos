#ifndef GXX_HAL_GPIO_H
#define GXX_HAL_GPIO_H


namespace hal {
	class gpio {
	public:
		class pin {
			virtual void set();
			virtual void clr();
			virtual void tgl();
			virtual void mode(int32_t settings); 
		};

		constexpr static uint8_t input = 0;
		constexpr static uint8_t output = 1;
	};
}

#endif