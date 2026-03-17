#include <stdint.h>
#include "rcc_hal.h"
#include "gpio_hal.h"

int main(void)
{
    // 1. Enable GPIOA clock
    __HAL_RCC_GPIOA_CLK_ENABLE();
    // 2. Configure PA5 as output
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = 5; // PA5
    GPIO_InitStruct.Mode = 1; // Output mode
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    // 3. Main loop: Toggle PA5

    while(1) {
        GPIO_TogglePin(GPIOA, 5); // Toggle PA5
        for (volatile uint32_t i = 0; i < 10000; i++); // Simple delay
    }
}