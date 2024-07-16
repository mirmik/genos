#ifndef STM32_ASM_STM32_TIMER_H
#define STM32_ASM_STM32_TIMER_H

#include <igris/compiler.h>
#include <igris/util/bits.h>
#include <stm32xxxx.h>

#define STM32_TIMER_DOWNCOUNT 1
#define STM32_TIMER_UPCOUNT 0
//#define STM32_TIMER_CENTERCOUNT

__BEGIN_DECLS

// Установить предделитель. Частота тика таймера равна частоте его
// шины деленной на значение предделителя.
static inline void stm32_timer_set_prescaler(TIM_TypeDef *tim,
                                             uint16_t prescaler)
{
    tim->PSC = prescaler - 1;
}

// Установить период счета в тиках таймера. После достижения значения периода
// таймер сбрасывается. Auto reload register.
static inline void stm32_timer_set_period(TIM_TypeDef *tim, uint16_t period)
{
    tim->ARR = period - 1;
}

// Установить текущее значение таймера.
static inline void stm32_timer_set_counter(TIM_TypeDef *tim, uint16_t counter)
{
    tim->CNT = counter;
}

// Считать значение счетчика.
static inline uint32_t stm32_timer_counter(TIM_TypeDef *tim)
{
    return tim->CNT;
}

static inline void
stm32_timer_channel_set_compare(TIM_TypeDef *tim, int channel, uint16_t value)
{
    switch (channel)
    {
    case 1:
        tim->CCR1 = value;
        break;
    case 2:
        tim->CCR2 = value;
        break;
    case 3:
        tim->CCR3 = value;
        break;
    case 4:
        tim->CCR4 = value;
        break;
    }
}

// Enable pwm mode.
static inline void stm32_timer_channel_enable_pwm(TIM_TypeDef *tim, int channel)
{
    switch (channel)
    {
    case 1:
        tim->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;
        break;
    case 2:
        tim->CCMR1 |= TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2;
        break;
    case 3:
        tim->CCMR2 |= TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2;
        break;
    case 4:
        tim->CCMR2 |= TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_2;
        break;
    }
}

static inline void stm32_timer_channel_enable_output(TIM_TypeDef *tim,
                                                     int channel)
{
    switch (channel)
    {
    case 1:
        tim->CCER |= TIM_CCER_CC1E;
        break;
    case 2:
        tim->CCER |= TIM_CCER_CC2E;
        break;
    case 3:
        tim->CCER |= TIM_CCER_CC3E;
        break;
    case 4:
        tim->CCER |= TIM_CCER_CC4E;
        break;
    }
}

// Включение выключения счета таймера.
static inline void stm32_timer_enable(TIM_TypeDef *tim, uint8_t en)
{
    bits_lvl(tim->CR1, TIM_CR1_CEN, en);
}

static inline void stm32_timer_set_center_aligned_mode(TIM_TypeDef *tim,
                                                       uint8_t mode)
{
    const int pos = __builtin_ctz(TIM_CR1_CMS);
    bits_assign(tim->CR1, TIM_CR1_CMS, (mode << pos));
}

enum TimerDirection
{
    TimerDirectionUp = 0,
    TimerDirectionDown = 1
};

// Управляет направлением счёта в случае, если CMS == 0.
static inline void stm32_timer_set_direction(TIM_TypeDef *tim,
                                             enum TimerDirection mode)
{
    bits_lvl(tim->CR1, TIM_CR1_DIR, mode);
}

static inline void stm32_timer_set_counter_mode(TIM_TypeDef *tim, uint8_t mode)
{
    stm32_timer_set_direction(tim, (enum TimerDirection)mode);
}

static inline void stm32_timer_set_one_pulse_mode(TIM_TypeDef *tim, uint8_t en)
{
    bits_lvl(tim->CR1, TIM_CR1_OPM, en);
}

static inline void stm32_timer_config_interrupt_source(TIM_TypeDef *TIMx,
                                                       uint16_t TIM_IT,
                                                       bool NewState)
{
    if (NewState)
    {
        /* Enable the Interrupt sources */
        TIMx->DIER |= TIM_IT;
    }
    else
    {
        /* Disable the Interrupt sources */
        TIMx->DIER &= (uint16_t)~TIM_IT;
    }
}

// Активировать прерывание по переполнению ( бит UIE - update interrupt enable
// ). Не забудьте настроить разрешение в NVIC. Перед выходом из прерывания
// следует сбросить флаг SR->UIF.
static inline void stm32_timer_ovf_irq_enable(TIM_TypeDef *tim, uint8_t en)
{
    bits_lvl(tim->DIER, TIM_DIER_UIE, en);
}

static inline void stm32_timer_channel_cc_irq_clear(TIM_TypeDef *tim, int ch)
{
    switch (ch)
    {
    case 1:
        tim->SR &= ~TIM_SR_CC1IF;
        break;
    case 2:
        tim->SR &= ~TIM_SR_CC2IF;
        break;
    case 3:
        tim->SR &= ~TIM_SR_CC3IF;
        break;
    case 4:
        tim->SR &= ~TIM_SR_CC4IF;
        break;
    }
}

static inline void
stm32_timer_channel_cc_irq_enable(TIM_TypeDef *tim, int ch, uint8_t en)
{
    switch (ch)
    {
    case 1:
        bits_lvl(tim->DIER, TIM_DIER_CC1IE, en);
        break;
    case 2:
        bits_lvl(tim->DIER, TIM_DIER_CC2IE, en);
        break;
    case 3:
        bits_lvl(tim->DIER, TIM_DIER_CC3IE, en);
        break;
    case 4:
        bits_lvl(tim->DIER, TIM_DIER_CC4IE, en);
        break;
    }
}

static inline void stm32_timer_drop_ovf_flag(TIM_TypeDef *tim)
{
    bits_clr(tim->SR, TIM_SR_UIF);
}

enum TimerICPolarity
{
    TimerICPolarityRising = 0x0,
    TimerICPolarityFalling = 0x2,
    TimerICPolarityBothEdge = 0xA
};

enum TimerEncoderMode
{
    TimerEncoderMode_TI1 = 0x1,
    TimerEncoderMode_TI2 = 0x2,
    TimerEncoderMode_TI12 = 0x3
};

//#define TIM_ICPolarity_Rising ((uint16_t)0x0000)
//#define TIM_ICPolarity_Falling ((uint16_t)0x0002)
//#define TIM_ICPolarity_BothEdge ((uint16_t)0x000A)

