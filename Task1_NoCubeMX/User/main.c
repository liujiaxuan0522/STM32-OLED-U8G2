#include "stm32f10x.h"
#include "soft_i2c.h"
#include "oled.h"
#include "delay.h"

/* 改成自己的姓名/学号；中文姓名需要把对应汉字加入 tools/gen_font.py 的 CHARS */
#define MY_NAME   "刘佳烜"
#define MY_ID     "632407030115"

int main(void)
{
    SoftI2C_Init();
    OLED_Init();

    OLED_Clear();
    OLED_ShowUTF8(0, 12, "姓名：" MY_NAME);
    OLED_ShowUTF8(0, 36, "学号" MY_ID);
    OLED_Refresh();
    Delay_ms(800);

    while (1)
    {
        OLED_ScrollUTF8("Hello，欢迎来到重庆交通大学物联网205实训室！", 24);
        Delay_ms(100);
    }
}



