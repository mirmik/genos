#include <stdbool.h>
#include <zillot/stm32/stm32_timer.h>

void TIM_ITConfig(TIM_TypeDef *TIMx, uint16_t TIM_IT, FunctionalState NewState);
void TIM_Cmd(TIM_TypeDef *TIMx, FunctionalState NewState);

//#if 0
/**
 * @brief  Fills each TIM_OCInitStruct member with its default value.
 * @param  TIM_OCInitStruct: pointer to a TIM_OCInitTypeDef structure which will
 *         be initialized.
 * @retval None
 */
void TIM_OCStructInit(TIM_OCInitTypeDef *TIM_OCInitStruct)
{
    /* Set the default configuration */
    TIM_OCInitStruct->TIM_OCMode = TIM_OCMode_Timing;
    TIM_OCInitStruct->TIM_OutputState = TIM_OutputState_Disable;
    TIM_OCInitStruct->TIM_OutputNState = TIM_OutputNState_Disable;
    TIM_OCInitStruct->TIM_Pulse = 0x00000000;
    TIM_OCInitStruct->TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStruct->TIM_OCNPolarity = TIM_OCPolarity_High;
    TIM_OCInitStruct->TIM_OCIdleState = TIM_OCIdleState_Reset;
    TIM_OCInitStruct->TIM_OCNIdleState = TIM_OCNIdleState_Reset;
}

void TIM_OC1Init(TIM_TypeDef *TIMx, TIM_OCInitTypeDef *TIM_OCInitStruct)
{
    uint16_t tmpccmrx = 0, tmpccer = 0, tmpcr2 = 0;

    /* Disable the Channel 1: Reset the CC1E Bit */
    TIMx->CCER &= (uint16_t)~TIM_CCER_CC1E;

    /* Get the TIMx CCER register value */
    tmpccer = TIMx->CCER;
    /* Get the TIMx CR2 register value */
    tmpcr2 = TIMx->CR2;

    /* Get the TIMx CCMR1 register value */
    tmpccmrx = TIMx->CCMR1;

    /* Reset the Output Compare Mode Bits */
    tmpccmrx &= (uint16_t)~TIM_CCMR1_OC1M;
    tmpccmrx &= (uint16_t)~TIM_CCMR1_CC1S;
    /* Select the Output Compare Mode */
    tmpccmrx |= TIM_OCInitStruct->TIM_OCMode;

    /* Reset the Output Polarity level */
    tmpccer &= (uint16_t)~TIM_CCER_CC1P;
    /* Set the Output Compare Polarity */
    tmpccer |= TIM_OCInitStruct->TIM_OCPolarity;

    /* Set the Output State */
    tmpccer |= TIM_OCInitStruct->TIM_OutputState;

    if (false
#ifdef TIM1
        || (TIMx == TIM1)
#endif
#ifdef TIM8
        || (TIMx == TIM8)
#endif
    )
    {
        /* Reset the Output N Polarity level */
        tmpccer &= (uint16_t)~TIM_CCER_CC1NP;
        /* Set the Output N Polarity */
        tmpccer |= TIM_OCInitStruct->TIM_OCNPolarity;
        /* Reset the Output N State */
        tmpccer &= (uint16_t)~TIM_CCER_CC1NE;

        /* Set the Output N State */
        tmpccer |= TIM_OCInitStruct->TIM_OutputNState;
        /* Reset the Output Compare and Output Compare N IDLE State */
        tmpcr2 &= (uint16_t)~TIM_CR2_OIS1;
        tmpcr2 &= (uint16_t)~TIM_CR2_OIS1N;
        /* Set the Output Idle state */
        tmpcr2 |= TIM_OCInitStruct->TIM_OCIdleState;
        /* Set the Output N Idle state */
        tmpcr2 |= TIM_OCInitStruct->TIM_OCNIdleState;
    }
    /* Write to TIMx CR2 */
    TIMx->CR2 = tmpcr2;

    /* Write to TIMx CCMR1 */
    TIMx->CCMR1 = tmpccmrx;

    /* Set the Capture Compare Register value */
    TIMx->CCR1 = TIM_OCInitStruct->TIM_Pulse;

    /* Write to TIMx CCER */
    TIMx->CCER = tmpccer;
}

