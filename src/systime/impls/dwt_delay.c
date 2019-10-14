#include <asm/dwt.h>

void delayMicroseconds(uint32_t us) 
{
	arm_dwt_delay_us(us);
}