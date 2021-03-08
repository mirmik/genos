#ifndef DRIVERS_STM32_I2C_H
#define DRIVERS_STM32_I2C_H

#include <drivers/i2c/i2c_device.h>
#include <periph/map.h>

#include <asm/stm32_i2c.h>

#include <igris/util/bug.h>
#include <igris/sync/semaphore.h>

#include <util/cpu_delay.h>

#define I2C_DUTYCYCLE_2                 0x00000000U
#define I2C_DUTYCYCLE_16_9              I2C_CCR_DUTY

#define I2C_MIN_PCLK_FREQ(__PCLK__, __SPEED__)             (((__SPEED__) <= 100000) ? ((__PCLK__) < I2C_MIN_PCLK_FREQ_STANDARD) : ((__PCLK__) < I2C_MIN_PCLK_FREQ_FAST))
#define I2C_CCR_CALCULATION(__PCLK__, __SPEED__, __COEFF__)     (((((__PCLK__) - 1U)/((__SPEED__) * (__COEFF__))) + 1U) & I2C_CCR_CCR)
#define I2C_FREQRANGE(__PCLK__)                            ((__PCLK__)/1000000U)
#define I2C_RISE_TIME(__FREQRANGE__, __SPEED__)            (((__SPEED__) <= 100000) ? ((__FREQRANGE__) + 1U) : ((((__FREQRANGE__) * 300U) / 1000U) + 1U))
#define I2C_SPEED_STANDARD(__PCLK__, __SPEED__)            ((I2C_CCR_CALCULATION((__PCLK__), (__SPEED__), 2U) < 4U)? 4U:I2C_CCR_CALCULATION((__PCLK__), (__SPEED__), 2U))
#define I2C_SPEED_FAST(__PCLK__, __SPEED__, __DUTYCYCLE__) (((__DUTYCYCLE__) == I2C_DUTYCYCLE_2)? I2C_CCR_CALCULATION((__PCLK__), (__SPEED__), 3U) : (I2C_CCR_CALCULATION((__PCLK__), (__SPEED__), 25U) | I2C_DUTYCYCLE_16_9))
#define I2C_SPEED(__PCLK__, __SPEED__, __DUTYCYCLE__)      (((__SPEED__) <= 100000)? (I2C_SPEED_STANDARD((__PCLK__), (__SPEED__))) : \
                                                                  ((I2C_SPEED_FAST((__PCLK__), (__SPEED__), (__DUTYCYCLE__)) & I2C_CCR_CCR) == 0U)? 1U : \
                                                                  ((I2C_SPEED_FAST((__PCLK__), (__SPEED__), (__DUTYCYCLE__))) | I2C_CCR_FS))
namespace genos
{
	class stm32_i2c_device : public genos::i2c_device
	{
		I2C_TypeDef * regs;
		int irqno_event;
		int irqno_error;

		struct semaphore __lock = SEMAPHORE_INIT(__lock, 1);

	public:
		stm32_i2c_device(
		    I2C_TypeDef * regs,
		    int irqno_event,
		    int irqno_error)
			:
			regs(regs),
			irqno_event(irqno_event),
			irqno_error(irqno_error) {}

		device * parent_device() override { return nullptr; }

		/*int exchange(uint8_t addr,
			const void * txdata, int txsize,
			void * rxdata, int rxsize) override;

		void set_slave_address(uint8_t addr) override;*/

		int write_memory(uint8_t devaddr, uint8_t memaddr, const void * buf, int len) override
		{
			int count = 0;

			uint8_t * cbuf = (uint8_t * ) buf;

			stm32_i2c_set_start(regs);

			// Wait until SB flag is set
			count = 0;
			while (!stm32_i2c_status_start_bit(regs)) 
			{
				if (count++ > 1000)  
				{
					dprln("fault");
					return -1;
				}
			}

			// 7 bit and write(0) command
			stm32_i2c_send_byte(regs, devaddr << 1);

			count = 0;
			while (!stm32_i2c_status_addr(regs)) 
			{
				if (count++ > 1000) 
				{
					dprln("fault");
					return -1;
				}
			}

			// сбрасываем флаг установки адреса.
			char sr1 = regs->SR1;
			char sr2 = regs->SR2;
			(void) sr1; (void) sr2;

			// send memory address
			{
				stm32_i2c_wait_txe_flag(regs);
				stm32_i2c_send_byte(regs, memaddr);
				stm32_i2c_wait_btf_flag(regs);
			}

			// send data
			while (len--)
			{
				stm32_i2c_wait_txe_flag(regs);
				stm32_i2c_send_byte(regs, *cbuf++);
				stm32_i2c_wait_btf_flag(regs);
			}

			stm32_i2c_wait_btf_flag(regs);
			stm32_i2c_set_stop(regs);
			
			return 0;
		}

