# 任务一：不使用 CubeMX 的 OLED 显示

> 任务一要求不用 CubeMX，因此本目录使用 **Keil + STM32 标准外设库（SPL）+ 软件 I2C**，不包含 HAL，也不包含 U8g2。
>
> 如果不知道文件具体加到哪个 Group、Include Paths 怎么填，请看 [Keil_Add_Files.md](Keil_Add_Files.md)。
>
> 如果不知道每个 Group 里的文件从哪找，请看 [文件来源说明.md](文件来源说明.md)。
>
> 任务三的 CubeMX + HAL + U8g2 代码在 `../Task3_CubeMX_U8g2/`。

## 1. 实现内容

- 软件 I2C 驱动 SSD1306 OLED；
- 显示姓名、学号；
- 显示 ASCII 字符串；
- 16×16 汉字点阵显示；
- 长字符串左右滚动；
- 字符串上下滚动；
- 不依赖 AHT20。

## 2. 接线

| OLED | STM32F103 |
| --- | --- |
| VCC | 3.3V |
| GND | GND |
| SCL | PB6 |
| SDA | PB7 |

OLED 地址：0x3C（8 位写地址 0x78）。

## 3. Keil 工程（不用 CubeMX）

1. 新建 Keil 工程，器件选 `STM32F103C8`。
2. 加入启动文件 `startup_stm32f10x_md.s`。
3. 加入 SPL 文件：
   - `stm32f10x_gpio.c`
   - `stm32f10x_rcc.c`
   - `system_stm32f10x.c`
   - `core_cm3.c`（如果工程需要）
4. `Options for Target → C/C++ → Define` 加入：

```text
USE_STDPERIPH_DRIVER
STM32F10X_MD
```

5. Include Paths 加入：
   - SPL 的 `inc` 目录；
   - 本目录的 `User`。
6. 把 `User` 下的文件加入工程：

```text
main.c
soft_i2c.c
oled.c
font_ascii.c
font_cn.c
delay.c
```

7. 确认系统时钟为 72 MHz（`system_stm32f10x.c` 默认配置即可）。
8. 编译下载。

> 如果你用的是现成的 SPL 模板工程，直接把 `User` 里的文件加进去，并保留模板里的 `stm32f10x_conf.h` 即可。

## 4. 姓名和学号

打开 `User/main.c`：

```c
#define MY_NAME   "NAME"
#define MY_ID     "YOUR_ID"
```

改成自己的姓名/学号。

- 英文/拼音姓名可以直接显示；
- 中文姓名需要把姓名中的汉字加入 `tools/gen_font.py` 的 `CHARS`，重新运行脚本生成 `font_cn.c`，再编译。

例如：

```python
CHARS = "姓名：学号：张三Hello，欢迎来到重庆交通大学物联网205实训室！"
```

运行：

```text
python tools/gen_font.py
```

会重新生成 `User/font_ascii.c`、`User/font_ascii.h`、`User/font_cn.c`、`User/font_cn.h`。

## 5. 代码结构

| 文件 | 作用 |
| --- | --- |
| `User/soft_i2c.c` | PB6/PB7 软件 I2C |
| `User/oled.c` | SSD1306 命令、显存、字符/汉字、滚动 |
| `User/font_ascii.c` | 8×16 ASCII 字库 |
| `User/font_cn.c` | 16×16 汉字字库（按需生成） |
| `User/delay.c` | 粗略延时 |
| `User/main.c` | 显示姓名学号、滚动演示 |
| `tools/gen_font.py` | 生成 ASCII/汉字点阵字库 |

## 6. 和任务三的区别

| | 任务一 | 任务三 |
| --- | --- | --- |
| 工程 | Keil + SPL，不用 CubeMX | CubeMX + HAL |
| 显示 | 裸 SSD1306 + 自建显存/字库 | U8g2 |
| I2C | 软件 I2C（GPIO 模拟） | 硬件 I2C1 |
| 重点 | 点阵、页地址、汉字取模 | GUI 库移植、图形界面 |
| AHT20 | 无 | 无（本次也没有） |

## 7. 说明

- 软件 I2C 的 SCL/SDA 是普通 GPIO，方便用 Keil 逻辑分析仪观察引脚变化；
- 但 Keil 仍不是真实总线分析仪，要抓完整 I2C 协议，建议外接逻辑分析仪或示波器；
- 汉字字库只包含 `tools/gen_font.py` 中 `CHARS` 里的字，缺字会显示为空白；
- 如果 OLED 不亮，先确认地址 0x3C、PB6/PB7 接线和上拉。
