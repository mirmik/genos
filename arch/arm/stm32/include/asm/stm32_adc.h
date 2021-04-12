#ifndef GENOS_ARM_STM32_ADC_H
#define GENOS_ARM_STM32_ADC_H

#include <sys/cdefs.h>
#include <periph/map.h>

#include <igris/util/bits.h>
#include <igris/dprint.h>

__BEGIN_DECLS

#if 0
void stm32_adc_enable(ADC_TypeDef * regs, int en);
void stm32_adc_set_channel(ADC_TypeDef * regs, int ch);

static inline
void stm32_adc_start_single_conversion(ADC_TypeDef * regs) 
{
	bits_set(regs->CR2, ADC_CR2_SWSTART);
}


static inline
void stm32_adc_enable_analog_watchdog(ADC_TypeDef * regs) 
{
	bits_set(regs->CR1, ADC_CR1_AWDEN);
}

static inline
int stm32_adc_set_resolution(ADC_TypeDef * regs, uint8_t bits) 
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

static inline
uint16_t stm32_adc_get_data(ADC_TypeDef * regs) 
{
	return regs->DR;
}

static inline
uint32_t stm32_adc_status(ADC_TypeDef * regs) 
{
	return regs->SR;
}

static inline
void stm32_adc_continuos_mode(ADC_TypeDef * regs, int en) 
{
	regs->CR2 |= ADC_CR2_CONT;
}

static inline 
void stm32_adc_debug_print(ADC_TypeDef * regs) 
{
	dprhex_noted("SR: ", regs->SR);
	dprhex_noted("CR1: ", regs->CR1);
	dprhex_noted("CR2: ", regs->CR2);
	dprhex_noted("SMPR1: ", regs->SMPR1);
	dprhex_noted("SMPR2: ", regs->SMPR2);
	dprhex_noted("JOFR1: ", regs->JOFR1);
	dprhex_noted("JOFR2: ", regs->JOFR2);
	dprhex_noted("JOFR3: ", regs->JOFR3);
	dprhex_noted("JOFR4: ", regs->JOFR4);
	dprhex_noted("HTR: ", regs->HTR);
	dprhex_noted("LTR: ", regs->LTR);
	dprhex_noted("SQR1: ", regs->SQR1);
	dprhex_noted("SQR2: ", regs->SQR2);
	dprhex_noted("SQR3: ", regs->SQR3);
	dprhex_noted("JSQR: ", regs->JSQR);
	dprhex_noted("JDR1: ", regs->JDR1);
	dprhex_noted("JDR2: ", regs->JDR2);
	dprhex_noted("JDR3: ", regs->JDR3);
	dprhex_noted("JDR4: ", regs->JDR4);
	dprhex_noted("DR: ", regs->DR);
}

static inline 
void stm32_adc_regular_channel_sequence_length(ADC_TypeDef * regs, int total) 
{
	bits_assign_bias(regs->SQR1, 0b1111, total - 1, 20);
}

static inline
void stm32_adc_regular_channel_rank(ADC_TypeDef * regs, int ch, int rank) 
{
	uint8_t bias;
	volatile uint32_t * sqr;

	if (rank >= 13) 
	{
		rank -= 13 + 1;
		sqr = &regs->SQR1;
	} 

	else if (rank >= 7) 
	{
		rank -= 7 + 1;	
		sqr = &regs->SQR2;
	} 

	else 
	{
		rank -= 1;
		sqr = &regs->SQR3;	
	}

	bias = rank * 5;
	ch = ch & 0b11111;
	bits_assign_bias(*sqr, 0b11111, ch, bias);
}

#endif
__END_DECLS

#endif