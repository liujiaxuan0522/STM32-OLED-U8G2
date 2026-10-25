# STM32F103 OLED 实验：任务一 + 任务三

STM32F103C8T6 + 0.96 寸 SSD1306 I2C OLED 的两套实现：

- **任务一**：不用 CubeMX，Keil + SPL + 软件 I2C + 裸 SSD1306 + 自建字库；
- **任务三**：CubeMX + HAL + 硬件 I2C + U8g2，包含 U8g2 demo、中文滚动和 64×64 头像动画。

> 本次实验室没有 AHT20，温湿度功能未实现；任务二（逻辑分析仪测量）本次未做。

## 硬件

- STM32F103C8T6
- 0.96 寸 SSD1306 OLED，128×64，I2C
- ST-Link / DAP 下载器
- 3.3V 供电

接线：

| OLED | STM32F103 |
| --- | --- |
| VCC | 3.3V |
| GND | GND |
| SCL | PB6 |
| SDA | PB7 |

OLED 地址一般是 `0x3C`（8 位写地址 `0x78`）。

## 目录

```text
.
├─ Task1_NoCubeMX/
│  ├─ OLED.uvprojx          Keil 工程
│  ├─ User/                 应用代码、软件 I2C、OLED 驱动、字库
│  └─ Libraries/            CMSIS + SPL GPIO/RCC
├─ Task3_CubeMX_U8g2/
│  ├─ OLED_U8g2.ioc         CubeMX 配置
│  ├─ Core/                 HAL 工程 + 应用代码
│  ├─ Drivers/              STM32 HAL/CMSIS
│  ├─ Middlewares/u8g2/     U8g2 csrc
│  └─ MDK-ARM/              Keil 工程
└─ docs/
   ├─ CSDN_STM32_OLED_U8G2_任务一任务三博客.md
   └─ images/
```

## 任务一：不用 CubeMX

打开：

```text
Task1_NoCubeMX\OLED.uvprojx
```

工程已经改成相对路径，包含：

- CMSIS 启动文件、system 文件、`stm32f10x.h`；
- SPL 的 `stm32f10x_gpio.c`、`stm32f10x_rcc.c`；
- `User` 下的软件 I2C、SSD1306 驱动、ASCII/汉字字库和主程序。

编译宏：

```text
USE_STDPERIPH_DRIVER,STM32F10X_MD
```

功能：

- 显示姓名、学号；
- 16×16 汉字点阵；
- 左右滚动；
- 软件 I2C，方便用 Keil 逻辑分析仪观察 SCL/SDA。

## 任务三：CubeMX + HAL + U8g2

打开：

```text
Task3_CubeMX_U8g2\MDK-ARM\OLED_U8g2.uvprojx
```

CubeMX 配置：

- HSE 8 MHz；
- PLL Source HSE，PLL input /1，PLLMul ×9；
- SYSCLK 72 MHz；
- APB1 36 MHz；
- I2C1：PB6/PB7，Fast Mode，400 kHz；
- ADC /6。

U8g2 已经放在：

```text
Task3_CubeMX_U8g2\Middlewares\u8g2\csrc
```

Keil 的 Include Path 已包含：

```text
..\Core\Inc
..\Middlewares\u8g2\csrc
```

功能：

- U8g2 demo；
- 姓名、学号；
- 中文长字符串左右滚动；
- 多行文字上下滚动；
- 64×64 XBM 头像动画。

## 中文字库说明

U8g2 自带的 `wqy12_t_chinese1` / `chinese3` 是子集，实测缺“姓、刘、佳、烜、息、验”等字。

任务三最终使用自定义字库：

```text
Core/Src/myfont16.c
```

在 `oled_app.c` 中：

```c
extern const uint8_t myfont16[];
u8g2_SetFont(&u8g2, myfont16);
```

自定义字库只包含作业中用到的字符，约 2 KB，适合 C8T6 的 64 KB Flash。

## 头像

`Task3_CubeMX_U8g2/Core/Inc/avatar.h` 是 64×64 XBM，512 字节。

显示：

```c
u8g2_DrawXBMP(&u8g2, x, 0, 64, 64, avatar_bits);
```

如果要换头像，可以用 `Task3_CubeMX_U8g2/tools/make_avatar.py` 或 image2cpp 生成 64×64 XBM，替换 `avatar_bits`。

## 编译注意

1. 使用 **ARM Compiler 6**（AC5 可能没有 License，报 `C9555E`）。
2. 如果 `u8g2.h` 找不到，检查 Include Path 是否是：

```text
..\Middlewares\u8g2\csrc
```

3. 如果 `myfont16` 未定义，确认 `Core/Src/myfont16.c` 已加入 Keil 的 `Application/User/Core` Group。
4. 如果 OLED 不亮，检查 0x3C/0x3D、PB6/PB7、3.3V 和上拉。

## 文档

- [任务一详细说明](Task1_NoCubeMX/README.md)
- [任务三详细操作](Task3_CubeMX_U8g2/详细操作.md)
- `docs/images/` 中放了头像效果图。

## 第三方库

- STM32F1xx HAL / CMSIS：ST 提供；
- U8g2：https://github.com/olikraus/u8g2
- 字库由 SimHei 生成，仅用于课程作业。
