#include <asm/stm32_i2c.h>
#include <asm/stm32_rcc.h>

void stm32_i2c_debug_print_status(I2C_TypeDef * i2c) 
{
	dpr("STATUS: "); dprhex(i2c->SR1); 
	dln();
	if (i2c->SR1 & I2C_SR1_SB) dprln("STATUS: 0 start bit generated");	
	if (i2c->SR1 & I2C_SR1_ADDR) dprln("STATUS: 1 addr sent/matched");
	if (i2c->SR1 & I2C_SR1_BTF) dprln("STATUS: 2 byte transfer finish");
	if (i2c->SR1 & I2C_SR1_ADD10) dprln("STATUS: 3 10-bit header sent");
	if (i2c->SR1 & I2C_SR1_STOPF) dprln("STATUS: 4 stop detection");
	// 5 - reserved
	if (i2c->SR1 & I2C_SR1_RXNE) dprln("STATUS: 6 0x20 rxne");
	if (i2c->SR1 & I2C_SR1_TXE) dprln("STATUS: 7 txe");
	if (i2c->SR1 & I2C_SR1_BERR) dprln("STATUS: 8 bus error");
	if (i2c->SR1 & I2C_SR1_ARLO) dprln("STATUS: 9 arbitation lost");
	if (i2c->SR1 & I2C_SR1_AF) dprln("STATUS: 10 aknowledge failure");
	if (i2c->SR1 & I2C_SR1_OVR) dprln("STATUS: 11 overrun/ underrun");
	if (i2c->SR1 & I2C_SR1_PECERR) dprln("STATUS: 12 PEC error in reception");
	// 13 - reserved
	if (i2c->SR1 & I2C_SR1_TIMEOUT) dprln("STATUS: 14 timeout or tlow error");
	if (i2c->SR1 & I2C_SR1_SMBALERT) dprln("STATUS: 15 SMBus alert");
}
