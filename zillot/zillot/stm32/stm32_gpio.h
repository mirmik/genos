#ifndef ASM_STM32_GPIO_H
#define ASM_STM32_GPIO_H

#include <igris/compiler.h>
#include <stm32xxxx.h>

/**
 * @brief   AF 0 selection
 */
#define GPIO_AF_RTC_50Hz                                                       \
    ((uint8_t)0x00) /* RTC_50Hz Alternate Function mapping */
#define GPIO_AF_MCO                                                            \
    ((uint8_t)0x00) /* MCO (MCO1 and MCO2) Alternate Function mapping */
#define GPIO_AF_TAMPER                                                         \
    ((uint8_t)0x00) /* TAMPER (TAMPER_1 and TAMPER_2) Alternate Function       \
                       mapping */
#define GPIO_AF_SWJ                                                            \
    ((uint8_t)0x00) /* SWJ (SWD and JTAG) Alternate Function mapping */
#define GPIO_AF_TRACE ((uint8_t)0x00) /* TRACE Alternate Function mapping */

/**
 * @brief   AF 1 selection
 */
#define GPIO_AF_TIM1 ((uint8_t)0x01) /* TIM1 Alternate Function mapping */
#define GPIO_AF_TIM2 ((uint8_t)0x01) /* TIM2 Alternate Function mapping */

/**
 * @brief   AF 2 selection
 */
#define GPIO_AF_TIM3 ((uint8_t)0x02) /* TIM3 Alternate Function mapping */
#define GPIO_AF_TIM4 ((uint8_t)0x02) /* TIM4 Alternate Function mapping */
#define GPIO_AF_TIM5 ((uint8_t)0x02) /* TIM5 Alternate Function mapping */

/**
 * @brief   AF 3 selection
 */
#define GPIO_AF_TIM8 ((uint8_t)0x03)  /* TIM8 Alternate Function mapping */
#define GPIO_AF_TIM9 ((uint8_t)0x03)  /* TIM9 Alternate Function mapping */
#define GPIO_AF_TIM10 ((uint8_t)0x03) /* TIM10 Alternate Function mapping */
#define GPIO_AF_TIM11 ((uint8_t)0x03) /* TIM11 Alternate Function mapping */

/**
 * @brief   AF 4 selection
 */
#define GPIO_AF_I2C1 ((uint8_t)0x04) /* I2C1 Alternate Function mapping */
#define GPIO_AF_I2C2 ((uint8_t)0x04) /* I2C2 Alternate Function mapping */
#define GPIO_AF_I2C3 ((uint8_t)0x04) /* I2C3 Alternate Function mapping */

/**
 * @brief   AF 5 selection
 */
#define GPIO_AF_SPI1 ((uint8_t)0x05) /* SPI1 Alternate Function mapping */
#define GPIO_AF_SPI2 ((uint8_t)0x05) /* SPI2/I2S2 Alternate Function mapping   \
                                      */
#define GPIO_AF_SPI4 ((uint8_t)0x05) /* SPI4 Alternate Function mapping */
#define GPIO_AF_SPI5 ((uint8_t)0x05) /* SPI5 Alternate Function mapping */
#define GPIO_AF_SPI6 ((uint8_t)0x05) /* SPI6 Alternate Function mapping */

/**
 * @brief   AF 6 selection
 */
#define GPIO_AF_SPI3 ((uint8_t)0x06) /* SPI3/I2S3 Alternate Function mapping   \
                                      */

#define GPIO_AF_SAI1 ((uint8_t)0x06) /* SAI1 Alternate Function mapping */

/**
 * @brief   AF 7 selection
 */
#define GPIO_AF_USART1 ((uint8_t)0x07)  /* USART1 Alternate Function mapping  */
#define GPIO_AF_USART2 ((uint8_t)0x07)  /* USART2 Alternate Function mapping  */
#define GPIO_AF_USART3 ((uint8_t)0x07)  /* USART3 Alternate Function mapping  */
#define GPIO_AF_I2S3ext ((uint8_t)0x07) /* I2S3ext Alternate Function mapping  \
                                         */

/**
 * @brief   AF 8 selection
 */
#define GPIO_AF_UART4 ((uint8_t)0x08)  /* UART4 Alternate Function mapping  */
#define GPIO_AF_UART5 ((uint8_t)0x08)  /* UART5 Alternate Function mapping  */
#define GPIO_AF_USART6 ((uint8_t)0x08) /* USART6 Alternate Function mapping */
#define GPIO_AF_UART7 ((uint8_t)0x08)  /* UART7 Alternate Function mapping  */
#define GPIO_AF_UART8 ((uint8_t)0x08)  /* UART8 Alternate Function mapping  */

