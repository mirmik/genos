#ifndef ADDON_RAMPS14
#define ADDON_RAMPS14

#include <drivers/gpio/arduino_pinout.h>
#include <ralgo/servo/impulse_writer.h>
#include <ralgo/util/math.h>
#include <igris/sync/syslock.h>

#include <drivers/timer/avr_timer.h>
#include <emergency_stop.h>

#include <limits.h>

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
	void emergency_stop();

	extern const gpio_pin& x_enable;
	extern const gpio_pin& x_dir;
	extern const gpio_pin& x_step;

	extern const gpio_pin& y_enable;
	extern const gpio_pin& y_dir;
	extern const gpio_pin& y_step;

	extern const gpio_pin& z_enable;
	extern const gpio_pin& z_dir;
	extern const gpio_pin& z_step;

	extern const gpio_pin& e_enable;
	extern const gpio_pin& e_dir;
	extern const gpio_pin& e_step;

	struct ramps_driver
	{
		using Timer = genos::avr::timer16;

		volatile uint16_t delta;
		Timer* timer;

		ramps_driver(Timer* timer);

		/*
			Настроить и активировать таймер и прерывания для работы драйвера.
			Устройство начнет работу сразу после запуска перываний.
		*/
		int init(uint32_t freq, uint8_t div);

		void isr_enabler();
		void isr_disabler();

		volatile uint32_t timer3_value;
	};

	struct impulse_driver : public ralgo::impulse_writer<igris::syslock, int64_t, float>
	{
		const gpio_pin& enable_pin;
		const gpio_pin& dir_pin;
		const gpio_pin& step_pin;

		volatile int32_t step_width = 10000;
		volatile int32_t step_counter = 0;

		volatile uint32_t less_impulses = 0;

		volatile bool enabled = false;
		
		volatile uint8_t dirval = 0;
		volatile int8_t spdsign = 1;

		impulse_driver(const gpio_pin& enable, const gpio_pin& dir, const gpio_pin& step)
			: enable_pin(enable), dir_pin(dir), step_pin(step)
		{
		}

		//void start_worker() override {}
		//void stop_worker() override {}
		void update_worker() override 
		{
			int64_t impulses = target_position - current_position;
			
			bool positive = evaluated_speed > 0;
			less_impulses = impulses > 0 ? impulses : -impulses;

			spdsign = positive ? 1 : -1;
			dirval = positive ? 0 : 1;
			
			if (evaluated_speed != 0) {
				step_width = 1000.0 / (evaluated_speed > 0 ? evaluated_speed : -evaluated_speed);
				//if (step_width < 200) step_width = 200;

				//dprln(step_width);

				if (step_width < 50) 
				{
					dprln("step_width < 50");
					ramps::emergency_stop();
				}

				if (step_width <= 0 || step_counter > step_width) 
					step_counter = step_width;
			}
			
			else 
			{
				step_width = LONG_MAX;
				step_counter = LONG_MAX;
			}
		}

		void enable()
		{
			enable_pin.mode(GPIO_MODE_OUTPUT);
			dir_pin.mode(GPIO_MODE_OUTPUT);
			step_pin.mode(GPIO_MODE_OUTPUT);
			enable_pin.set(0);
			dir_pin.set(0);
		}

		void serve(uint16_t delta) __attribute__((always_inline))
		{
			assert(delta < step_width);

//			dprln(delta, step_width);

			if (enabled && 
				(less_impulses != 0 || target_speed != 0.0))
			{
				step_counter -= delta;

				if (step_counter < 0)
				{
					step_counter += step_width;
					dir_pin.set(dirval);
					step_pin.set(1);
					less_impulses--;
					current_position += spdsign;
				}
			}
		}

		void print_state() 
		{
			dprln("tp", target_position, "cp", current_position, "es", evaluated_speed, "ts", target_speed, "sw", step_width, "dv", dirval);
		}
	};

	extern impulse_driver e_driver;
	extern impulse_driver x_driver;
	extern impulse_driver y_driver;
	extern impulse_driver z_driver;
}

#endif