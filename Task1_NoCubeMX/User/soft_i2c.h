#ifndef __SOFT_I2C_H
#define __SOFT_I2C_H

#include "stm32f10x.h"

void SoftI2C_Init(void);
void SoftI2C_Start(void);
void SoftI2C_Stop(void);
uint8_t SoftI2C_WriteByte(uint8_t data);
uint8_t SoftI2C_ReadByte(uint8_t ack);

#endif
