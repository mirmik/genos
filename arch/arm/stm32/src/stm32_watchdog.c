#include <asm/stm32_watchdog.h>
#include <periph/map.h>
#include <igris/util/bits.h>
#include <igris/util/bug.h>
#include <assert.h>

void stm32_watchdog_init(uint32_t presc, uint32_t counts) 
{
	stm32_watchdog_start();
	stm32_watchdog_enable_access();
	stm32_watchdog_set_prescaller(presc);
	stm32_watchdog_set_reload_register(counts);
	stm32_watchdog_wait_ready();
	stm32_watchdog_refresh();
}

void stm32_watchdog_enable_access() 
{
	IWDG->KR = 0x00005555;	
}

void stm32_watchdog_start() 
{
	IWDG->KR = 0x0000CCCC;
}

void stm32_watchdog_refresh() 
{
	IWDG->KR = 0x0000AAAA;
}

void stm32_watchdog_set_prescaller(uint32_t presc) 
{
	uint8_t val;

	switch (presc) 
	{
		case 4: val = 0; break; 
		case 8: val = 1; break;
		case 16: val = 2; break;
		case 32: val = 3; break;
		case 64: val = 4; break;
		case 128: val = 5; break;
		case 256: val = 6; break;
		default: BUG(); break;
	} 

	IWDG->PR = val;
}

void stm32_watchdog_set_reload_register(uint16_t val) 
{
	assert(val <= 0x0FFF);
	IWDG->RLR = val;
}

uint32_t stm32_watchdog_status_register() 
{
	return IWDG->SR;
}

void stm32_watchdog_wait_ready() 
{
	while(IWDG->SR != 0);
}


