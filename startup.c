#include <stdint.h>

extern uint32_t _estack[];
extern uint32_t _sidata[];
extern uint32_t _sdata[];
extern uint32_t _edata[];
extern uint32_t _sbss[];
extern uint32_t _ebss[];

extern int main(void);

__attribute__((naked, aligned(4))) void Default_Handler(void) {
  while (1) {
  }
}

__attribute__((naked, aligned(4))) void Reset_Handler(void) {

  uint32_t *src = _sidata;
  uint32_t *dst = _sdata;
  while (dst < _edata) {
    *dst++ = *src++;
  }

  dst = _sbss;

  while (dst < _ebss) {
    *dst++ = 0;
  }

  main();
  while (1)
    ;
}

__attribute__((section(".init_sp"), aligned(4)))
const uint32_t *initial_stack_pointer = _estack;

__attribute__((section(".system_exceptions"), aligned(4))) void (*const system_exceptions[15])(void) = {
    [0 ... 14] = Default_Handler,
    [0] = Reset_Handler,
};
__attribute__((section(".peripheral_interrupts"), aligned(4))) void (*const peripheral_interrupts[85])(void) = {
    [0 ... 84] = Default_Handler,
};