/**
 * @brief   AF 9 selection
 */
#define GPIO_AF_CAN1 ((uint8_t)0x09)  /* CAN1 Alternate Function mapping  */
#define GPIO_AF_CAN2 ((uint8_t)0x09)  /* CAN2 Alternate Function mapping  */
#define GPIO_AF_TIM12 ((uint8_t)0x09) /* TIM12 Alternate Function mapping */
#define GPIO_AF_TIM13 ((uint8_t)0x09) /* TIM13 Alternate Function mapping */
#define GPIO_AF_TIM14 ((uint8_t)0x09) /* TIM14 Alternate Function mapping */

#define GPIO_AF9_I2C2                                                          \
    ((uint8_t)0x09) /* I2C2 Alternate Function mapping (Only for STM32F401xx   \
                       Devices) */
#define GPIO_AF9_I2C3                                                          \
    ((uint8_t)0x09) /* I2C3 Alternate Function mapping (Only for STM32F401xx   \
                       Devices) */

/**
 * @brief   AF 10 selection
 */
#define GPIO_AF_OTG_FS ((uint8_t)0xA) /* OTG_FS Alternate Function mapping */
#define GPIO_AF_OTG_HS ((uint8_t)0xA) /* OTG_HS Alternate Function mapping */

/**
 * @brief   AF 11 selection
 */
#define GPIO_AF_ETH ((uint8_t)0x0B) /* ETHERNET Alternate Function mapping */

/**
 * @brief   AF 12 selection
 */
#if defined(STM32F40_41xxx)
#define GPIO_AF_FSMC                                                           \
    ((uint8_t)0xC) /* FSMC Alternate Function mapping                     */
#endif             /* STM32F40_41xxx */

#if defined(STM32F427_437xx) || defined(STM32F429_439xx)
#define GPIO_AF_FMC                                                            \
    ((uint8_t)0xC) /* FMC Alternate Function mapping                      */
#endif             /* STM32F427_437xx ||  STM32F429_439xx */

#define GPIO_AF_OTG_HS_FS                                                      \
    ((uint8_t)0xC) /* OTG HS configured in FS, Alternate Function mapping */
#define GPIO_AF_SDIO                                                           \
    ((uint8_t)0xC) /* SDIO Alternate Function mapping                     */

/**
 * @brief   AF 13 selection
 */
#define GPIO_AF_DCMI ((uint8_t)0x0D) /* DCMI Alternate Function mapping */

/**
 * @brief   AF 14 selection
 */

#define GPIO_AF_LTDC ((uint8_t)0x0E) /* LCD-TFT Alternate Function mapping */

/**
 * @brief   AF 15 selection
 */
#define GPIO_AF_EVENTOUT                                                       \
    ((uint8_t)0x0F) /* EVENTOUT Alternate Function mapping */

#if defined STM32F1XX || defined STM32F2XX
enum stm32_gpio_maxspeed_e
{
    STM32_GPIO_2MHZ = 0b10,
    STM32_GPIO_10MHZ = 0b01,
    STM32_GPIO_50MHZ = 0b11
};
#elif defined STM32F3XX || defined STM32F4XX || defined STM32L4XX ||           \
    defined STM32G4XX || STM32F7XX || STM32H7XX
enum stm32_gpio_maxspeed_e
{
    STM32_GPIO_SPEED_LEVEL_0 = 0b00,
    STM32_GPIO_SPEED_LEVEL_1 = 0b01,
    STM32_GPIO_SPEED_LEVEL_2 = 0b10,
    STM32_GPIO_SPEED_LEVEL_3 = 0b11,
};
#endif

__BEGIN_DECLS

void stm32_gpio_set_output(GPIO_TypeDef *gpio, unsigned int mask);
void stm32_gpio_set_level(GPIO_TypeDef *gpio, unsigned int mask, uint8_t lvl);

void stm32_gpio_write(GPIO_TypeDef *gpio, unsigned int mask, uint8_t lvl);
unsigned int stm32_gpio_read(GPIO_TypeDef *gpio, unsigned int mask);
void stm32_gpio_toggle(GPIO_TypeDef *gpio, unsigned int mask);

int stm32_gpio_set_alternate(GPIO_TypeDef *g, uint32_t mask, int32_t alternate);

void stm32_gpio_set_maxspeed(GPIO_TypeDef *g,
                             uint16_t map,
                             enum stm32_gpio_maxspeed_e maxspeed);

void stm32_gpio_pull_up(GPIO_TypeDef *gpio, unsigned int mask, uint8_t lvl);
int stm32_gpio_setup(GPIO_TypeDef *gpio, unsigned int mask, uint32_t mode);

__END_DECLS

#endif