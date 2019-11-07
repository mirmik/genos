#ifndef DRIVERS_GPIO_ABSTRACT_H
#define DRIVERS_GPIO_ABSTRACT_H

namespace drivers 
{
	class gpio_pin_abstract 
	{
	public:
		virtual void set_value(int lvl = 1) = 0;
		virtual int get_value() = 0;
		
		virtual void set_direction(int dir) = 0;
		virtual int get_direction() = 0;
		
		virtual void set_config(void* config) = 0;
		
		virtual void toggle() { set_value( !get_value() ); };
	};
}

#endif