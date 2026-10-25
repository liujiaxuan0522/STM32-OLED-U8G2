#include "oled.h"
#include "soft_i2c.h"
#include "font_ascii.h"
#include "font_cn.h"
#include <string.h>

#define OLED_ADDR   0x78
#define OLED_CMD    0x00
#define OLED_DATA   0x40

static uint8_t OLED_GRAM[8][128];

static void OLED_WriteCmd(uint8_t cmd)
{
    SoftI2C_Start();
    SoftI2C_WriteByte(OLED_ADDR);
    SoftI2C_WriteByte(OLED_CMD);
    SoftI2C_WriteByte(cmd);
    SoftI2C_Stop();
}

static void OLED_WriteData(const uint8_t *data, uint16_t len)
{
    uint16_t i;

    SoftI2C_Start();
    SoftI2C_WriteByte(OLED_ADDR);
    SoftI2C_WriteByte(OLED_DATA);
    for (i = 0; i < len; i++)
    {
        SoftI2C_WriteByte(data[i]);
    }
    SoftI2C_Stop();
}

static void OLED_SetPos(uint8_t page, uint8_t col)
{
    OLED_WriteCmd((uint8_t)(0xB0 + page));
    OLED_WriteCmd((uint8_t)(0x00 + (col & 0x0F)));
    OLED_WriteCmd((uint8_t)(0x10 + ((col >> 4) & 0x0F)));
}

void OLED_Init(void)
{
    static const uint8_t init_cmds[] = {
        0xAE, 0xD5, 0x80, 0xA8, 0x3F, 0xD3, 0x00, 0x40,
        0x8D, 0x14, 0x20, 0x00, 0xA1, 0xC8, 0xDA, 0x12,
        0x81, 0xCF, 0xD9, 0xF1, 0xDB, 0x40, 0xA4, 0xA6,
        0x2E, 0xAF
    };
    uint16_t i;

    for (i = 0; i < sizeof(init_cmds); i++)
    {
        OLED_WriteCmd(init_cmds[i]);
    }

    OLED_Clear();
    OLED_Refresh();
}

void OLED_Clear(void)
{
    memset(OLED_GRAM, 0, sizeof(OLED_GRAM));
}

void OLED_SetPixel(int16_t x, int16_t y, uint8_t color)
{
    if (x < 0 || y < 0 || x >= 128 || y >= 64)
    {
        return;
    }

    if (color)
    {
        OLED_GRAM[y / 8][x] |= (uint8_t)(1 << (y % 8));
    }
    else
    {
        OLED_GRAM[y / 8][x] &= (uint8_t)~(1 << (y % 8));
    }
}

void OLED_Refresh(void)
{
    uint8_t page;

    for (page = 0; page < 8; page++)
    {
        OLED_SetPos(page, 0);
        OLED_WriteData(OLED_GRAM[page], 128);
    }
}

void OLED_ShowChar(int16_t x, int16_t y, char ch)
{
    uint8_t i, row;
    uint8_t code = (uint8_t)ch;

    if (code < 0x20 || code > 0x7E)
    {
        code = 0x20;
    }
    code -= 0x20;

    for (row = 0; row < 16; row++)
    {
        uint8_t bits = ascii_8x16[code * 16 + row];
        for (i = 0; i < 8; i++)
        {
            if (bits & (1 << (7 - i)))
            {
                OLED_SetPixel(x + i, y + row, 1);
            }
        }
    }
}

void OLED_ShowString(int16_t x, int16_t y, const char *str)
{
    while (*str)
    {
        OLED_ShowChar(x, y, *str++);
        x += 8;
    }
}

static const uint8_t *OLED_FindCN(uint16_t code)
{
    uint16_t i;

    for (i = 0; i < cn16_count; i++)
    {
        if (cn16_table[i].code == code)
        {
            return cn16_table[i].data;
        }
    }

    return 0;
}

static uint8_t OLED_ShowCN(int16_t x, int16_t y, uint16_t code)
{
    const uint8_t *data = OLED_FindCN(code);
    uint8_t row, col;

    if (data == 0)
    {
        return 0;
    }

    for (row = 0; row < 16; row++)
    {
        uint8_t hi = data[row * 2];
        uint8_t lo = data[row * 2 + 1];

        for (col = 0; col < 8; col++)
        {
            if (hi & (1 << (7 - col)))
            {
                OLED_SetPixel(x + col, y + row, 1);
            }
            if (lo & (1 << (7 - col)))
            {
                OLED_SetPixel(x + 8 + col, y + row, 1);
            }
        }
    }

    return 1;
}

void OLED_ShowUTF8(int16_t x, int16_t y, const char *str)
{
    const uint8_t *p = (const uint8_t *)str;

    while (*p)
    {
        if (*p < 0x80)
        {
            OLED_ShowChar(x, y, (char)*p);
            x += 8;
            p++;
        }
        else if ((*p & 0xE0) == 0xC0 && p[1])
        {
            p += 2;
            x += 8;
        }
        else if ((*p & 0xF0) == 0xE0 && p[1] && p[2])
        {
            uint16_t code = (uint16_t)(((uint16_t)(p[0] & 0x0F) << 12) |
                                       ((uint16_t)(p[1] & 0x3F) << 6) |
                                        (uint16_t)(p[2] & 0x3F));
            if (OLED_ShowCN(x, y, code))
            {
                x += 16;
            }
            else
            {
                x += 16;
            }
            p += 3;
        }
        else
        {
            p++;
            x += 8;
        }
    }
}

uint16_t OLED_UTF8Width(const char *str)
{
    const uint8_t *p = (const uint8_t *)str;
    uint16_t w = 0;

    while (*p)
    {
        if (*p < 0x80)
        {
            w += 8;
            p++;
        }
        else if ((*p & 0xE0) == 0xC0 && p[1])
        {
            w += 8;
            p += 2;
        }
        else if ((*p & 0xF0) == 0xE0 && p[1] && p[2])
        {
            w += 16;
            p += 3;
        }
        else
        {
            p++;
        }
    }

    return w;
}

void OLED_ScrollUTF8(const char *str, uint8_t y)
{
    int16_t x;
    int16_t width = (int16_t)OLED_UTF8Width(str);

    for (x = 96; x > -width; x -= 6)
    {
        OLED_Clear();
        OLED_ShowUTF8(x, y, str);
        OLED_Refresh();
    }
}

void OLED_ScrollUTF8Vertical(const char *str)
{
    int16_t y;
    uint16_t width = OLED_UTF8Width(str);
    uint8_t x = (width < 128) ? (uint8_t)((128 - width) / 2) : 0;

    for (y = 64; y > -16; y--)
    {
        OLED_Clear();
        OLED_ShowUTF8(x, y, str);
        OLED_Refresh();
    }
}






