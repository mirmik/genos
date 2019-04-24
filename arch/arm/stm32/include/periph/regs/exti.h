#ifndef STM32_REGS_EXTI_H
#define STM32_REGS_EXTI_H

struct exti_regs 
{
	volatile uint32_t IMR;	// Interrupt mask register
	volatile uint32_t EMR;	// Event mask register
	volatile uint32_t RTSR; // Rising trigger selection register
	volatile uint32_t FTSR; // Falling trigger selection register
	volatile uint32_t SWIER;// Software interrupt event register
	volatile uint32_t PR;   // Pending register
};



#endif