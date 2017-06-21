#ifndef GENOS_TIMER_COUNTER_H
#define GENOS_TIMER_COUNTER_H

#include <inttypes.h>
#include <compiler.h>

#include <kernel/irq.h>

template <typename UInt>
class TimerDevice {
	virtual UInt value() = 0;

	virtual void start() = 0;
	virtual void stop() = 0;

	virtual void setLimit(UInt limit) = 0;
	virtual void setDivider(uint16_t divider) = 0;

	virtual void setHandlerOverflow(Kernel::IRQHandler handler) = 0;
	virtual void interruptEnableOverflow(bool b) = 0;
};

//__BEGIN_DECLS

//uint32_t tc_counter_value(uint8_t channel);
//uint64_t tc_hr_counter_value(uint8_t channel);

//void tc_start(uint8_t channel, uint32_t divider);
//void tc_stop(uint8_t channel);

//void tc_set_limit(uint8_t channel, uint32_t limit);

//void tc_ovf_int_enable(uint8_t channel, void (*handler)());
//void tc_ovf_int_disable(uint8_t channel);

//void tc_debug_info(uint8_t channel);

//uint8_t tc_total();

//__END_DECLS

#endif