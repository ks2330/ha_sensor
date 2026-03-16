#include "gpio_hal.h"
#include "stm32_def.h"
#include <stdint.h>


/* GPIO Initialization Function */
/* Initializes the GPIO pins based on the provided configuration */
/* The Parameters are:
 *   GPIOx: Pointer to the GPIO port
 *   GPIO_InitStruct: Pointer to the GPIO initialization structure
 */

void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct) 
{
    // 1. Clear and Set the Mode bits
    uint32_t pos = GPIO_InitStruct->Pin * 2; // Each pin has 2 bits in MODER
    CLEAR_BIT(GPIOx->MODER, (3UL << pos)); // Clear existing mode
    SET_BIT(GPIOx->MODER, (GPIO_InitStruct->Mode << pos)); // Set new mode
}

void GPIO_WritePin(GPIO_TypeDef* GPIOx, uint32_t Pin, uint32_t State) 
{
    if (State) {
        SET_BIT(GPIOx->BSRR, (1UL << Pin)); // Set pin
    } else {
        SET_BIT(GPIOx->BSRR, (1UL << (Pin + 16))); // Reset pin
    }
}

void GPIO_TogglePin(GPIO_TypeDef* GPIOx, uint32_t Pin) 
{
    TOGGLE_BIT(GPIOx->ODR, (1UL << Pin)); // Toggle pin state
}

