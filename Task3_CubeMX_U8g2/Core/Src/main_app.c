#include "app.h"
#include "main.h"
#include "u8g2_hal.h"
#include "oled_app.h"

void App_Init(void)
{
    U8g2_Init();
}

void App_Loop(void)
{
    OLED_ShowID();
    HAL_Delay(2500);

    OLED_ShowInfo();
    HAL_Delay(2500);

    OLED_ScrollTextHorizontal();
    OLED_ScrollTextVertical();

    OLED_U8g2Demo();
    HAL_Delay(3000);

    OLED_AvatarAnimation();
}
