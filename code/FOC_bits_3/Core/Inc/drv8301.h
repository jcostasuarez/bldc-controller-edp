#ifndef __DRV8301_H
#define __DRV8301_H

#include "main.h"

void drv8301_init(void);
void drv8301_write_reg(int reg, int data);
uint16_t drv8301_read_reg(int reg);

#endif // __DRV8301_H
