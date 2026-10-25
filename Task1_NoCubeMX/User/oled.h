#ifndef __OLED_H
#define __OLED_H

#include "stm32f10x.h"

void OLED_Init(void);
void OLED_Clear(void);
void OLED_Refresh(void);
void OLED_SetPixel(int16_t x, int16_t y, uint8_t color);
void OLED_ShowChar(int16_t x, int16_t y, char ch);
void OLED_ShowString(int16_t x, int16_t y, const char *str);
void OLED_ShowUTF8(int16_t x, int16_t y, const char *str);
uint16_t OLED_UTF8Width(const char *str);
void OLED_ScrollUTF8(const char *str, uint8_t y);
void OLED_ScrollUTF8Vertical(const char *str);

#endif

