#ifndef RL_SYNCRO_V2_BOARD
#define RL_SYNCRO_V2_BOARD

#include <sys/cdefs.h>
#include <zillot/stm32/pin.h>
#include <zillot/stm32/usart.h>

#define HSE_FREQ 8000000

extern zillot::stm32::usart usart2;
extern zillot::stm32::usart usart6;

extern zillot::stm32::pin board_led;
extern zillot::stm32::pin board_led2;
extern zillot::stm32::pin extpin[4];

__BEGIN_DECLS

void board_init();

__END_DECLS

#endif