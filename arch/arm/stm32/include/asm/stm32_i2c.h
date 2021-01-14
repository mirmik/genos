#ifndef GENOS_ARCH_ARM_STM32_I2C_H
#define GENOS_ARCH_ARM_STM32_I2C_H

#include <sys/cdefs.h>
#include <periph/map.h>

#include <igris/dprint.h>
#include <igris/util/bits.h>

#include <asm/stm32_rcc.h>

#include <igris/util/bug.h>

#ifndef STM32G4XX
static inline 
int stm32_i2c_status_addr(I2C_TypeDef * i2c) 
{
	return i2c->SR1 & I2C_SR1_ADDR;
}

static inline
int stm32_i2c_status_start_bit(I2C_TypeDef * i2c) 
{
	return i2c->SR1 & I2C_SR1_SB;
}

static inline
void stm32_i2c_wait_txe_flag(I2C_TypeDef * i2c) 
{
	while (!(i2c->SR1 & I2C_SR1_TXE));
}

static inline
void stm32_i2c_wait_rxne_flag(I2C_TypeDef * i2c) 
{
	while (!(i2c->SR1 & I2C_SR1_RXNE));
}

static inline
void stm32_i2c_wait_btf_flag(I2C_TypeDef * i2c) 
{
	while (!(i2c->SR1 & I2C_SR1_BTF));
}

static inline
void stm32_i2c_enable(I2C_TypeDef * i2c) 
{
	stm32_rcc_enable_i2c(i2c);
	i2c->CR1 |= I2C_CR1_PE;
}

static inline 
void stm32_i2c_set_freq_field(I2C_TypeDef * i2c, uint8_t val) 
{
	bits_assign(i2c->CR2, 0b11111, val);
}

static inline
void stm32_i2c_send_byte(I2C_TypeDef * i2c, char c) 
{
	i2c->DR = c;
}

static inline
void stm32_i2c_set_start(I2C_TypeDef * i2c) 
{
	i2c->CR1 |= I2C_CR1_START;
}

static inline
void stm32_i2c_set_stop(I2C_TypeDef * i2c) 
{
	i2c->CR1 |= I2C_CR1_STOP;
}

static inline
void stm32_i2c_debug_print_status(I2C_TypeDef * i2c) 
{
	DPRINT(i2c->CR1);
	DPRINT(i2c->CR2);
	DPRINT(i2c->SR1);
	DPRINT(i2c->TRISE);
	DPRINT(i2c->CCR);
}
#else // STM32G4XX

static inline 
int stm32_i2c_status_addr(I2C_TypeDef * i2c) 
{
	BUG();
	//return i2c->ISR & I2C_ISR_ADDR;
}

static inline
int stm32_i2c_status_start_bit(I2C_TypeDef * i2c) 
{
	BUG();
	//return i2c->ISR & I2C_ISR_SB;
}

static inline
void stm32_i2c_wait_txe_flag(I2C_TypeDef * i2c) 
{
	BUG();
	//while (!(i2c->SR1 & I2C_SR1_TXE));
}

static inline
void stm32_i2c_wait_rxne_flag(I2C_TypeDef * i2c) 
{
	BUG();
	//while (!(i2c->SR1 & I2C_SR1_RXNE));
}

static inline
void stm32_i2c_wait_btf_flag(I2C_TypeDef * i2c) 
{
	BUG();
	//while (!(i2c->SR1 & I2C_SR1_BTF));
}

static inline
void stm32_i2c_enable(I2C_TypeDef * i2c) 
{
	BUG();
	//stm32_rcc_enable_i2c(i2c);
	//i2c->CR1 |= I2C_CR1_PE;
}

static inline 
void stm32_i2c_set_freq_field(I2C_TypeDef * i2c, uint8_t val) 
{
	BUG();
	//bits_assign(i2c->CR2, 0b11111, val);
}

static inline
void stm32_i2c_send_byte(I2C_TypeDef * i2c, char c) 
{
	BUG();
	//i2c->DR = c;
}

static inline
void stm32_i2c_set_start(I2C_TypeDef * i2c) 
{
	//i2c->CR1 |= I2C_CR1_START;
	BUG();
}

static inline
void stm32_i2c_set_stop(I2C_TypeDef * i2c) 
{
	//i2c->CR1 |= I2C_CR1_STOP;
	BUG();
}

static inline
void stm32_i2c_debug_print_status(I2C_TypeDef * i2c) 
{
	DPRINT(i2c->CR1);
	DPRINT(i2c->CR2);
	//DPRINT(i2c->SR1);
	//DPRINT(i2c->TRISE);
	//DPRINT(i2c->CCR);
}

#endif

#endif