		int write(uint8_t devaddr, const void * buf, int len) override
		{
			dprln("Write");
			dprdump(buf, len);

			int count = 0;

			uint8_t * cbuf = (uint8_t * ) buf;

			stm32_i2c_set_start(regs);

			// Wait until SB flag is set
			count = 0;
			while (!stm32_i2c_status_start_bit(regs)) 
			{
				if (count++ > 1000)  
				{
					dprln("fault1");
					return -1;
				}
			}

			// 7 bit and write(0) command
			stm32_i2c_send_byte(regs, devaddr << 1);

			count = 0;
			while (!stm32_i2c_status_addr(regs)) 
			{
				if (count++ > 1000) 
				{
					dprln("fault2");
					return -1;
				}
			}

			// сбрасываем флаг установки адреса.
			char sr1 = regs->SR1;
			char sr2 = regs->SR2;
			(void) sr1; (void) sr2;

			// send data
			while (len--)
			{
				stm32_i2c_wait_txe_flag(regs);
				stm32_i2c_send_byte(regs, *cbuf++);
				stm32_i2c_wait_btf_flag(regs);
			}

			stm32_i2c_wait_btf_flag(regs);
			stm32_i2c_set_stop(regs);
			
			return 0;
		}

		int read(uint8_t devaddr, const void * buf, int len) override
		{
			dprln("Read");
			dprdump(buf, len);

			int count = 0;

			uint8_t * cbuf = (uint8_t * ) buf;

			stm32_i2c_set_start(regs);

			// Wait until SB flag is set
			count = 0;
			while (!stm32_i2c_status_start_bit(regs)) 
			{
				if (count++ > 1000)  
				{
					dprln("fault1");
					return -1;
				}
			}

			// 7 bit and write(0) command
			stm32_i2c_send_byte(regs, devaddr << 1 | 1); //read command

			count = 0;
			while (!stm32_i2c_status_addr(regs)) 
			{
				if (count++ > 1000) 
				{
					dprln("fault2");
					return -1;
				}
			}

			// сбрасываем флаг установки адреса.
			char sr1 = regs->SR1;
			char sr2 = regs->SR2;
			(void) sr1; (void) sr2;

			// recv data
			while (len--)
			{
				stm32_i2c_wait_rxne_flag(regs);
				*cbuf++ = stm32_i2c_recv_byte(regs);
				//stm32_i2c_wait_btf_flag(regs);
			}

			//stm32_i2c_wait_btf_flag(regs);
			stm32_i2c_set_stop(regs);
			
			return 0;
		}


		void lock_bus() override { semaphore_down(&__lock); }
		void unlock_bus() override { semaphore_up(&__lock); }

		void begin_master()
		{
			#ifdef STM32G4XX
			BUG();
			#else

			//uint32_t freqrange;
			//uint32_t pclk1;

			stm32_rcc_enable_i2c(regs);
			cpu_delay(100);
			regs->CR1 &= ~I2C_CR1_PE; // disable

			regs->CR1 |= I2C_CR1_SWRST;  // software reset 
			regs->CR1 &= ~I2C_CR1_SWRST; // software reset
			cpu_delay(100);


			//MODIFY_REG(hi2c->Instance->CR2, I2C_CR2_FREQ, freqrange);
			//bits_assign(regs->CR2, 0b11111, 0b00010); 
			bits_assign(regs->CR2, 0b11111, 8); 

			/*---------------------------- I2Cx TRISE Configuration --------------------*/
			/* Configure I2Cx: Rise Time */
			//MODIFY_REG(hi2c->Instance->TRISE, I2C_TRISE_TRISE, I2C_RISE_TIME(freqrange, hi2c->Init.ClockSpeed));
			bits_assign(regs->TRISE, I2C_TRISE_TRISE, 9);
			regs->CCR = 40;

			/*---------------------------- I2Cx CCR Configuration ----------------------*/
			/* Configure I2Cx: Speed */
			//uint16_t _I2C_SPEED = I2C_SPEED(pclk1, clock_speed, duty_cycle);
			//bits_assign(
			//	regs->CCR, 
			//	(I2C_CCR_FS | I2C_CCR_DUTY | I2C_CCR_CCR), 
			//	_I2C_SPEED);

			/*---------------------------- I2Cx CR1 Configuration ----------------------*/
			/* Configure I2Cx: Generalcall and NoStretch mode */
			//MODIFY_REG(hi2c->Instance->CR1, (I2C_CR1_ENGC | I2C_CR1_NOSTRETCH), (hi2c->Init.GeneralCallMode | hi2c->Init.NoStretchMode));

			/*---------------------------- I2Cx OAR1 Configuration ---------------------*/
			/* Configure I2Cx: Own Address1 and addressing mode */
			//MODIFY_REG(hi2c->Instance->OAR1, (I2C_OAR1_ADDMODE | I2C_OAR1_ADD8_9 | I2C_OAR1_ADD1_7 | I2C_OAR1_ADD0), (hi2c->Init.AddressingMode | hi2c->Init.OwnAddress1));

			/*---------------------------- I2Cx OAR2 Configuration ---------------------*/
			/* Configure I2Cx: Dual mode and Own Address2 */
			//MODIFY_REG(hi2c->Instance->OAR2, (I2C_OAR2_ENDUAL | I2C_OAR2_ADD2), (hi2c->Init.DualAddressMode | hi2c->Init.OwnAddress2));


			cpu_delay(100);
			regs->CR1 |= I2C_CR1_PE;
			#endif
		}
	};
}

#endif