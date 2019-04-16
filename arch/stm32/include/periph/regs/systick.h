#ifndef STM32_SYSTICK_REGS_H
#define STM32_SYSTICK_REGS_H

struct systick_regs 
{
	volatile uint32_t CSR;   // Control and status register
	volatile uint32_t RVR;   // Reload value register
	volatile uint32_t CVR;   // Current value register
	volatile uint32_t CALIB; // Calibration register
};

#define SYSTICK_VALUE_MASK 0x00FFFFFF

#define SYSTICK_CTRL_CLKSOURCE (1 << 2) 
#define SYSTICK_CTRL_TICKINT (1 << 1) 
#define SYSTICK_CTRL_ENABLE (1 << 0)  

#endif