//#define TIM_EncoderMode_TI1 ((uint16_t)0x0001)
//#define TIM_EncoderMode_TI2 ((uint16_t)0x0002)
//#define TIM_EncoderMode_TI12 ((uint16_t)0x0003)
static inline void
stm32_timer_encoder_interface_config(TIM_TypeDef *TIMx,
                                     enum TimerEncoderMode TIM_EncoderMode,
                                     enum TimerICPolarity TIM_IC1Polarity,
                                     enum TimerICPolarity TIM_IC2Polarity)
{
    // Считываем регистры во временные переменные
    uint16_t tmpsmcr = TIMx->SMCR;
    uint16_t tmpccmr1 = TIMx->CCMR1;
    uint16_t tmpccer = TIMx->CCER;

    // Очищаем поле slave mode selection и устанавливаем в него выбранный режим
    // энкодера.
    tmpsmcr &= (uint16_t)~TIM_SMCR_SMS;
    tmpsmcr |= TIM_EncoderMode;

    // Настраиваем таймер в режим счёта внешних сигналов
    // Выбираем Capture Compare 1 и Capture Compare 2 в режиме входа IC1 и IC2
    // Нулевой режим как то связан с направлением счёта?
    tmpccmr1 &= ((uint16_t)~TIM_CCMR1_CC1S) & ((uint16_t)~TIM_CCMR1_CC2S);
    tmpccmr1 |= TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_0;

    // Очищаем полярности входов энкодера и устанавливаем выбранные
    tmpccer &= ((uint16_t)~TIM_CCER_CC1P) & ((uint16_t)~TIM_CCER_CC2P);
    tmpccer |= (uint16_t)(TIM_IC1Polarity |
                          (uint16_t)(TIM_IC2Polarity << (uint16_t)4));

    // Записываем настройки
    TIMx->SMCR = tmpsmcr;
    TIMx->CCMR1 = tmpccmr1;
    TIMx->CCER = tmpccer;
}

__END_DECLS

///////////////////////////////////////////////////////////////
//#if 0
typedef struct
{
    uint16_t TIM_OCMode; /*!< Specifies the TIM mode.
                              This parameter can be a value of @ref
                            TIM_Output_Compare_and_PWM_modes */

    uint16_t TIM_OutputState; /*!< Specifies the TIM Output Compare state.
                                   This parameter can be a value of @ref
                                 TIM_Output_Compare_State */

    uint16_t TIM_OutputNState; /*!< Specifies the TIM complementary Output
                                  Compare state. This parameter can be a value
                                  of @ref TIM_Output_Compare_N_State
                                    @note This parameter is valid only for TIM1
                                  and TIM8. */

    uint32_t TIM_Pulse; /*!< Specifies the pulse value to be loaded into the
                           Capture Compare Register. This parameter can be a
                           number between 0x0000 and 0xFFFF */

    uint16_t TIM_OCPolarity; /*!< Specifies the output polarity.
                                  This parameter can be a value of @ref
                                TIM_Output_Compare_Polarity */

    uint16_t TIM_OCNPolarity; /*!< Specifies the complementary output polarity.
                                   This parameter can be a value of @ref
                                 TIM_Output_Compare_N_Polarity
                                   @note This parameter is valid only for TIM1
                                 and TIM8. */

    uint16_t TIM_OCIdleState; /*!< Specifies the TIM Output Compare pin state
                                 during Idle state. This parameter can be a
                                 value of @ref TIM_Output_Compare_Idle_State
                                   @note This parameter is valid only for TIM1
                                 and TIM8. */

    uint16_t TIM_OCNIdleState; /*!< Specifies the TIM Output Compare pin state
                                  during Idle state. This parameter can be a
                                  value of @ref TIM_Output_Compare_N_Idle_State
                                    @note This parameter is valid only for TIM1
                                  and TIM8. */
} TIM_OCInitTypeDef;

__BEGIN_DECLS

void TIM_OC1Init(TIM_TypeDef *TIMx, TIM_OCInitTypeDef *TIM_OCInitStruct);
void TIM_OC3Init(TIM_TypeDef *TIMx, TIM_OCInitTypeDef *TIM_OCInitStruct);
void TIM_OCStructInit(TIM_OCInitTypeDef *TIM_OCInitStruct);
void TIM_EncoderInterfaceConfig(TIM_TypeDef *TIMx,
                                uint16_t TIM_EncoderMode,
                                uint16_t TIM_IC1Polarity,
                                uint16_t TIM_IC2Polarity);
void TIM_ClearITPendingBit(TIM_TypeDef *TIMx, uint16_t TIM_IT);
void TIM_OC3Init(TIM_TypeDef *TIMx, TIM_OCInitTypeDef *TIM_OCInitStruct);

__END_DECLS

#define IS_TIM_ALL_PERIPH(PERIPH)                                              \
    (((PERIPH) == TIM1) || ((PERIPH) == TIM2) || ((PERIPH) == TIM3) ||         \
     ((PERIPH) == TIM4) || ((PERIPH) == TIM5) || ((PERIPH) == TIM6) ||         \
     ((PERIPH) == TIM7) || ((PERIPH) == TIM8) || ((PERIPH) == TIM9) ||         \
     ((PERIPH) == TIM10) || ((PERIPH) == TIM11) || ((PERIPH) == TIM12) ||      \
     (((PERIPH) == TIM13) || ((PERIPH) == TIM14)))
/* LIST1: TIM1, TIM2, TIM3, TIM4, TIM5, TIM8, TIM9, TIM10, TIM11, TIM12, TIM13
 * and TIM14 */
#define IS_TIM_LIST1_PERIPH(PERIPH)                                            \
    (((PERIPH) == TIM1) || ((PERIPH) == TIM2) || ((PERIPH) == TIM3) ||         \
     ((PERIPH) == TIM4) || ((PERIPH) == TIM5) || ((PERIPH) == TIM8) ||         \
     ((PERIPH) == TIM9) || ((PERIPH) == TIM10) || ((PERIPH) == TIM11) ||       \
     ((PERIPH) == TIM12) || ((PERIPH) == TIM13) || ((PERIPH) == TIM14))

/* LIST2: TIM1, TIM2, TIM3, TIM4, TIM5, TIM8, TIM9 and TIM12 */
#define IS_TIM_LIST2_PERIPH(PERIPH)                                            \
    (((PERIPH) == TIM1) || ((PERIPH) == TIM2) || ((PERIPH) == TIM3) ||         \
     ((PERIPH) == TIM4) || ((PERIPH) == TIM5) || ((PERIPH) == TIM8) ||         \
     ((PERIPH) == TIM9) || ((PERIPH) == TIM12))
