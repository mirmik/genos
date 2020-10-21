#include <asm/stm32_i2c.h>
#include <asm/stm32_rcc.h>

void stm32_i2c_enable(I2C_TypeDef * i2c) 
{
	stm32_rcc_enable_i2c(i2c);
	i2c->CR1 |= I2C_CR1_PE;
}

void stm32_i2c_send_byte(I2C_TypeDef * i2c, char c) 
{
	i2c->DR = c;
}

void stm32_i2c_wait_txe_flag(I2C_TypeDef * i2c) 
{
	while (!(i2c->SR1 & I2C_SR1_TXE));
}

void stm32_i2c_wait_rxne_flag(I2C_TypeDef * i2c) 
{
	while (!(i2c->SR1 & I2C_SR1_RXNE));
}

void stm32_i2c_set_start(I2C_TypeDef * i2c) 
{
	i2c->CR1 |= I2C_CR1_START;
}

void stm32_i2c_set_stop(I2C_TypeDef * i2c) 
{
	i2c->CR1 |= I2C_CR1_STOP;
}