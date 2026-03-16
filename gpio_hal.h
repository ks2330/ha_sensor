#ifndef GPIO_HAL_H
#define GPIO_HAL_H

#include <stdint.h>

typedef struct {
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
} GPIO_TypeDef;

typedef struct {
    uint32_t Pin;       // e.g., 5
    uint32_t Mode;      // e.g., GPIO_MODE_OUTPUT
} GPIO_InitTypeDef;

#define GPIOA ((GPIO_TypeDef*)0x40020000)
#define GPIOB ((GPIO_TypeDef*)0x40020400)

void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct);

void GPIO_WritePin(GPIO_TypeDef* GPIOx, uint32_t Pin, uint32_t State);
void GPIO_TogglePin(GPIO_TypeDef* GPIOx, uint32_t Pin);

#endif // GPIO_HAL_H