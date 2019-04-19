#include <addons/ramps/ramps.h>

#include <drivers/gpio/gpio.h>
#include <drivers/gpio/pin.h>

#include <igris/dprint.h>

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

	impulse_driver e_driver { PINOUT[E_ENABLE_PIN], PINOUT[E_DIR_PIN], PINOUT[E_STEP_PIN] };
	impulse_driver x_driver { PINOUT[X_ENABLE_PIN], PINOUT[X_DIR_PIN], PINOUT[X_STEP_PIN] };
	impulse_driver y_driver { PINOUT[Y_ENABLE_PIN], PINOUT[Y_DIR_PIN], PINOUT[Y_STEP_PIN] };
	impulse_driver z_driver { PINOUT[Z_ENABLE_PIN], PINOUT[Z_DIR_PIN], PINOUT[Z_STEP_PIN] };

	ramps_driver::ramps_driver(Timer* timer) : timer(timer) {}

	int ramps_driver::init(uint32_t freq, uint8_t div)
	{
		int32_t ocr = F_CPU / (freq * div);
		assert(ocr < 0x03FF);
		DPRINT(ocr);

		delta = ocr;

		periph::timer3.set_mode(Timer::TimerMode::Clock);

		timer->set_mode(Timer::TimerMode::CTC);
		timer->set_compare_a(ocr);
		timer->set_compare_b(ocr / 2);

		genos::irqtable::set_handler(timer->irqs.compa, &ramps_driver::isr_enabler, this);
		genos::irqtable::set_handler(timer->irqs.compb, &ramps_driver::isr_disabler, this);

		timer->irq_compare_a_enable(true);
		timer->irq_compare_b_enable(true);

		x_driver.enable();
		y_driver.enable();
		z_driver.enable();

		timer->set_divider(div);

		return 0;
	}

	void ramps_driver::isr_enabler()
	{
		//periph::timer3.set_value(0);
		//periph::timer3.set_divcode(1);

		x_driver.serve(delta);
		y_driver.serve(delta);
		z_driver.serve(delta);

		//periph::timer3.set_divcode(0);
		//timer3_value = periph::timer3.value();
	}

	void ramps_driver::isr_disabler()
	{
		x_driver.step_pin.set(0);
		y_driver.step_pin.set(0);
		z_driver.step_pin.set(0);
	}
}