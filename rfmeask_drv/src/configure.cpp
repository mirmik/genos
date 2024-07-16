#include "configure.h"
#include <igris/util/bits.h>

#include <zillot/armv7e-m/arm_nvic.h>
#include <zillot/gpiodefs.h>
#include <zillot/stm32/stm32_exti.h>
#include <zillot/stm32/stm32_gpio.h>
#include <zillot/stm32/stm32_rcc.h>
#include <zillot/stm32/stm32_syscfg.h>
#include <zillot/stm32/stm32_timer.h>
#include <zillot/stm32/stm32_usart.h>
#include <zillot/stm32/timer.h>
//#include <spl/stm32f4xx_tim.h>
//#include <spl/stm32f4xx_gpio.h>
#include <igris/util/cpu_delay.h>

//Связь с контроллером по rs232.
void usart6_configure()
{
    stm32_rcc_enable_gpio(GPIOC);
    stm32_rcc_enable_usart(USART6);

    cpu_delay(100);

    stm32_gpio_setup(GPIOC, (1 << 6 | 1 << 7), GPIO_MODE_ALTERNATE);
    stm32_gpio_set_maxspeed(GPIOC, (1 << 6 | 1 << 7), STM32_GPIO_SPEED_LEVEL_0);
    stm32_gpio_set_alternate(GPIOC, (1 << 6 | 1 << 7), GPIO_AF_USART6);

    nvic_enable_irq(USART6_IRQn);
    stm32_usart_setup(USART6, 115200, 'e', 9, 1);
}

//Таймер для генерации одиночного импульса. OneShoot.
void tim10_configure()
{
    stm32_rcc_enable_gpio(GPIOB);
    stm32_rcc_enable_timer(TIM10);

    stm32_gpio_setup(GPIOB, (1 << 8), GPIO_MODE_ALTERNATE);
    stm32_gpio_set_maxspeed(GPIOB, (1 << 8), STM32_GPIO_SPEED_LEVEL_3);
    stm32_gpio_set_alternate(GPIOB, (1 << 8), GPIO_AF_TIM10);

    TIM_OCInitTypeDef TIM_OCInitStructure;

    // Длительность импульса 200 наносекунд при 3, 6 ?
    stm32_timer_set_prescaler(TIM10, 1);
    // stm32_timer_set_period(TIM10, 6);
    stm32_timer_set_period(TIM10, 168); // 1 микросекунда

    stm32_timer_set_direction(TIM10, TimerDirection::TimerDirectionDown);
    stm32_timer_set_one_pulse_mode(TIM10, 1);

    stm32_timer_set_counter(TIM10, 0);

    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 1; //< пульс длиной один такт

    // Полярность
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //< проседающий
    // импульс
    // TIM_OCInitStructure.TIM_OCPolarity =
    //    TIM_OCPolarity_Low; //< выпуклый импульс

    TIM_OC1Init(TIM10, &TIM_OCInitStructure);
};

void tim10_configure_polarity(int reverse_polarity) 
{
    auto polarity = TIM_OCPolarity_High;
    polarity = reverse_polarity != 0 ? TIM_OCPolarity_High : TIM_OCPolarity_Low;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 1; //< пульс длиной один такт
    TIM_OCInitStructure.TIM_OCPolarity = polarity; //< проседающий
    TIM_OC1Init(TIM10, &TIM_OCInitStructure);
}

void tim10_configure_duration(int duration) 
{
    stm32_timer_set_prescaler(TIM10, 1);
    stm32_timer_set_period(TIM10, duration);
}

//Генератор импульсов.
void tim4_configure()
{
    dprln("tim4 disabled");
    /*TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    rcc_enable_timer(TIM4);

    TIM_TimeBaseStructure.TIM_Prescaler = 10 - 1;
    TIM_TimeBaseStructure.TIM_Period = 16000 - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

    TIM4->CNT = 0;
    TIM_Cmd(TIM4, ENABLE);
    NVIC_EnableIRQ(TIM4_IRQn);*/
};

// Декодер квадратурного импульсного сигнала
// TODO: Какой это порт?
// Предположительно это CH2.
void tim5_configure()
{
    TIM_OCInitTypeDef TIM_OCInitStructure;

    stm32_rcc_enable_timer(TIM5);
    stm32_rcc_enable_gpio(GPIOA);
    stm32_gpio_setup(GPIOA, (1 << 0) | (1 << 1), GPIO_MODE_ALTERNATE);
    stm32_gpio_set_alternate(GPIOA, (1 << 0) | (1 << 1), GPIO_AF_TIM5);

    TIM_EncoderInterfaceConfig(TIM5,
                               TIM_EncoderMode_TI12,
                               TIM_ICPolarity_Rising,
                               TIM_ICPolarity_Rising);
    TIM5->CNT = 0;

    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Active;
    TIM_OCInitStructure.TIM_Pulse = 5000;
    TIM_OC3Init(TIM5, &TIM_OCInitStructure);

    stm32_timer_enable(TIM5, ENABLE);

    nvic_set_priority(TIM5_IRQn, 0);
    nvic_enable_irq(TIM5_IRQn);
};

