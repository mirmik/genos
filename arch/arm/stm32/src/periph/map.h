#ifndef GENOS_STM32F4XX_REGS_MAP
#define GENOS_STM32F4XX_REGS_MAP

#include <periph/regs/gpio.h>
#include <periph/regs/usart.h>
#include <periph/regs/spi.h>
#include <periph/regs/timer.h>
#include <periph/regs/rcc.h>
#include <periph/regs/pwr.h>
#include <periph/regs/flash.h>
#include <periph/regs/syscfg.h>
#include <periph/regs/exti.h>

#include <periph/coremap.h>

#define FLASH_BASE            ((uint32_t)0x08000000) /*!< FLASH(up to 1 MB) base address in the alias region                         */
#define CCMDATARAM_BASE       ((uint32_t)0x10000000) /*!< CCM(core coupled memory) data RAM(64 KB) base address in the alias region  */
#define SRAM1_BASE            ((uint32_t)0x20000000) /*!< SRAM1(112 KB) base address in the alias region                             */
#define SRAM2_BASE            ((uint32_t)0x2001C000) /*!< SRAM2(16 KB) base address in the alias region                              */
#define SRAM3_BASE            ((uint32_t)0x20020000) /*!< SRAM3(64 KB) base address in the alias region                              */
#define PERIPH_BASE           ((uint32_t)0x40000000) /*!< Peripheral base address in the alias region                                */
#define BKPSRAM_BASE          ((uint32_t)0x40024000) /*!< Backup SRAM(4 KB) base address in the alias region                         */

#if defined (STM32F40_41xxx)
#define FSMC_R_BASE           ((uint32_t)0xA0000000) /*!< FSMC registers base address                                                */
#endif /* STM32F40_41xxx */

#if defined (STM32F427_437xx) || defined (STM32F429_439xx)
#define FMC_R_BASE            ((uint32_t)0xA0000000) /*!< FMC registers base address                                                 */
#endif /* STM32F427_437xx ||  STM32F429_439xx */

#define CCMDATARAM_BB_BASE    ((uint32_t)0x12000000) /*!< CCM(core coupled memory) data RAM(64 KB) base address in the bit-band region  */
#define SRAM1_BB_BASE         ((uint32_t)0x22000000) /*!< SRAM1(112 KB) base address in the bit-band region                             */
#define SRAM2_BB_BASE         ((uint32_t)0x2201C000) /*!< SRAM2(16 KB) base address in the bit-band region                              */
#define SRAM3_BB_BASE         ((uint32_t)0x22400000) /*!< SRAM3(64 KB) base address in the bit-band region                              */
#define PERIPH_BB_BASE        ((uint32_t)0x42000000) /*!< Peripheral base address in the bit-band region                                */
#define BKPSRAM_BB_BASE       ((uint32_t)0x42024000) /*!< Backup SRAM(4 KB) base address in the bit-band region                         */

/* Legacy defines */
#define SRAM_BASE             SRAM1_BASE
#define SRAM_BB_BASE          SRAM1_BB_BASE

/*!< Peripheral memory map */
#define APB1PERIPH_BASE       PERIPH_BASE
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x00010000)
#define AHB1PERIPH_BASE       (PERIPH_BASE + 0x00020000)
#define AHB2PERIPH_BASE       (PERIPH_BASE + 0x10000000)

/*!< APB1 peripherals */
#define TIM2_BASE             (APB1PERIPH_BASE + 0x0000)
#define TIM3_BASE             (APB1PERIPH_BASE + 0x0400)
#define TIM4_BASE             (APB1PERIPH_BASE + 0x0800)
#define TIM5_BASE             (APB1PERIPH_BASE + 0x0C00)
#define TIM6_BASE             (APB1PERIPH_BASE + 0x1000)
#define TIM7_BASE             (APB1PERIPH_BASE + 0x1400)
#define TIM12_BASE            (APB1PERIPH_BASE + 0x1800)
#define TIM13_BASE            (APB1PERIPH_BASE + 0x1C00)
#define TIM14_BASE            (APB1PERIPH_BASE + 0x2000)
#define RTC_BASE              (APB1PERIPH_BASE + 0x2800)
#define WWDG_BASE             (APB1PERIPH_BASE + 0x2C00)
#define IWDG_BASE             (APB1PERIPH_BASE + 0x3000)
#define I2S2ext_BASE          (APB1PERIPH_BASE + 0x3400)
#define SPI2_BASE             (APB1PERIPH_BASE + 0x3800)
#define SPI3_BASE             (APB1PERIPH_BASE + 0x3C00)
#define I2S3ext_BASE          (APB1PERIPH_BASE + 0x4000)
#define USART2_BASE           (APB1PERIPH_BASE + 0x4400)
#define USART3_BASE           (APB1PERIPH_BASE + 0x4800)
#define UART4_BASE            (APB1PERIPH_BASE + 0x4C00)
#define UART5_BASE            (APB1PERIPH_BASE + 0x5000)
#define I2C1_BASE             (APB1PERIPH_BASE + 0x5400)
#define I2C2_BASE             (APB1PERIPH_BASE + 0x5800)
#define I2C3_BASE             (APB1PERIPH_BASE + 0x5C00)
#define CAN1_BASE             (APB1PERIPH_BASE + 0x6400)
#define CAN2_BASE             (APB1PERIPH_BASE + 0x6800)
#define PWR_BASE              (APB1PERIPH_BASE + 0x7000)
#define DAC_BASE              (APB1PERIPH_BASE + 0x7400)
#define UART7_BASE            (APB1PERIPH_BASE + 0x7800)
#define UART8_BASE            (APB1PERIPH_BASE + 0x7C00)

