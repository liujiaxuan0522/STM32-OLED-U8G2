#include "oled_app.h"
#include "u8g2_hal.h"
#include "avatar.h"
#include "main.h"

extern const uint8_t myfont16[];

static void OLED_Clear(void)
{
    u8g2_ClearBuffer(&u8g2);
}

static void OLED_Show(void)
{
    u8g2_SendBuffer(&u8g2);
}

void OLED_ShowID(void)
{
    OLED_Clear();
    u8g2_SetFont(&u8g2, myfont16);

    u8g2_DrawUTF8(&u8g2, 0, 14, "姓名：" STUDENT_NAME);
    u8g2_DrawUTF8(&u8g2, 0, 30, "学号" STUDENT_ID);

    u8g2_SetFont(&u8g2, u8g2_font_6x10_tf);
    u8g2_DrawStr(&u8g2, 0, 46, "STM32F103 + SSD1306");
    u8g2_DrawStr(&u8g2, 0, 58, "I2C1: PB6/PB7");

    OLED_Show();
}

void OLED_ShowInfo(void)
{
    OLED_Clear();
    u8g2_SetFont(&u8g2, myfont16);
    u8g2_DrawUTF8(&u8g2, 0, 14, "系统信息");

    u8g2_SetFont(&u8g2, u8g2_font_6x10_tf);
    u8g2_DrawStr(&u8g2, 0, 32, "MCU: STM32F103");
    u8g2_DrawStr(&u8g2, 0, 44, "OLED: SSD1306 128x64");
    u8g2_DrawStr(&u8g2, 0, 56, "GUI: U8g2 / HAL");

    OLED_Show();
}

void OLED_ScrollTextHorizontal(void)
{
    const char *text = "Hello，欢迎来到重庆交通大学物联网205实训室！";
    int width;
    int x;

    u8g2_SetFont(&u8g2, myfont16);
    width = u8g2_GetUTF8Width(&u8g2, text);

    for (x = 128; x > -width; x -= 2)
    {
        OLED_Clear();
        u8g2_DrawUTF8(&u8g2, x, 36, text);
        OLED_Show();
        HAL_Delay(25);
    }
}

void OLED_ScrollTextVertical(void)
{
    const char *lines[] = {
        "STM32F103",
        "0.96 OLED",
        "I2C1 PB6/PB7",
        "U8g2 GUI",
        "CQJTU IoT 205"
    };
    int count = (int)(sizeof(lines) / sizeof(lines[0]));
    int offset;

    u8g2_SetFont(&u8g2, u8g2_font_6x10_tf);

    for (offset = 64; offset > -count * 12; offset--)
    {
        int i;
        OLED_Clear();
        for (i = 0; i < count; i++)
        {
            u8g2_DrawStr(&u8g2, 8, offset + i * 12, lines[i]);
        }
        OLED_Show();
        HAL_Delay(30);
    }
}

void OLED_U8g2Demo(void)
{
    OLED_Clear();
    u8g2_SetFont(&u8g2, u8g2_font_6x10_tf);

    u8g2_DrawFrame(&u8g2, 0, 0, 128, 64);
    u8g2_DrawFrame(&u8g2, 4, 4, 120, 56);
    u8g2_DrawCircle(&u8g2, 24, 24, 12, U8G2_DRAW_ALL);
    u8g2_DrawDisc(&u8g2, 24, 24, 5, U8G2_DRAW_ALL);
    u8g2_DrawLine(&u8g2, 44, 12, 116, 12);
    u8g2_DrawLine(&u8g2, 44, 20, 116, 20);
    u8g2_DrawBox(&u8g2, 44, 28, 72, 10);
    u8g2_DrawStr(&u8g2, 46, 36, "U8g2 demo");
    u8g2_DrawStr(&u8g2, 8, 56, "CQJTU  IoT 205");

    OLED_Show();
}

void OLED_AvatarAnimation(void)
{
    int x;
    int i;

    for (i = 0; i < 200; i++)
    {
        x = (i * 2) % 65;

        OLED_Clear();
        u8g2_DrawXBMP(&u8g2, x, 0, 64, 64, avatar_bits);
        OLED_Show();
        HAL_Delay(30);
    }
}

