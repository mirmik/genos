#include <asm/adc.h>
#include <asm/rcc.h>

#include <igris/util/bits.h>

void stm32_adc_enable(struct stm32_adc_regs * regs, int en) 
{
	bits_lvl(regs->CR2, ADC_CR2_ADON, en);
}

void stm32_adc_enable_clocking(struct stm32_adc_regs * regs) 
{
	rcc_enable_adc(regs);
}


/*void stm32_adc_start_single_conversion(struct stm32_adc_regs * regs) 
{
	bits_set(regs->CR2, ADC_CR2_SWSTART);
}

int stm32_adc_set_resolution(struct stm32_adc_regs * regs, uint8_t bits) 
{
	uint8_t code;

	switch (bits) 
	{
		case 12 : code = 0b00; break;
		case 10 : code = 0b01; break;
		case 8 :  code = 0b10; break;
		case 6 :  code = 0b11; break;
		default : return -1;
	}

	bits_assign_bias(regs->CR1, 0b11, code, 8);
	return 0;
}

uint16_t stm32_adc_get_data(struct stm32_adc_regs * regs) 
{
	return regs->DR;
}*/