#ifndef __KEY_H
#define __KEY_H

#include "./SYSTEM/sys/sys.h"

void key_init(void);
uint8_t key_scan(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

#endif
