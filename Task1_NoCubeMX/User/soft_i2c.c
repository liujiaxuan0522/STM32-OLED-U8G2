#include "soft_i2c.h"
#include "delay.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#define I2C_PORT    GPIOB
#define I2C_SCL     GPIO_Pin_6
#define I2C_SDA     GPIO_Pin_7

#define SCL_H()     GPIO_SetBits(I2C_PORT, I2C_SCL)
#define SCL_L()     GPIO_ResetBits(I2C_PORT, I2C_SCL)
#define SDA_H()     GPIO_SetBits(I2C_PORT, I2C_SDA)
#define SDA_L()     GPIO_ResetBits(I2C_PORT, I2C_SDA)
#define SDA_READ()  GPIO_ReadInputDataBit(I2C_PORT, I2C_SDA)

static void I2C_Delay(void)
{
    Delay_us(1);
}

void SoftI2C_Init(void)
{
    GPIO_InitTypeDef gpio;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    gpio.GPIO_Pin = I2C_SCL | I2C_SDA;
    gpio.GPIO_Mode = GPIO_Mode_Out_OD;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(I2C_PORT, &gpio);

    SCL_H();
    SDA_H();
}

void SoftI2C_Start(void)
{
    SDA_H();
    SCL_H();
    I2C_Delay();
    SDA_L();
    I2C_Delay();
    SCL_L();
    I2C_Delay();
}

void SoftI2C_Stop(void)
{
    SDA_L();
    I2C_Delay();
    SCL_H();
    I2C_Delay();
    SDA_H();
    I2C_Delay();
}

uint8_t SoftI2C_WriteByte(uint8_t data)
{
    uint8_t i;
    uint8_t ack;

    for (i = 0; i < 8; i++)
    {
        if (data & 0x80)
        {
            SDA_H();
        }
        else
        {
            SDA_L();
        }
        data <<= 1;

        I2C_Delay();
        SCL_H();
        I2C_Delay();
        SCL_L();
        I2C_Delay();
    }

    SDA_H();
    I2C_Delay();
    SCL_H();
    I2C_Delay();
    ack = SDA_READ();
    SCL_L();
    I2C_Delay();

    return ack;
}

uint8_t SoftI2C_ReadByte(uint8_t ack)
{
    uint8_t i;
    uint8_t data = 0;

    SDA_H();

    for (i = 0; i < 8; i++)
    {
        data <<= 1;
        SCL_H();
        I2C_Delay();
        if (SDA_READ())
        {
            data |= 0x01;
        }
        SCL_L();
        I2C_Delay();
    }

    if (ack)
    {
        SDA_L();
    }
    else
    {
        SDA_H();
    }

    I2C_Delay();
    SCL_H();
    I2C_Delay();
    SCL_L();
    I2C_Delay();
    SDA_H();

    return data;
}


