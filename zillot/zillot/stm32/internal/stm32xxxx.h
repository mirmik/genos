#ifndef STM32XXXX_H
#define STM32XXXX_H

#ifdef STM32G4XX
#include "stm32g4xx.h"
#elif defined(STM32F4XX)
#include "stm32f4xx.h"
#elif defined(STM32L4XX)
#include "stm32l4xx.h"
#elif defined(STM32F7XX)
#include "stm32f7xx.h"
#elif defined(STM32H7XX)
#include "stm32h7xx.h"
#else 
#error "undefined arch"
#endif

#endif