/**
 * @brief  Configures the TIMx Encoder Interface.
 * @param  TIMx: where x can be 1, 2, 3, 4, 5, 8, 9 or 12 to select the TIM
 *         peripheral.
 * @param  TIM_EncoderMode: specifies the TIMx Encoder Mode.
 *          This parameter can be one of the following values:
 *            @arg TIM_EncoderMode_TI1: Counter counts on TI1FP1 edge depending
 * on TI2FP2 level.
 *            @arg TIM_EncoderMode_TI2: Counter counts on TI2FP2 edge depending
 * on TI1FP1 level.
 *            @arg TIM_EncoderMode_TI12: Counter counts on both TI1FP1 and
 * TI2FP2 edges depending on the level of the other input.
 * @param  TIM_IC1Polarity: specifies the IC1 Polarity
 *          This parameter can be one of the following values:
 *            @arg TIM_ICPolarity_Falling: IC Falling edge.
 *            @arg TIM_ICPolarity_Rising: IC Rising edge.
 * @param  TIM_IC2Polarity: specifies the IC2 Polarity
 *          This parameter can be one of the following values:
 *            @arg TIM_ICPolarity_Falling: IC Falling edge.
 *            @arg TIM_ICPolarity_Rising: IC Rising edge.
 * @retval None
 */

void TIM_EncoderInterfaceConfig(TIM_TypeDef *TIMx,
                                uint16_t TIM_EncoderMode,
                                uint16_t TIM_IC1Polarity,
                                uint16_t TIM_IC2Polarity)
{
    uint16_t tmpsmcr = 0;
    uint16_t tmpccmr1 = 0;
    uint16_t tmpccer = 0;

    /* Get the TIMx SMCR register value */
    tmpsmcr = TIMx->SMCR;

    /* Get the TIMx CCMR1 register value */
    tmpccmr1 = TIMx->CCMR1;

    /* Get the TIMx CCER register value */
    tmpccer = TIMx->CCER;

    /* Set the encoder Mode */
    tmpsmcr &= (uint16_t)~TIM_SMCR_SMS;
    tmpsmcr |= TIM_EncoderMode;

    /* Select the Capture Compare 1 and the Capture Compare 2 as input */
    tmpccmr1 &= ((uint16_t)~TIM_CCMR1_CC1S) & ((uint16_t)~TIM_CCMR1_CC2S);
    tmpccmr1 |= TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_0;

    /* Set the TI1 and the TI2 Polarities */
    tmpccer &= ((uint16_t)~TIM_CCER_CC1P) & ((uint16_t)~TIM_CCER_CC2P);
    tmpccer |= (uint16_t)(TIM_IC1Polarity |
                          (uint16_t)(TIM_IC2Polarity << (uint16_t)4));

    /* Write to TIMx SMCR */
    TIMx->SMCR = tmpsmcr;

    /* Write to TIMx CCMR1 */
    TIMx->CCMR1 = tmpccmr1;

    /* Write to TIMx CCER */
    TIMx->CCER = tmpccer;
}

/**
 * @brief  Enables or disables the specified TIM interrupts.
 * @param  TIMx: where x can be 1 to 14 to select the TIMx peripheral.
 * @param  TIM_IT: specifies the TIM interrupts sources to be enabled or
 * disabled. This parameter can be any combination of the following values:
 *            @arg TIM_IT_Update: TIM update Interrupt source
 *            @arg TIM_IT_CC1: TIM Capture Compare 1 Interrupt source
 *            @arg TIM_IT_CC2: TIM Capture Compare 2 Interrupt source
 *            @arg TIM_IT_CC3: TIM Capture Compare 3 Interrupt source
 *            @arg TIM_IT_CC4: TIM Capture Compare 4 Interrupt source
 *            @arg TIM_IT_COM: TIM Commutation Interrupt source
 *            @arg TIM_IT_Trigger: TIM Trigger Interrupt source
 *            @arg TIM_IT_Break: TIM Break Interrupt source
 *
 * @note   For TIM6 and TIM7 only the parameter TIM_IT_Update can be used
 * @note   For TIM9 and TIM12 only one of the following parameters can be used:
 * TIM_IT_Update, TIM_IT_CC1, TIM_IT_CC2 or TIM_IT_Trigger.
 * @note   For TIM10, TIM11, TIM13 and TIM14 only one of the following
 * parameters can be used: TIM_IT_Update or TIM_IT_CC1
 * @note   TIM_IT_COM and TIM_IT_Break can be used only with TIM1 and TIM8
 *
 * @param  NewState: new state of the TIM interrupts.
 *          This parameter can be: ENABLE or DISABLE.
 * @retval None
 */
