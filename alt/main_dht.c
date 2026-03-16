#include <stdint.h>

// --- Register Definitions ---
#define RCC_BASE      0x40023800
#define RCC_AHB1ENR   *(volatile uint32_t*)(RCC_BASE + 0x30)
#define RCC_APB1ENR   *(volatile uint32_t*)(RCC_BASE + 0x40)

#define GPIOA_BASE    0x40020000
#define GPIOA_MODER   *(volatile uint32_t*)(GPIOA_BASE + 0x00)
#define GPIOA_IDR     *(volatile uint32_t*)(GPIOA_BASE + 0x10)
#define GPIOA_BSRR    *(volatile uint32_t*)(GPIOA_BASE + 0x18)
#define GPIOA_AFRL    *(volatile uint32_t*)(GPIOA_BASE + 0x20)

#define USART2_BASE   0x40004400
#define USART2_SR     *(volatile uint32_t*)(USART2_BASE + 0x00)
#define USART2_DR     *(volatile uint32_t*)(USART2_BASE + 0x04)
#define USART2_BRR    *(volatile uint32_t*)(USART2_BASE + 0x08)
#define USART2_CR1    *(volatile uint32_t*)(USART2_BASE + 0x0C)

#define TIM2_BASE     0x40000000
#define TIM2_CR1      *(volatile uint32_t*)(TIM2_BASE + 0x00)
#define TIM2_PSC      *(volatile uint32_t*)(TIM2_BASE + 0x28)
#define TIM2_ARR      *(volatile uint32_t*)(TIM2_BASE + 0x2C)
#define TIM2_CNT      *(volatile uint32_t*)(TIM2_BASE + 0x24)

// --- UART Functions ---
void UART2_Init(void) {
    RCC_AHB1ENR |= (1 << 0);   // Enable GPIOA Clock
    RCC_APB1ENR |= (1 << 17);  // Enable USART2 Clock

    // Configure PA2 (TX) as Alternate Function 7
    GPIOA_MODER &= ~(3 << (2 * 2));
    GPIOA_MODER |=  (2 << (2 * 2));
    GPIOA_AFRL  |=  (7 << (2 * 4));

    // Baud rate 115200 @ 16MHz (16,000,000 / 115,200 = 138.88)
    // Mantissa 8, Fraction 11 -> 0x8B
    USART2_BRR = 0x8B;
    USART2_CR1 |= (1 << 13) | (1 << 3); // Enable UE and TE
}

void UART_SendChar(char c) {
    while (!(USART2_SR & (1 << 7))); // Wait for TXE
    USART2_DR = c;
}

void UART_SendString(char* str) {
    while (*str) UART_SendChar(*str++);
}

void UART_SendInt(int n) {
    char buf[10];
    int i = 0;
    if (n == 0) { UART_SendChar('0'); return; }
    while (n > 0) { buf[i++] = (n % 10) + '0'; n /= 10; }
    while (i > 0) { UART_SendChar(buf[--i]); }
}

// --- DHT11 Helper Functions ---
void delay_us(uint32_t us) {
    uint32_t start = TIM2_CNT;
    while ((TIM2_CNT - start) < us);
}

void delay_ms(uint32_t ms) {
    for (uint32_t i = 0; i < ms; i++) delay_us(1000);
}

// --- Main ---
int main(void) {
    // Enable Peripheral Clocks
    RCC_AHB1ENR |= (1 << 0); // GPIOA
    RCC_APB1ENR |= (1 << 0); // TIM2
    
    UART2_Init();

    // Timer setup (1us ticks at 16MHz)
    TIM2_PSC = 16 - 1;
    TIM2_ARR = 0xFFFFFFFF;
    TIM2_CR1 |= (1 << 0);

    uint8_t data[5];

    while (1) {
        // Start Signal
        GPIOA_MODER |= (1 << (0 * 2)); // Output
        GPIOA_BSRR = (1 << 16);        // LOW
        delay_ms(18);
        GPIOA_BSRR = (1 << 0);         // HIGH
        delay_us(30);
        
        // Switch to Input
        GPIOA_MODER &= ~(3 << (0 * 2)); 

        // Handshake
        while (GPIOA_IDR & (1 << 0));    // Wait for LOW
        while (!(GPIOA_IDR & (1 << 0))); // Wait for HIGH
        while (GPIOA_IDR & (1 << 0));    // Wait for Data start (LOW)

        // Read 40 bits
        for (int j = 0; j < 5; j++) {
            data[j] = 0;
            for (int i = 0; i < 8; i++) {
                while (!(GPIOA_IDR & (1 << 0))); // Wait for HIGH
                uint32_t start_bit = TIM2_CNT;
                while (GPIOA_IDR & (1 << 0));    // Wait for LOW
                if ((TIM2_CNT - start_bit) > 40) {
                    data[j] |= (1 << (7 - i));
                }
            }
        }

        // Output results to Serial
        if (((data[0] + data[1] + data[2] + data[3]) & 0xFF) == data[4]) {
            UART_SendString("Hum: ");
            UART_SendInt(data[0]);
            UART_SendString("% Temp: ");
            UART_SendInt(data[2]);
            UART_SendString("C\r\n");
        } else {
            UART_SendString("Checksum Error!\r\n");
        }

        delay_ms(2000); // DHT11 needs 2s between reads
    }
}