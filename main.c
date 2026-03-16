#include "rcc_hal.h"
#include "gpio_hal.h"
#include "stm32_def.h"

#include <stdint.h>



int main(void) {

    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = 0; // PA0
    GPIO_InitStruct.Mode = 1; // Output
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    while (1) {
        GPIO_TogglePin(GPIOA, 0);
        for (volatile uint32_t i = 0; i < 100000; i++); // Simple delay
        }
}
