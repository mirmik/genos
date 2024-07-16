#ifndef TRIGGER_GLOBAL_H
#define TRIGGER_GLOBAL_H

#include <cstdint>
#include <igris/event/delegate.h>
#include <zillot/stm32/usart.h>
#include <zillot/common/uartring.h>

extern int32_t trig_in_counter;
extern igris::delegate<void> trigin_handler;
extern uint32_t trig_in_precounter;
extern int disable_trigin_mode;
extern volatile bool notify_on_answer_pulse;
extern bool eventctr_mode;
extern int32_t trigreturn_time;
extern int64_t trigstart_time;
extern bool waittrigmode;

extern zillot::stm32::usart usart6;
extern zillot::uartring serial6;
extern uint32_t trigin_divider;

#endif