/*!< APB2 peripherals */
#define TIM1_BASE             (APB2PERIPH_BASE + 0x0000)
#define TIM8_BASE             (APB2PERIPH_BASE + 0x0400)
#define USART1_BASE           (APB2PERIPH_BASE + 0x1000)
#define USART6_BASE           (APB2PERIPH_BASE + 0x1400)
#define ADC1_BASE             (APB2PERIPH_BASE + 0x2000)
#define ADC2_BASE             (APB2PERIPH_BASE + 0x2100)
#define ADC3_BASE             (APB2PERIPH_BASE + 0x2200)
#define ADC_BASE              (APB2PERIPH_BASE + 0x2300)
#define SDIO_BASE             (APB2PERIPH_BASE + 0x2C00)
#define SPI1_BASE             (APB2PERIPH_BASE + 0x3000)
#define SPI4_BASE             (APB2PERIPH_BASE + 0x3400)
#define SYSCFG_BASE           (APB2PERIPH_BASE + 0x3800)
#define EXTI_BASE             (APB2PERIPH_BASE + 0x3C00)
#define TIM9_BASE             (APB2PERIPH_BASE + 0x4000)
#define TIM10_BASE            (APB2PERIPH_BASE + 0x4400)
#define TIM11_BASE            (APB2PERIPH_BASE + 0x4800)
#define SPI5_BASE             (APB2PERIPH_BASE + 0x5000)
#define SPI6_BASE             (APB2PERIPH_BASE + 0x5400)
#define SAI1_BASE             (APB2PERIPH_BASE + 0x5800)
#define SAI1_Block_A_BASE     (SAI1_BASE + 0x004)
#define SAI1_Block_B_BASE     (SAI1_BASE + 0x024)
#define LTDC_BASE             (APB2PERIPH_BASE + 0x6800)
#define LTDC_Layer1_BASE      (LTDC_BASE + 0x84)
#define LTDC_Layer2_BASE      (LTDC_BASE + 0x104) 

/*!< AHB1 peripherals */
#define GPIOA_BASE            (AHB1PERIPH_BASE + 0x0000)
#define GPIOB_BASE            (AHB1PERIPH_BASE + 0x0400)
#define GPIOC_BASE            (AHB1PERIPH_BASE + 0x0800)
#define GPIOD_BASE            (AHB1PERIPH_BASE + 0x0C00)
#define GPIOE_BASE            (AHB1PERIPH_BASE + 0x1000)
#define GPIOF_BASE            (AHB1PERIPH_BASE + 0x1400)
#define GPIOG_BASE            (AHB1PERIPH_BASE + 0x1800)
#define GPIOH_BASE            (AHB1PERIPH_BASE + 0x1C00)
#define GPIOI_BASE            (AHB1PERIPH_BASE + 0x2000)
#define GPIOJ_BASE            (AHB1PERIPH_BASE + 0x2400)
#define GPIOK_BASE            (AHB1PERIPH_BASE + 0x2800)
#define CRC_BASE              (AHB1PERIPH_BASE + 0x3000)
#define RCC_BASE              (AHB1PERIPH_BASE + 0x3800)
#define FLASH_R_BASE          (AHB1PERIPH_BASE + 0x3C00)
#define DMA1_BASE             (AHB1PERIPH_BASE + 0x6000)
#define DMA1_Stream0_BASE     (DMA1_BASE + 0x010)
#define DMA1_Stream1_BASE     (DMA1_BASE + 0x028)
#define DMA1_Stream2_BASE     (DMA1_BASE + 0x040)
#define DMA1_Stream3_BASE     (DMA1_BASE + 0x058)
#define DMA1_Stream4_BASE     (DMA1_BASE + 0x070)
#define DMA1_Stream5_BASE     (DMA1_BASE + 0x088)
#define DMA1_Stream6_BASE     (DMA1_BASE + 0x0A0)
#define DMA1_Stream7_BASE     (DMA1_BASE + 0x0B8)
#define DMA2_BASE             (AHB1PERIPH_BASE + 0x6400)
#define DMA2_Stream0_BASE     (DMA2_BASE + 0x010)
#define DMA2_Stream1_BASE     (DMA2_BASE + 0x028)
#define DMA2_Stream2_BASE     (DMA2_BASE + 0x040)
#define DMA2_Stream3_BASE     (DMA2_BASE + 0x058)
#define DMA2_Stream4_BASE     (DMA2_BASE + 0x070)
#define DMA2_Stream5_BASE     (DMA2_BASE + 0x088)
#define DMA2_Stream6_BASE     (DMA2_BASE + 0x0A0)
#define DMA2_Stream7_BASE     (DMA2_BASE + 0x0B8)
#define ETH_BASE              (AHB1PERIPH_BASE + 0x8000)
#define ETH_MAC_BASE          (ETH_BASE)
#define ETH_MMC_BASE          (ETH_BASE + 0x0100)
#define ETH_PTP_BASE          (ETH_BASE + 0x0700)
#define ETH_DMA_BASE          (ETH_BASE + 0x1000)
#define DMA2D_BASE            (AHB1PERIPH_BASE + 0xB000)

