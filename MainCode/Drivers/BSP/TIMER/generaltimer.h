#ifndef __GENERALTIMER_H
#define __GENERALTIMER_H

#include "./SYSTEM/sys/sys.h"
#include "header.h"

void gtim_timx_pwm_chy_init(void);
void btim_timx_int_init(uint16_t psc,uint16_t arr);


#endif