/* LIST3: TIM1, TIM2, TIM3, TIM4, TIM5 and TIM8 */
#define IS_TIM_LIST3_PERIPH(PERIPH)                                            \
    (((PERIPH) == TIM1) || ((PERIPH) == TIM2) || ((PERIPH) == TIM3) ||         \
     ((PERIPH) == TIM4) || ((PERIPH) == TIM5) || ((PERIPH) == TIM8))
/* LIST4: TIM1 and TIM8 */
#define IS_TIM_LIST4_PERIPH(PERIPH) (((PERIPH) == TIM1) || ((PERIPH) == TIM8))
/* LIST5: TIM1, TIM2, TIM3, TIM4, TIM5, TIM6, TIM7 and TIM8 */
#define IS_TIM_LIST5_PERIPH(PERIPH)                                            \
    (((PERIPH) == TIM1) || ((PERIPH) == TIM2) || ((PERIPH) == TIM3) ||         \
     ((PERIPH) == TIM4) || ((PERIPH) == TIM5) || ((PERIPH) == TIM6) ||         \
     ((PERIPH) == TIM7) || ((PERIPH) == TIM8))
/* LIST6: TIM2, TIM5 and TIM11 */
#define IS_TIM_LIST6_PERIPH(TIMx)                                              \
    (((TIMx) == TIM2) || ((TIMx) == TIM5) || ((TIMx) == TIM11))

/** @defgroup TIM_Output_Compare_and_PWM_modes
 * @{
 */

#define TIM_OCMode_Timing ((uint16_t)0x0000)
#define TIM_OCMode_Active ((uint16_t)0x0010)
#define TIM_OCMode_Inactive ((uint16_t)0x0020)
#define TIM_OCMode_Toggle ((uint16_t)0x0030)
#define TIM_OCMode_PWM1 ((uint16_t)0x0060)
#define TIM_OCMode_PWM2 ((uint16_t)0x0070)
#define IS_TIM_OC_MODE(MODE)                                                   \
    (((MODE) == TIM_OCMode_Timing) || ((MODE) == TIM_OCMode_Active) ||         \
     ((MODE) == TIM_OCMode_Inactive) || ((MODE) == TIM_OCMode_Toggle) ||       \
     ((MODE) == TIM_OCMode_PWM1) || ((MODE) == TIM_OCMode_PWM2))
#define IS_TIM_OCM(MODE)                                                       \
    (((MODE) == TIM_OCMode_Timing) || ((MODE) == TIM_OCMode_Active) ||         \
     ((MODE) == TIM_OCMode_Inactive) || ((MODE) == TIM_OCMode_Toggle) ||       \
     ((MODE) == TIM_OCMode_PWM1) || ((MODE) == TIM_OCMode_PWM2) ||             \
     ((MODE) == TIM_ForcedAction_Active) ||                                    \
     ((MODE) == TIM_ForcedAction_InActive))
/**
 * @}
 */

/** @defgroup TIM_One_Pulse_Mode
 * @{
 */

#define TIM_OPMode_Single ((uint16_t)0x0008)
#define TIM_OPMode_Repetitive ((uint16_t)0x0000)
#define IS_TIM_OPM_MODE(MODE)                                                  \
    (((MODE) == TIM_OPMode_Single) || ((MODE) == TIM_OPMode_Repetitive))
/**
 * @}
 */

/** @defgroup TIM_Channel
 * @{
 */

#define TIM_Channel_1 ((uint16_t)0x0000)
#define TIM_Channel_2 ((uint16_t)0x0004)
#define TIM_Channel_3 ((uint16_t)0x0008)
#define TIM_Channel_4 ((uint16_t)0x000C)

#define IS_TIM_CHANNEL(CHANNEL)                                                \
    (((CHANNEL) == TIM_Channel_1) || ((CHANNEL) == TIM_Channel_2) ||           \
     ((CHANNEL) == TIM_Channel_3) || ((CHANNEL) == TIM_Channel_4))

#define IS_TIM_PWMI_CHANNEL(CHANNEL)                                           \
    (((CHANNEL) == TIM_Channel_1) || ((CHANNEL) == TIM_Channel_2))
#define IS_TIM_COMPLEMENTARY_CHANNEL(CHANNEL)                                  \
    (((CHANNEL) == TIM_Channel_1) || ((CHANNEL) == TIM_Channel_2) ||           \
     ((CHANNEL) == TIM_Channel_3))
/**
 * @}
 */

/** @defgroup TIM_Clock_Division_CKD
 * @{
 */

#define TIM_CKD_DIV1 ((uint16_t)0x0000)
#define TIM_CKD_DIV2 ((uint16_t)0x0100)
#define TIM_CKD_DIV4 ((uint16_t)0x0200)
#define IS_TIM_CKD_DIV(DIV)                                                    \
    (((DIV) == TIM_CKD_DIV1) || ((DIV) == TIM_CKD_DIV2) ||                     \
     ((DIV) == TIM_CKD_DIV4))
/**
 * @}
 */

/** @defgroup TIM_Counter_Mode
 * @{
 */

#define TIM_CounterMode_Up ((uint16_t)0x0000)
#define TIM_CounterMode_Down ((uint16_t)0x0010)
#define TIM_CounterMode_CenterAligned1 ((uint16_t)0x0020)
#define TIM_CounterMode_CenterAligned2 ((uint16_t)0x0040)
#define TIM_CounterMode_CenterAligned3 ((uint16_t)0x0060)
#define IS_TIM_COUNTER_MODE(MODE)                                              \
    (((MODE) == TIM_CounterMode_Up) || ((MODE) == TIM_CounterMode_Down) ||     \
     ((MODE) == TIM_CounterMode_CenterAligned1) ||                             \
     ((MODE) == TIM_CounterMode_CenterAligned2) ||                             \
     ((MODE) == TIM_CounterMode_CenterAligned3))
/**
 * @}
 */

/** @defgroup TIM_Output_Compare_Polarity
 * @{
 */

#define TIM_OCPolarity_High ((uint16_t)0x0000)
#define TIM_OCPolarity_Low ((uint16_t)0x0002)
#define IS_TIM_OC_POLARITY(POLARITY)                                           \
    (((POLARITY) == TIM_OCPolarity_High) || ((POLARITY) == TIM_OCPolarity_Low))
/**
 * @}
 */

/** @defgroup TIM_Output_Compare_N_Polarity
 * @{
 */