/*!< AHB2 peripherals */
#define DCMI_BASE             (AHB2PERIPH_BASE + 0x50000)
#define CRYP_BASE             (AHB2PERIPH_BASE + 0x60000)
#define HASH_BASE             (AHB2PERIPH_BASE + 0x60400)
#define HASH_DIGEST_BASE      (AHB2PERIPH_BASE + 0x60710)
#define RNG_BASE              (AHB2PERIPH_BASE + 0x60800)

#define RCC ((struct rcc_regs *) RCC_BASE)

#define GPIOA ((struct gpio_regs *) GPIOA_BASE)
#define GPIOB ((struct gpio_regs *) GPIOB_BASE)
#define GPIOC ((struct gpio_regs *) GPIOC_BASE)
#define GPIOD ((struct gpio_regs *) GPIOD_BASE)
#define GPIOE ((struct gpio_regs *) GPIOE_BASE)
#define GPIOF ((struct gpio_regs *) GPIOF_BASE)
#define GPIOG ((struct gpio_regs *) GPIOG_BASE)
#define GPIOH ((struct gpio_regs *) GPIOH_BASE)
#define GPIOI ((struct gpio_regs *) GPIOI_BASE)
#define GPIOJ ((struct gpio_regs *) GPIOJ_BASE)
#define GPIOK ((struct gpio_regs *) GPIOK_BASE)

#define SPI1 ((struct spi_regs *) SPI1_BASE) 
#define SPI2 ((struct spi_regs *) SPI2_BASE) 
#define SPI3 ((struct spi_regs *) SPI3_BASE) 
#define SPI4 ((struct spi_regs *) SPI4_BASE)
#define SPI5 ((struct spi_regs *) SPI5_BASE)
#define SPI6 ((struct spi_regs *) SPI6_BASE)

#define USART1 ((struct usart_regs *) USART1_BASE) 
#define USART2 ((struct usart_regs *) USART2_BASE)
#define USART3 ((struct usart_regs *) USART3_BASE)
#define USART6 ((struct usart_regs *) USART6_BASE)

#define TIM1 ((struct timer_regs *) TIM1_BASE)
#define TIM2 ((struct timer_regs *) TIM2_BASE)
#define TIM3 ((struct timer_regs *) TIM3_BASE)
#define TIM4 ((struct timer_regs *) TIM4_BASE)
#define TIM5 ((struct timer_regs *) TIM5_BASE)
#define TIM6 ((struct timer_regs *) TIM6_BASE)
#define TIM7 ((struct timer_regs *) TIM7_BASE)
#define TIM8 ((struct timer_regs *) TIM8_BASE)
#define TIM9 ((struct timer_regs *) TIM9_BASE)
#define TIM10 ((struct timer_regs *) TIM10_BASE)
#define TIM11 ((struct timer_regs *) TIM11_BASE)
#define TIM12 ((struct timer_regs *) TIM12_BASE)
#define TIM13 ((struct timer_regs *) TIM13_BASE)
#define TIM14 ((struct timer_regs *) TIM14_BASE)

