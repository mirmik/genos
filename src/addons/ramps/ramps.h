#ifndef ADDON_RAMPS14
#define ADDON_RAMPS14

#include <drivers/gpio/arduino_pinout.h>
#include <ralgo/servo/impulse_writer.h>
#include <gxx/syslock.h>

#define X_STEP_PIN         54
#define X_DIR_PIN          55
#define X_ENABLE_PIN       38
#define X_MIN_PIN           3
#define X_MAX_PIN           2

#define Y_STEP_PIN         60
#define Y_DIR_PIN          61
#define Y_ENABLE_PIN       56
#define Y_MIN_PIN          14
#define Y_MAX_PIN          15

#define Z_STEP_PIN         46
#define Z_DIR_PIN          48
#define Z_ENABLE_PIN       62
#define Z_MIN_PIN          18
#define Z_MAX_PIN          19

#define E_STEP_PIN         26
#define E_DIR_PIN          28
#define E_ENABLE_PIN       24

namespace ramps
{
	const gpio_pin& x_enable = 		PINOUT[X_ENABLE_PIN];
	const gpio_pin& x_dir = 		PINOUT[X_DIR_PIN];
	const gpio_pin& x_step = 		PINOUT[X_STEP_PIN];

	const gpio_pin& y_enable = 		PINOUT[Y_ENABLE_PIN];
	const gpio_pin& y_dir = 		PINOUT[Y_DIR_PIN];
	const gpio_pin& y_step = 		PINOUT[Y_STEP_PIN];

	const gpio_pin& z_enable = 		PINOUT[Z_ENABLE_PIN];
	const gpio_pin& z_dir = 		PINOUT[Z_DIR_PIN];
	const gpio_pin& z_step = 		PINOUT[Z_STEP_PIN];

	const gpio_pin& e_enable = 		PINOUT[E_ENABLE_PIN];
	const gpio_pin& e_dir = 		PINOUT[E_DIR_PIN];
	const gpio_pin& e_step = 		PINOUT[E_STEP_PIN];

	struct impulse_driver : public ralgo::impulse_writer<gxx::syslock, int64_t, float>
	{
		const gpio_pin& enable_pin;
		const gpio_pin& dir_pin;
		const gpio_pin& step_pin;

		int32_t step_width=10000;
		int32_t step_counter=0;

		bool enabled;

		impulse_driver(const gpio_pin& enable, const gpio_pin& dir, const gpio_pin& step)
			: enable_pin(enable), dir_pin(dir), step_pin(step)
		{
		}

		void start_worker() override {}
		void stop_worker() override {}
		void set_speed(float spd) override {}

		void enable() { 
			enable_pin.mode		(GPIO_MODE_OUTPUT); 
			dir_pin.mode		(GPIO_MODE_OUTPUT); 
			step_pin.mode		(GPIO_MODE_OUTPUT); 
			enable_pin.set(0); 
			dir_pin.set(0); 
		}

		void step_toggle() 
		{
			step_pin.tgl();
		}
		
		void step_enable() 
		{
			step_pin.set(1);
		}
		
		void step_disable() 
		{
			step_pin.set(0);
		}

		void serve_enable(int32_t delta) 
		{
			if (enabled) 
			{
				step_counter -= delta;
				if (step_counter < 0) 
				{
					step_counter += step_width;
					step_enable();
				} 
			}
		}

		void serve_disable() 
		{
			if (enabled) 
			{
				step_disable();
			}
		}
	};

	impulse_driver e_driver { PINOUT[E_ENABLE_PIN], PINOUT[E_DIR_PIN], PINOUT[E_STEP_PIN] };
	impulse_driver x_driver { PINOUT[X_ENABLE_PIN], PINOUT[X_DIR_PIN], PINOUT[X_STEP_PIN] };
	impulse_driver y_driver { PINOUT[Y_ENABLE_PIN], PINOUT[Y_DIR_PIN], PINOUT[Y_STEP_PIN] };
	impulse_driver z_driver { PINOUT[Z_ENABLE_PIN], PINOUT[Z_DIR_PIN], PINOUT[Z_STEP_PIN] };

	template <typename Timer>
	struct ramps_driver 
	{
		int delta;
		Timer* timer;

		ramps_driver(Timer* timer) : timer(timer) {}

		void init(uint32_t freq)
		{
			delta = F_CPU / freq;
			timer->set_freq(freq);
			timer->regs->ocr_b = timer->regs->ocr_a / 2;
			genos::irqtable::set_handler(timer->irqs.compa, &ramps_driver::isr_enabler, this);
			genos::irqtable::set_handler(timer->irqs.compb, &ramps_driver::isr_disabler, this);
			timer->irq_compare_a_enable(true);
			timer->irq_compare_b_enable(true);

			x_driver.enable();
			y_driver.enable();
			z_driver.enable();
		}

		void isr_enabler() 
		{
			x_driver.serve_enable(delta);
			y_driver.serve_enable(delta);
			z_driver.serve_enable(delta);
		}

		void isr_disabler() 
		{
			x_driver.serve_disable();
			y_driver.serve_disable();
			z_driver.serve_disable();
		}		
	};
}

#endif