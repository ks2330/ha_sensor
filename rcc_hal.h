
#ifndef RCC_HAL_H
#define RCC_HAL_H

#include <stdint.h>
#include "stm32_def.h"

typedef struct{
    volatile uint32_t CR;
    volatile uint32_t PLLCFGR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;
    volatile uint32_t AHB1RSTR;
    volatile uint32_t AHB2RSTR;
    uint32_t RESERVED0[2];
    volatile uint32_t APB1RSTR;
    volatile uint32_t APB2RSTR;
    uint32_t RESERVED1[2];
    volatile uint32_t AHB1ENR;
    volatile uint32_t AHB2ENR;
    volatile uint32_t AHB3ENR;
} RCC_TypeDef;


#define RCC ((RCC_TypeDef*)0x40023800)
#define RCC_AHB1ENR_GPIOAEN (1UL << 0)
#define __HAL_RCC_GPIOA_CLK_ENABLE() (SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN))

#endif // RCC_HAL_H