#define ADC   ((struct stm32_adc_common_regs *) ADC_BASE)
#define ADC1  ((struct stm32_adc_regs *) ADC1_BASE)
#define ADC2  ((struct stm32_adc_regs *) ADC2_BASE)
#define ADC3  ((struct stm32_adc_regs *) ADC3_BASE)

#define FLASH ((struct flash_regs *) FLASH_R_BASE)
#define PWR ((struct pwr_regs *) PWR_BASE)
#define SYSCFG ((struct syscfg_regs *) SYSCFG_BASE)
#define EXTI ((struct exti_regs *) EXTI_BASE)









///**
//  * @}
//  */
//  
///** @addtogroup Peripheral_memory_map
//  * @{
//  */
//#define FLASH_BASE            ((uint32_t)0x08000000) /*!< FLASH(up to 1 MB) base address in the alias region                         */
//#define CCMDATARAM_BASE       ((uint32_t)0x10000000) /*!< CCM(core coupled memory) data RAM(64 KB) base address in the alias region  */
//#define SRAM1_BASE            ((uint32_t)0x20000000) /*!< SRAM1(112 KB) base address in the alias region                             */
//#define SRAM2_BASE            ((uint32_t)0x2001C000) /*!< SRAM2(16 KB) base address in the alias region                              */
//#define SRAM3_BASE            ((uint32_t)0x20020000) /*!< SRAM3(64 KB) base address in the alias region                              */
//#define PERIPH_BASE           ((uint32_t)0x40000000) /*!< Peripheral base address in the alias region                                */
//#define BKPSRAM_BASE          ((uint32_t)0x40024000) /*!< Backup SRAM(4 KB) base address in the alias region                         */
//
//#if defined (STM32F40_41xxx)
//#define FSMC_R_BASE           ((uint32_t)0xA0000000) /*!< FSMC registers base address                                                */
//#endif /* STM32F40_41xxx */
//
//#if defined (STM32F427_437xx) || defined (STM32F429_439xx)
//#define FMC_R_BASE            ((uint32_t)0xA0000000) /*!< FMC registers base address                                                 */
//#endif /* STM32F427_437xx ||  STM32F429_439xx */
//
//#define CCMDATARAM_BB_BASE    ((uint32_t)0x12000000) /*!< CCM(core coupled memory) data RAM(64 KB) base address in the bit-band region  */
//#define SRAM1_BB_BASE         ((uint32_t)0x22000000) /*!< SRAM1(112 KB) base address in the bit-band region                             */
//#define SRAM2_BB_BASE         ((uint32_t)0x2201C000) /*!< SRAM2(16 KB) base address in the bit-band region                              */
//#define SRAM3_BB_BASE         ((uint32_t)0x22400000) /*!< SRAM3(64 KB) base address in the bit-band region                              */
//#define PERIPH_BB_BASE        ((uint32_t)0x42000000) /*!< Peripheral base address in the bit-band region                                */
//#define BKPSRAM_BB_BASE       ((uint32_t)0x42024000) /*!< Backup SRAM(4 KB) base address in the bit-band region                         */
//
///* Legacy defines */
//#define SRAM_BASE             SRAM1_BASE
//#define SRAM_BB_BASE          SRAM1_BB_BASE
//
//
///*!< Peripheral memory map */
//#define APB1PERIPH_BASE       PERIPH_BASE
//#define APB2PERIPH_BASE       (PERIPH_BASE + 0x00010000)
//#define AHB1PERIPH_BASE       (PERIPH_BASE + 0x00020000)
//#define AHB2PERIPH_BASE       (PERIPH_BASE + 0x10000000)
//
///*!< APB1 peripherals */
//#define TIM2_BASE             (APB1PERIPH_BASE + 0x0000)
//#define TIM3_BASE             (APB1PERIPH_BASE + 0x0400)
//#define TIM4_BASE             (APB1PERIPH_BASE + 0x0800)
//#define TIM5_BASE             (APB1PERIPH_BASE + 0x0C00)
//#define TIM6_BASE             (APB1PERIPH_BASE + 0x1000)
//#define TIM7_BASE             (APB1PERIPH_BASE + 0x1400)
//#define TIM12_BASE            (APB1PERIPH_BASE + 0x1800)
//#define TIM13_BASE            (APB1PERIPH_BASE + 0x1C00)
//#define TIM14_BASE            (APB1PERIPH_BASE + 0x2000)
//#define RTC_BASE              (APB1PERIPH_BASE + 0x2800)
//#define WWDG_BASE             (APB1PERIPH_BASE + 0x2C00)
//#define IWDG_BASE             (APB1PERIPH_BASE + 0x3000)
//#define I2S2ext_BASE          (APB1PERIPH_BASE + 0x3400)
//#define SPI2_BASE             (APB1PERIPH_BASE + 0x3800)
//#define SPI3_BASE             (APB1PERIPH_BASE + 0x3C00)
//#define I2S3ext_BASE          (APB1PERIPH_BASE + 0x4000)
//#define USART2_BASE           (APB1PERIPH_BASE + 0x4400)
//#define USART3_BASE           (APB1PERIPH_BASE + 0x4800)
//#define UART4_BASE            (APB1PERIPH_BASE + 0x4C00)
//#define UART5_BASE            (APB1PERIPH_BASE + 0x5000)
//#define I2C1_BASE             (APB1PERIPH_BASE + 0x5400)
//#define I2C2_BASE             (APB1PERIPH_BASE + 0x5800)
//#define I2C3_BASE             (APB1PERIPH_BASE + 0x5C00)
//#define CAN1_BASE             (APB1PERIPH_BASE + 0x6400)
//#define CAN2_BASE             (APB1PERIPH_BASE + 0x6800)
//#define PWR_BASE              (APB1PERIPH_BASE + 0x7000)
//#define DAC_BASE              (APB1PERIPH_BASE + 0x7400)
//#define UART7_BASE            (APB1PERIPH_BASE + 0x7800)
//#define UART8_BASE            (APB1PERIPH_BASE + 0x7C00)
//
///*!< APB2 peripherals */
//#define TIM1_BASE             (APB2PERIPH_BASE + 0x0000)
//#define TIM8_BASE             (APB2PERIPH_BASE + 0x0400)
//#define USART1_BASE           (APB2PERIPH_BASE + 0x1000)
//#define USART6_BASE           (APB2PERIPH_BASE + 0x1400)
//#define ADC1_BASE             (APB2PERIPH_BASE + 0x2000)
//#define ADC2_BASE             (APB2PERIPH_BASE + 0x2100)
//#define ADC3_BASE             (APB2PERIPH_BASE + 0x2200)
//#define ADC_BASE              (APB2PERIPH_BASE + 0x2300)
//#define SDIO_BASE             (APB2PERIPH_BASE + 0x2C00)
//#define SPI1_BASE             (APB2PERIPH_BASE + 0x3000)
//#define SPI4_BASE             (APB2PERIPH_BASE + 0x3400)
//#define SYSCFG_BASE           (APB2PERIPH_BASE + 0x3800)
//#define EXTI_BASE             (APB2PERIPH_BASE + 0x3C00)
//#define TIM9_BASE             (APB2PERIPH_BASE + 0x4000)
//#define TIM10_BASE            (APB2PERIPH_BASE + 0x4400)
//#define TIM11_BASE            (APB2PERIPH_BASE + 0x4800)
//#define SPI5_BASE             (APB2PERIPH_BASE + 0x5000)
//#define SPI6_BASE             (APB2PERIPH_BASE + 0x5400)
//#define SAI1_BASE             (APB2PERIPH_BASE + 0x5800)
//#define SAI1_Block_A_BASE     (SAI1_BASE + 0x004)
//#define SAI1_Block_B_BASE     (SAI1_BASE + 0x024)
//#define LTDC_BASE             (APB2PERIPH_BASE + 0x6800)
//#define LTDC_Layer1_BASE      (LTDC_BASE + 0x84)
//#define LTDC_Layer2_BASE      (LTDC_BASE + 0x104) 
//
///*!< AHB1 peripherals */
//#define GPIOA_BASE            (AHB1PERIPH_BASE + 0x0000)
//#define GPIOB_BASE            (AHB1PERIPH_BASE + 0x0400)
//#define GPIOC_BASE            (AHB1PERIPH_BASE + 0x0800)
//#define GPIOD_BASE            (AHB1PERIPH_BASE + 0x0C00)
//#define GPIOE_BASE            (AHB1PERIPH_BASE + 0x1000)
//#define GPIOF_BASE            (AHB1PERIPH_BASE + 0x1400)
//#define GPIOG_BASE            (AHB1PERIPH_BASE + 0x1800)
//#define GPIOH_BASE            (AHB1PERIPH_BASE + 0x1C00)
//#define GPIOI_BASE            (AHB1PERIPH_BASE + 0x2000)
//#define GPIOJ_BASE            (AHB1PERIPH_BASE + 0x2400)
//#define GPIOK_BASE            (AHB1PERIPH_BASE + 0x2800)
//#define CRC_BASE              (AHB1PERIPH_BASE + 0x3000)
//#define RCC_BASE              (AHB1PERIPH_BASE + 0x3800)
//#define FLASH_R_BASE          (AHB1PERIPH_BASE + 0x3C00)
//#define DMA1_BASE             (AHB1PERIPH_BASE + 0x6000)
//#define DMA1_Stream0_BASE     (DMA1_BASE + 0x010)
//#define DMA1_Stream1_BASE     (DMA1_BASE + 0x028)
//#define DMA1_Stream2_BASE     (DMA1_BASE + 0x040)
//#define DMA1_Stream3_BASE     (DMA1_BASE + 0x058)
//#define DMA1_Stream4_BASE     (DMA1_BASE + 0x070)
//#define DMA1_Stream5_BASE     (DMA1_BASE + 0x088)
//#define DMA1_Stream6_BASE     (DMA1_BASE + 0x0A0)
//#define DMA1_Stream7_BASE     (DMA1_BASE + 0x0B8)
//#define DMA2_BASE             (AHB1PERIPH_BASE + 0x6400)
//#define DMA2_Stream0_BASE     (DMA2_BASE + 0x010)
//#define DMA2_Stream1_BASE     (DMA2_BASE + 0x028)
//#define DMA2_Stream2_BASE     (DMA2_BASE + 0x040)
//#define DMA2_Stream3_BASE     (DMA2_BASE + 0x058)
//#define DMA2_Stream4_BASE     (DMA2_BASE + 0x070)
//#define DMA2_Stream5_BASE     (DMA2_BASE + 0x088)
//#define DMA2_Stream6_BASE     (DMA2_BASE + 0x0A0)
//#define DMA2_Stream7_BASE     (DMA2_BASE + 0x0B8)
//#define ETH_BASE              (AHB1PERIPH_BASE + 0x8000)
//#define ETH_MAC_BASE          (ETH_BASE)
//#define ETH_MMC_BASE          (ETH_BASE + 0x0100)
//#define ETH_PTP_BASE          (ETH_BASE + 0x0700)
//#define ETH_DMA_BASE          (ETH_BASE + 0x1000)
//#define DMA2D_BASE            (AHB1PERIPH_BASE + 0xB000)
//
///*!< AHB2 peripherals */
//#define DCMI_BASE             (AHB2PERIPH_BASE + 0x50000)
//#define CRYP_BASE             (AHB2PERIPH_BASE + 0x60000)
//#define HASH_BASE             (AHB2PERIPH_BASE + 0x60400)
//#define HASH_DIGEST_BASE      (AHB2PERIPH_BASE + 0x60710)
//#define RNG_BASE              (AHB2PERIPH_BASE + 0x60800)
//
//#if defined (STM32F40_41xxx)
///*!< FSMC Bankx registers base address */
//#define FSMC_Bank1_R_BASE     (FSMC_R_BASE + 0x0000)
//#define FSMC_Bank1E_R_BASE    (FSMC_R_BASE + 0x0104)
//#define FSMC_Bank2_R_BASE     (FSMC_R_BASE + 0x0060)
//#define FSMC_Bank3_R_BASE     (FSMC_R_BASE + 0x0080)
//#define FSMC_Bank4_R_BASE     (FSMC_R_BASE + 0x00A0)
//#endif /* STM32F40_41xxx */
//
//#if defined (STM32F427_437xx) || defined (STM32F429_439xx)
///*!< FMC Bankx registers base address */
//#define FMC_Bank1_R_BASE      (FMC_R_BASE + 0x0000)
//#define FMC_Bank1E_R_BASE     (FMC_R_BASE + 0x0104)
//#define FMC_Bank2_R_BASE      (FMC_R_BASE + 0x0060)
//#define FMC_Bank3_R_BASE      (FMC_R_BASE + 0x0080)
//#define FMC_Bank4_R_BASE      (FMC_R_BASE + 0x00A0)
//#define FMC_Bank5_6_R_BASE    (FMC_R_BASE + 0x0140)
//#endif /* STM32F427_437xx ||  STM32F429_439xx */
//
///* Debug MCU registers base address */
//#define DBGMCU_BASE           ((uint32_t )0xE0042000)
//
///**
//  * @}
//  */
//  
///** @addtogroup Peripheral_declaration
//  * @{
//  */  
//#define TIM2                ((TIM_TypeDef *) TIM2_BASE)
//#define TIM3                ((TIM_TypeDef *) TIM3_BASE)
//#define TIM4                ((TIM_TypeDef *) TIM4_BASE)
//#define TIM5                ((TIM_TypeDef *) TIM5_BASE)
//#define TIM6                ((TIM_TypeDef *) TIM6_BASE)
//#define TIM7                ((TIM_TypeDef *) TIM7_BASE)
//#define TIM12               ((TIM_TypeDef *) TIM12_BASE)
//#define TIM13               ((TIM_TypeDef *) TIM13_BASE)
//#define TIM14               ((TIM_TypeDef *) TIM14_BASE)
//#define RTC                 ((RTC_TypeDef *) RTC_BASE)
//#define WWDG                ((WWDG_TypeDef *) WWDG_BASE)
//#define IWDG                ((IWDG_TypeDef *) IWDG_BASE)
//#define I2S2ext             ((SPI_TypeDef *) I2S2ext_BASE)
//#define SPI2                ((SPI_TypeDef *) SPI2_BASE)
//#define SPI3                ((SPI_TypeDef *) SPI3_BASE)
//#define I2S3ext             ((SPI_TypeDef *) I2S3ext_BASE)
//#define USART2              ((USART_TypeDef *) USART2_BASE)
//#define USART3              ((USART_TypeDef *) USART3_BASE)
//#define UART4               ((USART_TypeDef *) UART4_BASE)
//#define UART5               ((USART_TypeDef *) UART5_BASE)
//#define I2C1                ((I2C_TypeDef *) I2C1_BASE)
//#define I2C2                ((I2C_TypeDef *) I2C2_BASE)
//#define I2C3                ((I2C_TypeDef *) I2C3_BASE)
//#define CAN1                ((CAN_TypeDef *) CAN1_BASE)
//#define CAN2                ((CAN_TypeDef *) CAN2_BASE)
//#define PWR                 ((PWR_TypeDef *) PWR_BASE)
//#define DAC                 ((DAC_TypeDef *) DAC_BASE)
//#define UART7               ((USART_TypeDef *) UART7_BASE)
//#define UART8               ((USART_TypeDef *) UART8_BASE)
//#define TIM1                ((TIM_TypeDef *) TIM1_BASE)
//#define TIM8                ((TIM_TypeDef *) TIM8_BASE)
//#define USART1              ((USART_TypeDef *) USART1_BASE)
//#define USART6              ((USART_TypeDef *) USART6_BASE)
//#define ADC                 ((ADC_Common_TypeDef *) ADC_BASE)
//#define ADC1                ((ADC_TypeDef *) ADC1_BASE)
//#define ADC2                ((ADC_TypeDef *) ADC2_BASE)
//#define ADC3                ((ADC_TypeDef *) ADC3_BASE)
//#define SDIO                ((SDIO_TypeDef *) SDIO_BASE)
//#define SPI1                ((SPI_TypeDef *) SPI1_BASE) 
//#define SPI4                ((SPI_TypeDef *) SPI4_BASE)
//#define SYSCFG              ((SYSCFG_TypeDef *) SYSCFG_BASE)
//#define EXTI                ((EXTI_TypeDef *) EXTI_BASE)
//#define TIM9                ((TIM_TypeDef *) TIM9_BASE)
//#define TIM10               ((TIM_TypeDef *) TIM10_BASE)
//#define TIM11               ((TIM_TypeDef *) TIM11_BASE)
//#define SPI5                ((SPI_TypeDef *) SPI5_BASE)
//#define SPI6                ((SPI_TypeDef *) SPI6_BASE)
//#define SAI1                ((SAI_TypeDef *) SAI1_BASE)
//#define SAI1_Block_A        ((SAI_Block_TypeDef *)SAI1_Block_A_BASE)
//#define SAI1_Block_B        ((SAI_Block_TypeDef *)SAI1_Block_B_BASE)
//#define LTDC                ((LTDC_TypeDef *)LTDC_BASE)
//#define LTDC_Layer1         ((LTDC_Layer_TypeDef *)LTDC_Layer1_BASE)
//#define LTDC_Layer2         ((LTDC_Layer_TypeDef *)LTDC_Layer2_BASE)
//#define GPIOA               ((GPIO_TypeDef *) GPIOA_BASE)
//#define GPIOB               ((GPIO_TypeDef *) GPIOB_BASE)
//#define GPIOC               ((GPIO_TypeDef *) GPIOC_BASE)
//#define GPIOD               ((GPIO_TypeDef *) GPIOD_BASE)
//#define GPIOE               ((GPIO_TypeDef *) GPIOE_BASE)
//#define GPIOF               ((GPIO_TypeDef *) GPIOF_BASE)
//#define GPIOG               ((GPIO_TypeDef *) GPIOG_BASE)
//#define GPIOH               ((GPIO_TypeDef *) GPIOH_BASE)
//#define GPIOI               ((GPIO_TypeDef *) GPIOI_BASE)
//#define GPIOJ               ((GPIO_TypeDef *) GPIOJ_BASE)
//#define GPIOK               ((GPIO_TypeDef *) GPIOK_BASE)
//#define CRC                 ((CRC_TypeDef *) CRC_BASE)
//#define RCC                 ((RCC_TypeDef *) RCC_BASE)
//#define FLASH               ((FLASH_TypeDef *) FLASH_R_BASE)
//#define DMA1                ((DMA_TypeDef *) DMA1_BASE)
//#define DMA1_Stream0        ((DMA_Stream_TypeDef *) DMA1_Stream0_BASE)
//#define DMA1_Stream1        ((DMA_Stream_TypeDef *) DMA1_Stream1_BASE)
//#define DMA1_Stream2        ((DMA_Stream_TypeDef *) DMA1_Stream2_BASE)
//#define DMA1_Stream3        ((DMA_Stream_TypeDef *) DMA1_Stream3_BASE)
//#define DMA1_Stream4        ((DMA_Stream_TypeDef *) DMA1_Stream4_BASE)
//#define DMA1_Stream5        ((DMA_Stream_TypeDef *) DMA1_Stream5_BASE)
//#define DMA1_Stream6        ((DMA_Stream_TypeDef *) DMA1_Stream6_BASE)
//#define DMA1_Stream7        ((DMA_Stream_TypeDef *) DMA1_Stream7_BASE)
//#define DMA2                ((DMA_TypeDef *) DMA2_BASE)
//#define DMA2_Stream0        ((DMA_Stream_TypeDef *) DMA2_Stream0_BASE)
//#define DMA2_Stream1        ((DMA_Stream_TypeDef *) DMA2_Stream1_BASE)
//#define DMA2_Stream2        ((DMA_Stream_TypeDef *) DMA2_Stream2_BASE)
//#define DMA2_Stream3        ((DMA_Stream_TypeDef *) DMA2_Stream3_BASE)
//#define DMA2_Stream4        ((DMA_Stream_TypeDef *) DMA2_Stream4_BASE)
//#define DMA2_Stream5        ((DMA_Stream_TypeDef *) DMA2_Stream5_BASE)
//#define DMA2_Stream6        ((DMA_Stream_TypeDef *) DMA2_Stream6_BASE)
//#define DMA2_Stream7        ((DMA_Stream_TypeDef *) DMA2_Stream7_BASE)
//#define ETH                 ((ETH_TypeDef *) ETH_BASE)  
//#define DMA2D               ((DMA2D_TypeDef *)DMA2D_BASE)
//#define DCMI                ((DCMI_TypeDef *) DCMI_BASE)
//#define CRYP                ((CRYP_TypeDef *) CRYP_BASE)
//#define HASH                ((HASH_TypeDef *) HASH_BASE)
//#define HASH_DIGEST         ((HASH_DIGEST_TypeDef *) HASH_DIGEST_BASE)
//#define RNG                 ((RNG_TypeDef *) RNG_BASE)
//
//#if defined (STM32F40_41xxx)
//#define FSMC_Bank1          ((FSMC_Bank1_TypeDef *) FSMC_Bank1_R_BASE)
//#define FSMC_Bank1E         ((FSMC_Bank1E_TypeDef *) FSMC_Bank1E_R_BASE)
//#define FSMC_Bank2          ((FSMC_Bank2_TypeDef *) FSMC_Bank2_R_BASE)
//#define FSMC_Bank3          ((FSMC_Bank3_TypeDef *) FSMC_Bank3_R_BASE)
//#define FSMC_Bank4          ((FSMC_Bank4_TypeDef *) FSMC_Bank4_R_BASE)
//#endif /* STM32F40_41xxx */
//
//#if defined (STM32F427_437xx) || defined (STM32F429_439xx)
//#define FMC_Bank1           ((FMC_Bank1_TypeDef *) FMC_Bank1_R_BASE)
//#define FMC_Bank1E          ((FMC_Bank1E_TypeDef *) FMC_Bank1E_R_BASE)
//#define FMC_Bank2           ((FMC_Bank2_TypeDef *) FMC_Bank2_R_BASE)
//#define FMC_Bank3           ((FMC_Bank3_TypeDef *) FMC_Bank3_R_BASE)
//#define FMC_Bank4           ((FMC_Bank4_TypeDef *) FMC_Bank4_R_BASE)
//#define FMC_Bank5_6         ((FMC_Bank5_6_TypeDef *) FMC_Bank5_6_R_BASE)
//#endif /* STM32F427_437xx ||  STM32F429_439xx */
//
//#define DBGMCU              ((DBGMCU_TypeDef *) DBGMCU_BASE)






#endif