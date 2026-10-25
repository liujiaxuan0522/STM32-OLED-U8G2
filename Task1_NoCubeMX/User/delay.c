#include "delay.h"

void Delay_us(uint32_t us)
{
    while (us--)
    {
        for (volatile uint32_t i = 0; i < 18; i++)
        {
            __NOP();
        }
    }
}

void Delay_ms(uint32_t ms)
{
    while (ms--)
    {
        Delay_us(1000);
    }
}