void tim5_compare_enable()
{
    TIM_ClearITPendingBit(TIM5, TIM_IT_CC3);
    stm32_timer_config_interrupt_source(TIM5, TIM_IT_CC3, ENABLE);
};

void tim5_compare_disable()
{
    stm32_timer_config_interrupt_source(TIM5, TIM_IT_CC3, DISABLE);
};

// Декодер квадратурного импульсного сигнала
// TODO: Какой это порт?
// Предположительно это CH1.
void tim2_configure()
{
    TIM_OCInitTypeDef TIM_OCInitStructure;

    stm32_rcc_enable_gpio(GPIOA);
    stm32_rcc_enable_gpio(GPIOB);
    stm32_rcc_enable_timer(TIM2);

    stm32_gpio_setup(GPIOA, (1 << 5), GPIO_MODE_ALTERNATE);
    stm32_gpio_set_alternate(GPIOA, (1 << 5), GPIO_AF_TIM2);

    stm32_gpio_setup(GPIOB, (1 << 3), GPIO_MODE_ALTERNATE);
    stm32_gpio_set_alternate(GPIOB, (1 << 3), GPIO_AF_TIM2);

    TIM_EncoderInterfaceConfig(TIM2,
                               TIM_EncoderMode_TI12,
                               TIM_ICPolarity_Rising,
                               TIM_ICPolarity_Rising);
    TIM2->CNT = 0;

    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Active;
    TIM_OCInitStructure.TIM_Pulse = 5000;
    TIM_OC3Init(TIM2, &TIM_OCInitStructure);

    stm32_timer_enable(TIM2, ENABLE);

    nvic_set_priority(TIM2_IRQn, 0);
    nvic_enable_irq(TIM2_IRQn);
};

void tim2_compare_enable()
{
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC3);
    stm32_timer_config_interrupt_source(TIM2, TIM_IT_CC3, ENABLE);
};

void tim2_compare_disable()
{
    stm32_timer_config_interrupt_source(TIM2, TIM_IT_CC3, DISABLE);
};

void motor_pins_configure()
{
    stm32_rcc_enable_gpio(GPIOD);
    stm32_rcc_enable_gpio(GPIOB);

    stm32_gpio_setup(GPIOD,
                     (1 << 2) | (1 << 4) | (1 << 6),
                     GPIO_MODE_OUTPUT | GPIO_MODE_OUT_PUSH_PULL);
    stm32_gpio_setup(
        GPIOB, (1 << 5), GPIO_MODE_OUTPUT | GPIO_MODE_OUT_PUSH_PULL);

    stm32_gpio_set_maxspeed(
        GPIOD, (1 << 2) | (1 << 4) | (1 << 6), STM32_GPIO_SPEED_LEVEL_0);
    stm32_gpio_set_maxspeed(GPIOB, (1 << 5), STM32_GPIO_SPEED_LEVEL_0);
};

void trigin_configure()
{
    stm32_rcc_enable_syscfg();
    stm32_rcc_enable_gpio(GPIOB);

    stm32_gpio_setup(GPIOB, (1 << 9), GPIO_MODE_INPUT | GPIO_MODE_IN_NOPULL);
    stm32_gpio_set_maxspeed(GPIOB, (1 << 9), STM32_GPIO_SPEED_LEVEL_3);

    stm32_syscfg_exti_map(9, SYSCFG_EXTI_PORTB);

    stm32_exti_unmask(9, true);

    // Высокий уровень сигнализирует о готовности,
    // поэтому восходящий фронт интерпретируется
    // как входной триггер.
    stm32_exti_rising_trigger_enable(9, true);

    nvic_set_priority(EXTI9_5_IRQn, 0);
    nvic_enable_irq(EXTI9_5_IRQn);
};

// Настройка дополнительных gpio (четырёхштырьковый разъём на плате)
void external_gpio_configure(void)
{
    stm32_rcc_enable_gpio(GPIOB);
    stm32_gpio_setup(GPIOB,
                     (1 << 12) | (1 << 13) | (1 << 14) | (1 << 15),
                     GPIO_MODE_OUTPUT | GPIO_MODE_OUT_PUSH_PULL);

    stm32_gpio_write(GPIOB, (1 << 12) | (1 << 13) | (1 << 14) | (1 << 15), 0);
};

void project_configure()
{
    stm32_usart_setup(USART2, 115200, 'e', 9, 1);
    usart6_configure();
    tim10_configure(); // ONE_PULSE_GENERATOR
    tim5_configure();  // ENCODER X (CH2)
    tim2_configure();  // ENCODER Y (CH1)
    tim4_configure();  // GENERATOR X (CH2)
    motor_pins_configure();
    trigin_configure(); // TRIGGER
    external_gpio_configure();
};