#define TIM_OCNPolarity_High ((uint16_t)0x0000)
#define TIM_OCNPolarity_Low ((uint16_t)0x0008)
#define IS_TIM_OCN_POLARITY(POLARITY)                                          \
    (((POLARITY) == TIM_OCNPolarity_High) ||                                   \
     ((POLARITY) == TIM_OCNPolarity_Low))
/**
 * @}
 */

/** @defgroup TIM_Output_Compare_State
 * @{
 */

#define TIM_OutputState_Disable ((uint16_t)0x0000)
#define TIM_OutputState_Enable ((uint16_t)0x0001)
#define IS_TIM_OUTPUT_STATE(STATE)                                             \
    (((STATE) == TIM_OutputState_Disable) ||                                   \
     ((STATE) == TIM_OutputState_Enable))
/**
 * @}
 */

/** @defgroup TIM_Output_Compare_N_State
 * @{
 */

#define TIM_OutputNState_Disable ((uint16_t)0x0000)
#define TIM_OutputNState_Enable ((uint16_t)0x0004)
#define IS_TIM_OUTPUTN_STATE(STATE)                                            \
    (((STATE) == TIM_OutputNState_Disable) ||                                  \
     ((STATE) == TIM_OutputNState_Enable))
/**
 * @}
 */

/** @defgroup TIM_Capture_Compare_State
 * @{
 */

#define TIM_CCx_Enable ((uint16_t)0x0001)
#define TIM_CCx_Disable ((uint16_t)0x0000)
#define IS_TIM_CCX(CCX)                                                        \
    (((CCX) == TIM_CCx_Enable) || ((CCX) == TIM_CCx_Disable))
/**
 * @}
 */

/** @defgroup TIM_Capture_Compare_N_State
 * @{
 */

#define TIM_CCxN_Enable ((uint16_t)0x0004)
#define TIM_CCxN_Disable ((uint16_t)0x0000)
#define IS_TIM_CCXN(CCXN)                                                      \
    (((CCXN) == TIM_CCxN_Enable) || ((CCXN) == TIM_CCxN_Disable))
/**
 * @}
 */

/** @defgroup TIM_Break_Input_enable_disable
 * @{
 */

#define TIM_Break_Enable ((uint16_t)0x1000)
#define TIM_Break_Disable ((uint16_t)0x0000)
#define IS_TIM_BREAK_STATE(STATE)                                              \
    (((STATE) == TIM_Break_Enable) || ((STATE) == TIM_Break_Disable))
/**
 * @}
 */

/** @defgroup TIM_Break_Polarity
 * @{
 */

#define TIM_BreakPolarity_Low ((uint16_t)0x0000)
#define TIM_BreakPolarity_High ((uint16_t)0x2000)
#define IS_TIM_BREAK_POLARITY(POLARITY)                                        \
    (((POLARITY) == TIM_BreakPolarity_Low) ||                                  \
     ((POLARITY) == TIM_BreakPolarity_High))
/**
 * @}
 */

/** @defgroup TIM_AOE_Bit_Set_Reset
 * @{
 */

#define TIM_AutomaticOutput_Enable ((uint16_t)0x4000)
#define TIM_AutomaticOutput_Disable ((uint16_t)0x0000)
#define IS_TIM_AUTOMATIC_OUTPUT_STATE(STATE)                                   \
    (((STATE) == TIM_AutomaticOutput_Enable) ||                                \
     ((STATE) == TIM_AutomaticOutput_Disable))
/**
 * @}
 */

/** @defgroup TIM_Lock_level
 * @{
 */

#define TIM_LOCKLevel_OFF ((uint16_t)0x0000)
#define TIM_LOCKLevel_1 ((uint16_t)0x0100)
#define TIM_LOCKLevel_2 ((uint16_t)0x0200)
#define TIM_LOCKLevel_3 ((uint16_t)0x0300)
#define IS_TIM_LOCK_LEVEL(LEVEL)                                               \
    (((LEVEL) == TIM_LOCKLevel_OFF) || ((LEVEL) == TIM_LOCKLevel_1) ||         \
     ((LEVEL) == TIM_LOCKLevel_2) || ((LEVEL) == TIM_LOCKLevel_3))
/**
 * @}
 */

/** @defgroup TIM_OSSI_Off_State_Selection_for_Idle_mode_state
 * @{
 */

#define TIM_OSSIState_Enable ((uint16_t)0x0400)
#define TIM_OSSIState_Disable ((uint16_t)0x0000)
#define IS_TIM_OSSI_STATE(STATE)                                               \
    (((STATE) == TIM_OSSIState_Enable) || ((STATE) == TIM_OSSIState_Disable))
/**
 * @}
 */

/** @defgroup TIM_OSSR_Off_State_Selection_for_Run_mode_state
 * @{
 */

#define TIM_OSSRState_Enable ((uint16_t)0x0800)
#define TIM_OSSRState_Disable ((uint16_t)0x0000)
#define IS_TIM_OSSR_STATE(STATE)                                               \
    (((STATE) == TIM_OSSRState_Enable) || ((STATE) == TIM_OSSRState_Disable))
/**
 * @}
 */

/** @defgroup TIM_Output_Compare_Idle_State
 * @{
 */

#define TIM_OCIdleState_Set ((uint16_t)0x0100)
#define TIM_OCIdleState_Reset ((uint16_t)0x0000)
#define IS_TIM_OCIDLE_STATE(STATE)                                             \
    (((STATE) == TIM_OCIdleState_Set) || ((STATE) == TIM_OCIdleState_Reset))
/**
 * @}
 */

/** @defgroup TIM_Output_Compare_N_Idle_State
 * @{
 */

#define TIM_OCNIdleState_Set ((uint16_t)0x0200)
#define TIM_OCNIdleState_Reset ((uint16_t)0x0000)
#define IS_TIM_OCNIDLE_STATE(STATE)                                            \
    (((STATE) == TIM_OCNIdleState_Set) || ((STATE) == TIM_OCNIdleState_Reset))
/**
 * @}
 */

/** @defgroup TIM_Input_Capture_Polarity
 * @{
 */

#define TIM_ICPolarity_Rising ((uint16_t)0x0000)
#define TIM_ICPolarity_Falling ((uint16_t)0x0002)
#define TIM_ICPolarity_BothEdge ((uint16_t)0x000A)
#define IS_TIM_IC_POLARITY(POLARITY)                                           \
    (((POLARITY) == TIM_ICPolarity_Rising) ||                                  \
     ((POLARITY) == TIM_ICPolarity_Falling) ||                                 \
     ((POLARITY) == TIM_ICPolarity_BothEdge))