void TIM_ITConfig(TIM_TypeDef *TIMx, uint16_t TIM_IT, FunctionalState NewState)
{
    if (NewState != DISABLE)
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

/**
 * @brief  Enables or disables the specified TIM peripheral.
 * @param  TIMx: where x can be 1 to 14 to select the TIMx peripheral.
 * @param  NewState: new state of the TIMx peripheral.
 *          This parameter can be: ENABLE or DISABLE.
 * @retval None
 */
void TIM_Cmd(TIM_TypeDef *TIMx, FunctionalState NewState)
{
    if (NewState != DISABLE)
    {
        /* Enable the TIM Counter */
        TIMx->CR1 |= TIM_CR1_CEN;
    }
    else
    {
        /* Disable the TIM Counter */
        TIMx->CR1 &= (uint16_t)~TIM_CR1_CEN;
    }
}

void TIM_ClearITPendingBit(TIM_TypeDef *TIMx, uint16_t TIM_IT)
{
    TIMx->SR = (uint16_t)~TIM_IT;
}

ITStatus TIM_GetITStatus(TIM_TypeDef *TIMx, uint16_t TIM_IT)
{
    ITStatus bitstatus = RESET;
    uint16_t itstatus = 0x0, itenable = 0x0;
    /* Check the parameters */

    itstatus = TIMx->SR & TIM_IT;

    itenable = TIMx->DIER & TIM_IT;
    if ((itstatus != (uint16_t)RESET) && (itenable != (uint16_t)RESET))
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }
    return bitstatus;
}

void TIM_OC3Init(TIM_TypeDef *TIMx, TIM_OCInitTypeDef *TIM_OCInitStruct)
{
    uint16_t tmpccmrx = 0, tmpccer = 0, tmpcr2 = 0;

    /* Disable the Channel 3: Reset the CC2E Bit */
    TIMx->CCER &= (uint16_t)~TIM_CCER_CC3E;

    /* Get the TIMx CCER register value */
    tmpccer = TIMx->CCER;
    /* Get the TIMx CR2 register value */
    tmpcr2 = TIMx->CR2;

    /* Get the TIMx CCMR2 register value */
    tmpccmrx = TIMx->CCMR2;

    /* Reset the Output Compare mode and Capture/Compare selection Bits */
    tmpccmrx &= (uint16_t)~TIM_CCMR2_OC3M;
    tmpccmrx &= (uint16_t)~TIM_CCMR2_CC3S;
    /* Select the Output Compare Mode */
    tmpccmrx |= TIM_OCInitStruct->TIM_OCMode;

    /* Reset the Output Polarity level */
    tmpccer &= (uint16_t)~TIM_CCER_CC3P;
    /* Set the Output Compare Polarity */
    tmpccer |= (uint16_t)(TIM_OCInitStruct->TIM_OCPolarity << 8);

    /* Set the Output State */
    tmpccer |= (uint16_t)(TIM_OCInitStruct->TIM_OutputState << 8);

    if (false
#ifdef TIM1
        || (TIMx == TIM1)
#endif
#ifdef TIM8
        || (TIMx == TIM8)
#endif
    )
    {
        /* Reset the Output N Polarity level */
        tmpccer &= (uint16_t)~TIM_CCER_CC3NP;
        /* Set the Output N Polarity */
        tmpccer |= (uint16_t)(TIM_OCInitStruct->TIM_OCNPolarity << 8);
        /* Reset the Output N State */
        tmpccer &= (uint16_t)~TIM_CCER_CC3NE;

        /* Set the Output N State */
        tmpccer |= (uint16_t)(TIM_OCInitStruct->TIM_OutputNState << 8);
        /* Reset the Output Compare and Output Compare N IDLE State */
        tmpcr2 &= (uint16_t)~TIM_CR2_OIS3;
        tmpcr2 &= (uint16_t)~TIM_CR2_OIS3N;
        /* Set the Output Idle state */
        tmpcr2 |= (uint16_t)(TIM_OCInitStruct->TIM_OCIdleState << 4);
        /* Set the Output N Idle state */
        tmpcr2 |= (uint16_t)(TIM_OCInitStruct->TIM_OCNIdleState << 4);
    }
    /* Write to TIMx CR2 */
    TIMx->CR2 = tmpcr2;

    /* Write to TIMx CCMR2 */
    TIMx->CCMR2 = tmpccmrx;

    /* Set the Capture Compare Register value */
    TIMx->CCR3 = TIM_OCInitStruct->TIM_Pulse;

    /* Write to TIMx CCER */
    TIMx->CCER = tmpccer;
}

//#endif