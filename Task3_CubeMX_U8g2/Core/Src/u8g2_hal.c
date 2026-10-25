#include "u8g2_hal.h"
#include "main.h"
#include "i2c.h"
#include <string.h>

/* SSD1306 的 8 位写地址：0x3C << 1 */
#define OLED_I2C_ADDR   0x78
#define I2C_TIMEOUT_MS  100

u8g2_t u8g2;

static uint8_t  i2c_buf[256];
static uint16_t i2c_len;

/*
 * U8g2 的字节回调：
 * - START_TRANSFER 清空缓冲；
 * - SEND 把控制字节和数据追加到缓冲；
 * - END_TRANSFER 一次性通过 HAL 发出。
 */
uint8_t u8x8_byte_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    (void)u8x8;

    switch (msg)
    {
    case U8X8_MSG_BYTE_SEND:
        if (i2c_len + arg_int > sizeof(i2c_buf))
        {
            return 0;
        }
        memcpy(&i2c_buf[i2c_len], arg_ptr, arg_int);
        i2c_len += arg_int;
        break;

    case U8X8_MSG_BYTE_INIT:
        i2c_len = 0;
        break;

    case U8X8_MSG_BYTE_START_TRANSFER:
        i2c_len = 0;
        break;

    case U8X8_MSG_BYTE_SET_DC:
        break;

    case U8X8_MSG_BYTE_END_TRANSFER:
        if (i2c_len == 0)
        {
            return 1;
        }
        if (HAL_I2C_Master_Transmit(&hi2c1, OLED_I2C_ADDR,
                                    i2c_buf, i2c_len, I2C_TIMEOUT_MS) != HAL_OK)
        {
            return 0;
        }
        i2c_len = 0;
        break;

    default:
        return 0;
    }

    return 1;
}

uint8_t u8x8_gpio_and_delay_cb(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    (void)u8x8;
    (void)arg_ptr;

    switch (msg)
    {
    case U8X8_MSG_GPIO_AND_DELAY_INIT:
        break;

    case U8X8_MSG_DELAY_MILLI:
        HAL_Delay(arg_int);
        break;

    case U8X8_MSG_DELAY_10MICRO:
        /* 粗略延时：72 MHz 下约 10 us 的循环 */
        for (volatile uint32_t i = 0; i < (uint32_t)arg_int * 120U; i++)
        {
            __NOP();
        }
        break;

    case U8X8_MSG_DELAY_100NANO:
    case U8X8_MSG_DELAY_NANO:
    case U8X8_MSG_DELAY_I2C:
        break;

    default:
        break;
    }

    return 1;
}

void U8g2_Init(void)
{
    u8g2_Setup_ssd1306_i2c_128x64_noname_f(
        &u8g2,
        U8G2_R0,
        u8x8_byte_hw_i2c,
        u8x8_gpio_and_delay_cb);

    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0);
    u8g2_ClearBuffer(&u8g2);
    u8g2_SendBuffer(&u8g2);
}