/**
 * @}
 */

/** @defgroup TIM_Input_Capture_Selection
 * @{
 */

#define TIM_ICSelection_DirectTI                                               \
    ((uint16_t)0x0001) /*!< TIM Input 1, 2, 3 or 4 is selected to be           \
                            connected to IC1, IC2, IC3 or IC4, respectively */
#define TIM_ICSelection_IndirectTI                                             \
    ((uint16_t)0x0002) /*!< TIM Input 1, 2, 3 or 4 is selected to be           \
                            connected to IC2, IC1, IC4 or IC3, respectively.   \
                        */
#define TIM_ICSelection_TRC                                                    \
    ((uint16_t)0x0003) /*!< TIM Input 1, 2, 3 or 4 is selected to be connected \
                          to TRC. */
#define IS_TIM_IC_SELECTION(SELECTION)                                         \
    (((SELECTION) == TIM_ICSelection_DirectTI) ||                              \
     ((SELECTION) == TIM_ICSelection_IndirectTI) ||                            \
     ((SELECTION) == TIM_ICSelection_TRC))
/**
 * @}
 */

/** @defgroup TIM_Input_Capture_Prescaler
 * @{
 */

#define TIM_ICPSC_DIV1                                                         \
    ((uint16_t)0x0000) /*!< Capture performed each time an edge is detected on \
                          the capture input. */
#define TIM_ICPSC_DIV2                                                         \
    ((uint16_t)0x0004) /*!< Capture performed once every 2 events. */
#define TIM_ICPSC_DIV4                                                         \
    ((uint16_t)0x0008) /*!< Capture performed once every 4 events. */
#define TIM_ICPSC_DIV8                                                         \
    ((uint16_t)0x000C) /*!< Capture performed once every 8 events. */
#define IS_TIM_IC_PRESCALER(PRESCALER)                                         \
    (((PRESCALER) == TIM_ICPSC_DIV1) || ((PRESCALER) == TIM_ICPSC_DIV2) ||     \
     ((PRESCALER) == TIM_ICPSC_DIV4) || ((PRESCALER) == TIM_ICPSC_DIV8))
/**
 * @}
 */

/** @defgroup TIM_interrupt_sources
 * @{
 */

#define TIM_IT_Update ((uint16_t)0x0001)
#define TIM_IT_CC1 ((uint16_t)0x0002)
#define TIM_IT_CC2 ((uint16_t)0x0004)
#define TIM_IT_CC3 ((uint16_t)0x0008)
#define TIM_IT_CC4 ((uint16_t)0x0010)
#define TIM_IT_COM ((uint16_t)0x0020)
#define TIM_IT_Trigger ((uint16_t)0x0040)
#define TIM_IT_Break ((uint16_t)0x0080)
#define IS_TIM_IT(IT)                                                          \
    ((((IT) & (uint16_t)0xFF00) == 0x0000) && ((IT) != 0x0000))

#define IS_TIM_GET_IT(IT)                                                      \
    (((IT) == TIM_IT_Update) || ((IT) == TIM_IT_CC1) ||                        \
     ((IT) == TIM_IT_CC2) || ((IT) == TIM_IT_CC3) || ((IT) == TIM_IT_CC4) ||   \
     ((IT) == TIM_IT_COM) || ((IT) == TIM_IT_Trigger) ||                       \
     ((IT) == TIM_IT_Break))
/**
 * @}
 */

/** @defgroup TIM_DMA_Base_address
 * @{
 */

#define TIM_DMABase_CR1 ((uint16_t)0x0000)
#define TIM_DMABase_CR2 ((uint16_t)0x0001)
#define TIM_DMABase_SMCR ((uint16_t)0x0002)
#define TIM_DMABase_DIER ((uint16_t)0x0003)
#define TIM_DMABase_SR ((uint16_t)0x0004)
#define TIM_DMABase_EGR ((uint16_t)0x0005)
#define TIM_DMABase_CCMR1 ((uint16_t)0x0006)
#define TIM_DMABase_CCMR2 ((uint16_t)0x0007)
#define TIM_DMABase_CCER ((uint16_t)0x0008)
#define TIM_DMABase_CNT ((uint16_t)0x0009)
#define TIM_DMABase_PSC ((uint16_t)0x000A)
#define TIM_DMABase_ARR ((uint16_t)0x000B)
#define TIM_DMABase_RCR ((uint16_t)0x000C)
#define TIM_DMABase_CCR1 ((uint16_t)0x000D)
#define TIM_DMABase_CCR2 ((uint16_t)0x000E)
#define TIM_DMABase_CCR3 ((uint16_t)0x000F)
#define TIM_DMABase_CCR4 ((uint16_t)0x0010)
#define TIM_DMABase_BDTR ((uint16_t)0x0011)
#define TIM_DMABase_DCR ((uint16_t)0x0012)
#define TIM_DMABase_OR ((uint16_t)0x0013)
#define IS_TIM_DMA_BASE(BASE)                                                  \
    (((BASE) == TIM_DMABase_CR1) || ((BASE) == TIM_DMABase_CR2) ||             \
     ((BASE) == TIM_DMABase_SMCR) || ((BASE) == TIM_DMABase_DIER) ||           \
     ((BASE) == TIM_DMABase_SR) || ((BASE) == TIM_DMABase_EGR) ||              \
     ((BASE) == TIM_DMABase_CCMR1) || ((BASE) == TIM_DMABase_CCMR2) ||         \
     ((BASE) == TIM_DMABase_CCER) || ((BASE) == TIM_DMABase_CNT) ||            \
     ((BASE) == TIM_DMABase_PSC) || ((BASE) == TIM_DMABase_ARR) ||             \
     ((BASE) == TIM_DMABase_RCR) || ((BASE) == TIM_DMABase_CCR1) ||            \
     ((BASE) == TIM_DMABase_CCR2) || ((BASE) == TIM_DMABase_CCR3) ||           \
     ((BASE) == TIM_DMABase_CCR4) || ((BASE) == TIM_DMABase_BDTR) ||           \
     ((BASE) == TIM_DMABase_DCR) || ((BASE) == TIM_DMABase_OR))
/**
 * @}
 */

/** @defgroup TIM_DMA_Burst_Length
 * @{
 */

