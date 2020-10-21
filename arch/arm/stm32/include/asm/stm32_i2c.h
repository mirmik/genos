#ifndef GENOS_ARCH_ARM_STM32_I2C_H
#define GENOS_ARCH_ARM_STM32_I2C_H

#include <sys/cdefs.h>
#include <periph/map.h>

__BEGIN_DECLS

void stm32_i2c_enable(I2C_TypeDef * i2c);


void stm32_i2c_send_byte(I2C_TypeDef * i2c, char c) ;

void stm32_i2c_wait_txe_flag(I2C_TypeDef * i2c);
void stm32_i2c_wait_rxne_flag(I2C_TypeDef * i2c);

void stm32_i2c_set_start(I2C_TypeDef * i2c);

__END_DECLS

#endif