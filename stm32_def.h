#ifndef STM32_DEF_H
#define STM32_DEF_H

/* STM32 Definitions */
/* Basic Bit Operations */
#define SET_BIT(REG, BIT)     ((REG) |= (BIT))
#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))
#define READ_BIT(REG, BIT)    ((REG) & (BIT))
#define TOGGLE_BIT(REG, BIT)  ((REG) ^= (BIT))

#endif