#define TIM_DMABurstLength_1Transfer ((uint16_t)0x0000)
#define TIM_DMABurstLength_2Transfers ((uint16_t)0x0100)
#define TIM_DMABurstLength_3Transfers ((uint16_t)0x0200)
#define TIM_DMABurstLength_4Transfers ((uint16_t)0x0300)
#define TIM_DMABurstLength_5Transfers ((uint16_t)0x0400)
#define TIM_DMABurstLength_6Transfers ((uint16_t)0x0500)
#define TIM_DMABurstLength_7Transfers ((uint16_t)0x0600)
#define TIM_DMABurstLength_8Transfers ((uint16_t)0x0700)
#define TIM_DMABurstLength_9Transfers ((uint16_t)0x0800)
#define TIM_DMABurstLength_10Transfers ((uint16_t)0x0900)
#define TIM_DMABurstLength_11Transfers ((uint16_t)0x0A00)
#define TIM_DMABurstLength_12Transfers ((uint16_t)0x0B00)
#define TIM_DMABurstLength_13Transfers ((uint16_t)0x0C00)
#define TIM_DMABurstLength_14Transfers ((uint16_t)0x0D00)
#define TIM_DMABurstLength_15Transfers ((uint16_t)0x0E00)
#define TIM_DMABurstLength_16Transfers ((uint16_t)0x0F00)
#define TIM_DMABurstLength_17Transfers ((uint16_t)0x1000)
#define TIM_DMABurstLength_18Transfers ((uint16_t)0x1100)
#define IS_TIM_DMA_LENGTH(LENGTH)                                              \
    (((LENGTH) == TIM_DMABurstLength_1Transfer) ||                             \
     ((LENGTH) == TIM_DMABurstLength_2Transfers) ||                            \
     ((LENGTH) == TIM_DMABurstLength_3Transfers) ||                            \
     ((LENGTH) == TIM_DMABurstLength_4Transfers) ||                            \
     ((LENGTH) == TIM_DMABurstLength_5Transfers) ||                            \
     ((LENGTH) == TIM_DMABurstLength_6Transfers) ||                            \
     ((LENGTH) == TIM_DMABurstLength_7Transfers) ||                            \
     ((LENGTH) == TIM_DMABurstLength_8Transfers) ||                            \
     ((LENGTH) == TIM_DMABurstLength_9Transfers) ||                            \
     ((LENGTH) == TIM_DMABurstLength_10Transfers) ||                           \
     ((LENGTH) == TIM_DMABurstLength_11Transfers) ||                           \
     ((LENGTH) == TIM_DMABurstLength_12Transfers) ||                           \
     ((LENGTH) == TIM_DMABurstLength_13Transfers) ||                           \
     ((LENGTH) == TIM_DMABurstLength_14Transfers) ||                           \
     ((LENGTH) == TIM_DMABurstLength_15Transfers) ||                           \
     ((LENGTH) == TIM_DMABurstLength_16Transfers) ||                           \
     ((LENGTH) == TIM_DMABurstLength_17Transfers) ||                           \
     ((LENGTH) == TIM_DMABurstLength_18Transfers))
/**
 * @}
 */

/** @defgroup TIM_DMA_sources
 * @{
 */

#define TIM_DMA_Update ((uint16_t)0x0100)
#define TIM_DMA_CC1 ((uint16_t)0x0200)
#define TIM_DMA_CC2 ((uint16_t)0x0400)
#define TIM_DMA_CC3 ((uint16_t)0x0800)
#define TIM_DMA_CC4 ((uint16_t)0x1000)
#define TIM_DMA_COM ((uint16_t)0x2000)
#define TIM_DMA_Trigger ((uint16_t)0x4000)
#define IS_TIM_DMA_SOURCE(SOURCE)                                              \
    ((((SOURCE) & (uint16_t)0x80FF) == 0x0000) && ((SOURCE) != 0x0000))

/**
 * @}
 */

/** @defgroup TIM_External_Trigger_Prescaler
 * @{
 */

#define TIM_ExtTRGPSC_OFF ((uint16_t)0x0000)
#define TIM_ExtTRGPSC_DIV2 ((uint16_t)0x1000)
#define TIM_ExtTRGPSC_DIV4 ((uint16_t)0x2000)
#define TIM_ExtTRGPSC_DIV8 ((uint16_t)0x3000)
#define IS_TIM_EXT_PRESCALER(PRESCALER)                                        \
    (((PRESCALER) == TIM_ExtTRGPSC_OFF) ||                                     \
     ((PRESCALER) == TIM_ExtTRGPSC_DIV2) ||                                    \
     ((PRESCALER) == TIM_ExtTRGPSC_DIV4) ||                                    \
     ((PRESCALER) == TIM_ExtTRGPSC_DIV8))
/**
 * @}
 */

/** @defgroup TIM_Internal_Trigger_Selection
 * @{
 */

#define TIM_TS_ITR0 ((uint16_t)0x0000)
#define TIM_TS_ITR1 ((uint16_t)0x0010)
#define TIM_TS_ITR2 ((uint16_t)0x0020)
#define TIM_TS_ITR3 ((uint16_t)0x0030)
#define TIM_TS_TI1F_ED ((uint16_t)0x0040)
#define TIM_TS_TI1FP1 ((uint16_t)0x0050)
#define TIM_TS_TI2FP2 ((uint16_t)0x0060)
#define TIM_TS_ETRF ((uint16_t)0x0070)
#define IS_TIM_TRIGGER_SELECTION(SELECTION)                                    \
    (((SELECTION) == TIM_TS_ITR0) || ((SELECTION) == TIM_TS_ITR1) ||           \
     ((SELECTION) == TIM_TS_ITR2) || ((SELECTION) == TIM_TS_ITR3) ||           \
     ((SELECTION) == TIM_TS_TI1F_ED) || ((SELECTION) == TIM_TS_TI1FP1) ||      \
     ((SELECTION) == TIM_TS_TI2FP2) || ((SELECTION) == TIM_TS_ETRF))
#define IS_TIM_INTERNAL_TRIGGER_SELECTION(SELECTION)                           \
    (((SELECTION) == TIM_TS_ITR0) || ((SELECTION) == TIM_TS_ITR1) ||           \
     ((SELECTION) == TIM_TS_ITR2) || ((SELECTION) == TIM_TS_ITR3))
/**
 * @}
 */

/** @defgroup TIM_TIx_External_Clock_Source
 * @{
 */

#define TIM_TIxExternalCLK1Source_TI1 ((uint16_t)0x0050)
#define TIM_TIxExternalCLK1Source_TI2 ((uint16_t)0x0060)
#define TIM_TIxExternalCLK1Source_TI1ED ((uint16_t)0x0040)

/**
 * @}
 */

/** @defgroup TIM_External_Trigger_Polarity
 * @{
 */
#define TIM_ExtTRGPolarity_Inverted ((uint16_t)0x8000)
#define TIM_ExtTRGPolarity_NonInverted ((uint16_t)0x0000)
#define IS_TIM_EXT_POLARITY(POLARITY)                                          \
    (((POLARITY) == TIM_ExtTRGPolarity_Inverted) ||                            \
     ((POLARITY) == TIM_ExtTRGPolarity_NonInverted))
/**
 * @}
 */

/** @defgroup TIM_Prescaler_Reload_Mode
 * @{
 */

#define TIM_PSCReloadMode_Update ((uint16_t)0x0000)
#define TIM_PSCReloadMode_Immediate ((uint16_t)0x0001)
#define IS_TIM_PRESCALER_RELOAD(RELOAD)                                        \
    (((RELOAD) == TIM_PSCReloadMode_Update) ||                                 \
     ((RELOAD) == TIM_PSCReloadMode_Immediate))
/**
 * @}
 */

/** @defgroup TIM_Forced_Action
 * @{
 */

#define TIM_ForcedAction_Active ((uint16_t)0x0050)
#define TIM_ForcedAction_InActive ((uint16_t)0x0040)
#define IS_TIM_FORCED_ACTION(ACTION)                                           \
    (((ACTION) == TIM_ForcedAction_Active) ||                                  \
     ((ACTION) == TIM_ForcedAction_InActive))
/**
 * @}
 */

/** @defgroup TIM_Encoder_Mode
 * @{
 */

#define TIM_EncoderMode_TI1 ((uint16_t)0x0001)
#define TIM_EncoderMode_TI2 ((uint16_t)0x0002)
#define TIM_EncoderMode_TI12 ((uint16_t)0x0003)
#define IS_TIM_ENCODER_MODE(MODE)                                              \
    (((MODE) == TIM_EncoderMode_TI1) || ((MODE) == TIM_EncoderMode_TI2) ||     \
     ((MODE) == TIM_EncoderMode_TI12))
/**
 * @}
 */

/** @defgroup TIM_Event_Source
 * @{
 */

#define TIM_EventSource_Update ((uint16_t)0x0001)
#define TIM_EventSource_CC1 ((uint16_t)0x0002)
#define TIM_EventSource_CC2 ((uint16_t)0x0004)
#define TIM_EventSource_CC3 ((uint16_t)0x0008)
#define TIM_EventSource_CC4 ((uint16_t)0x0010)
#define TIM_EventSource_COM ((uint16_t)0x0020)
#define TIM_EventSource_Trigger ((uint16_t)0x0040)
#define TIM_EventSource_Break ((uint16_t)0x0080)
#define IS_TIM_EVENT_SOURCE(SOURCE)                                            \
    ((((SOURCE) & (uint16_t)0xFF00) == 0x0000) && ((SOURCE) != 0x0000))

/**
 * @}
 */

/** @defgroup TIM_Update_Source
 * @{
 */

#define TIM_UpdateSource_Global                                                \
    ((uint16_t)0x0000) /*!< Source of update is the counter overflow/underflow \
                            or the setting of UG bit, or an update generation  \
                            through the slave mode controller. */
#define TIM_UpdateSource_Regular                                               \
    ((uint16_t)0x0001) /*!< Source of update is counter overflow/underflow. */
#define IS_TIM_UPDATE_SOURCE(SOURCE)                                           \
    (((SOURCE) == TIM_UpdateSource_Global) ||                                  \
     ((SOURCE) == TIM_UpdateSource_Regular))
/**
 * @}
 */

/** @defgroup TIM_Output_Compare_Preload_State
 * @{
 */

#define TIM_OCPreload_Enable ((uint16_t)0x0008)
#define TIM_OCPreload_Disable ((uint16_t)0x0000)
#define IS_TIM_OCPRELOAD_STATE(STATE)                                          \
    (((STATE) == TIM_OCPreload_Enable) || ((STATE) == TIM_OCPreload_Disable))
/**
 * @}
 */

/** @defgroup TIM_Output_Compare_Fast_State
 * @{
 */

#define TIM_OCFast_Enable ((uint16_t)0x0004)
#define TIM_OCFast_Disable ((uint16_t)0x0000)
#define IS_TIM_OCFAST_STATE(STATE)                                             \
    (((STATE) == TIM_OCFast_Enable) || ((STATE) == TIM_OCFast_Disable))

/**
 * @}
 */

/** @defgroup TIM_Output_Compare_Clear_State
 * @{
 */

#define TIM_OCClear_Enable ((uint16_t)0x0080)
#define TIM_OCClear_Disable ((uint16_t)0x0000)
#define IS_TIM_OCCLEAR_STATE(STATE)                                            \
    (((STATE) == TIM_OCClear_Enable) || ((STATE) == TIM_OCClear_Disable))
/**
 * @}
 */

/** @defgroup TIM_Trigger_Output_Source
 * @{
 */

#define TIM_TRGOSource_Reset ((uint16_t)0x0000)
#define TIM_TRGOSource_Enable ((uint16_t)0x0010)
#define TIM_TRGOSource_Update ((uint16_t)0x0020)
#define TIM_TRGOSource_OC1 ((uint16_t)0x0030)
#define TIM_TRGOSource_OC1Ref ((uint16_t)0x0040)
#define TIM_TRGOSource_OC2Ref ((uint16_t)0x0050)
#define TIM_TRGOSource_OC3Ref ((uint16_t)0x0060)
#define TIM_TRGOSource_OC4Ref ((uint16_t)0x0070)
#define IS_TIM_TRGO_SOURCE(SOURCE)                                             \
    (((SOURCE) == TIM_TRGOSource_Reset) ||                                     \
     ((SOURCE) == TIM_TRGOSource_Enable) ||                                    \
     ((SOURCE) == TIM_TRGOSource_Update) ||                                    \
     ((SOURCE) == TIM_TRGOSource_OC1) ||                                       \
     ((SOURCE) == TIM_TRGOSource_OC1Ref) ||                                    \
     ((SOURCE) == TIM_TRGOSource_OC2Ref) ||                                    \
     ((SOURCE) == TIM_TRGOSource_OC3Ref) ||                                    \
     ((SOURCE) == TIM_TRGOSource_OC4Ref))
/**
 * @}
 */

/** @defgroup TIM_Slave_Mode
 * @{
 */

#define TIM_SlaveMode_Reset ((uint16_t)0x0004)
#define TIM_SlaveMode_Gated ((uint16_t)0x0005)
#define TIM_SlaveMode_Trigger ((uint16_t)0x0006)
#define TIM_SlaveMode_External1 ((uint16_t)0x0007)
#define IS_TIM_SLAVE_MODE(MODE)                                                \
    (((MODE) == TIM_SlaveMode_Reset) || ((MODE) == TIM_SlaveMode_Gated) ||     \
     ((MODE) == TIM_SlaveMode_Trigger) || ((MODE) == TIM_SlaveMode_External1))
/**
 * @}
 */

/** @defgroup TIM_Master_Slave_Mode
 * @{
 */

#define TIM_MasterSlaveMode_Enable ((uint16_t)0x0080)
#define TIM_MasterSlaveMode_Disable ((uint16_t)0x0000)
#define IS_TIM_MSM_STATE(STATE)                                                \
    (((STATE) == TIM_MasterSlaveMode_Enable) ||                                \
     ((STATE) == TIM_MasterSlaveMode_Disable))
/**
 * @}
 */
/** @defgroup TIM_Remap
 * @{
 */

#define TIM2_TIM8_TRGO ((uint16_t)0x0000)
#define TIM2_ETH_PTP ((uint16_t)0x0400)
#define TIM2_USBFS_SOF ((uint16_t)0x0800)
#define TIM2_USBHS_SOF ((uint16_t)0x0C00)

#define TIM5_GPIO ((uint16_t)0x0000)
#define TIM5_LSI ((uint16_t)0x0040)
#define TIM5_LSE ((uint16_t)0x0080)
#define TIM5_RTC ((uint16_t)0x00C0)

#define TIM11_GPIO ((uint16_t)0x0000)
#define TIM11_HSE ((uint16_t)0x0002)

#define IS_TIM_REMAP(TIM_REMAP)                                                \
    (((TIM_REMAP) == TIM2_TIM8_TRGO) || ((TIM_REMAP) == TIM2_ETH_PTP) ||       \
     ((TIM_REMAP) == TIM2_USBFS_SOF) || ((TIM_REMAP) == TIM2_USBHS_SOF) ||     \
     ((TIM_REMAP) == TIM5_GPIO) || ((TIM_REMAP) == TIM5_LSI) ||                \
     ((TIM_REMAP) == TIM5_LSE) || ((TIM_REMAP) == TIM5_RTC) ||                 \
     ((TIM_REMAP) == TIM11_GPIO) || ((TIM_REMAP) == TIM11_HSE))

/**
 * @}
 */
/** @defgroup TIM_Flags
 * @{
 */

#define TIM_FLAG_Update ((uint16_t)0x0001)
#define TIM_FLAG_CC1 ((uint16_t)0x0002)
#define TIM_FLAG_CC2 ((uint16_t)0x0004)
#define TIM_FLAG_CC3 ((uint16_t)0x0008)
#define TIM_FLAG_CC4 ((uint16_t)0x0010)
#define TIM_FLAG_COM ((uint16_t)0x0020)
#define TIM_FLAG_Trigger ((uint16_t)0x0040)
#define TIM_FLAG_Break ((uint16_t)0x0080)
#define TIM_FLAG_CC1OF ((uint16_t)0x0200)
#define TIM_FLAG_CC2OF ((uint16_t)0x0400)
#define TIM_FLAG_CC3OF ((uint16_t)0x0800)
#define TIM_FLAG_CC4OF ((uint16_t)0x1000)
#define IS_TIM_GET_FLAG(FLAG)                                                  \
    (((FLAG) == TIM_FLAG_Update) || ((FLAG) == TIM_FLAG_CC1) ||                \
     ((FLAG) == TIM_FLAG_CC2) || ((FLAG) == TIM_FLAG_CC3) ||                   \
     ((FLAG) == TIM_FLAG_CC4) || ((FLAG) == TIM_FLAG_COM) ||                   \
     ((FLAG) == TIM_FLAG_Trigger) || ((FLAG) == TIM_FLAG_Break) ||             \
     ((FLAG) == TIM_FLAG_CC1OF) || ((FLAG) == TIM_FLAG_CC2OF) ||               \
     ((FLAG) == TIM_FLAG_CC3OF) || ((FLAG) == TIM_FLAG_CC4OF))

/**
 * @}
 */

/** @defgroup TIM_Input_Capture_Filer_Value
 * @{
 */

#define IS_TIM_IC_FILTER(ICFILTER) ((ICFILTER) <= 0xF)
/**
 * @}
 */

/** @defgroup TIM_External_Trigger_Filter
 * @{
 */

#define IS_TIM_EXT_FILTER(EXTFILTER) ((EXTFILTER) <= 0xF)
/**
 * @}
 */

/** @defgroup TIM_Legacy
 * @{
 */

#define TIM_DMABurstLength_1Byte TIM_DMABurstLength_1Transfer
#define TIM_DMABurstLength_2Bytes TIM_DMABurstLength_2Transfers
#define TIM_DMABurstLength_3Bytes TIM_DMABurstLength_3Transfers
#define TIM_DMABurstLength_4Bytes TIM_DMABurstLength_4Transfers
#define TIM_DMABurstLength_5Bytes TIM_DMABurstLength_5Transfers
#define TIM_DMABurstLength_6Bytes TIM_DMABurstLength_6Transfers
#define TIM_DMABurstLength_7Bytes TIM_DMABurstLength_7Transfers
#define TIM_DMABurstLength_8Bytes TIM_DMABurstLength_8Transfers
#define TIM_DMABurstLength_9Bytes TIM_DMABurstLength_9Transfers
#define TIM_DMABurstLength_10Bytes TIM_DMABurstLength_10Transfers
#define TIM_DMABurstLength_11Bytes TIM_DMABurstLength_11Transfers
#define TIM_DMABurstLength_12Bytes TIM_DMABurstLength_12Transfers
#define TIM_DMABurstLength_13Bytes TIM_DMABurstLength_13Transfers
#define TIM_DMABurstLength_14Bytes TIM_DMABurstLength_14Transfers
#define TIM_DMABurstLength_15Bytes TIM_DMABurstLength_15Transfers
#define TIM_DMABurstLength_16Bytes TIM_DMABurstLength_16Transfers
#define TIM_DMABurstLength_17Bytes TIM_DMABurstLength_17Transfers
#define TIM_DMABurstLength_18Bytes TIM_DMABurstLength_18Transfers

#endif
//#endif