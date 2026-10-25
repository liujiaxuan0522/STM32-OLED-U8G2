# 任务三：CubeMX + HAL + U8g2 的 OLED 显示

> 本目录对应任务三：使用 STM32CubeMX 生成 HAL 工程，再移植 U8g2。
>
> 任务一（不用 CubeMX，SPL + 软件 I2C）在 `../Task1_NoCubeMX/`。
>
> **更详细的 CubeMX/Keil/U8g2 操作步骤见 [详细操作.md](详细操作.md)。**
>
> 适用：STM32F103C8T6（Blue Pill）/ STM32F103 系列，0.96 寸 SSD1306 I2C OLED，Keil MDK + STM32CubeMX HAL 库。
>
> **本次没有 AHT20，温湿度功能跳过。** 代码只保留 OLED、汉字、滚动、U8g2 demo 和头像动画。
>
> 姓名、学号先用占位符 `YOUR_NAME`、`YOUR_ID`，在 `Core/Inc/oled_app.h` 中改成自己的即可。

## 1. 接线

| OLED | STM32F103 |
| --- | --- |
| VCC | 3.3V |
| GND | GND |
| SCL | PB6 / I2C1_SCL |
| SDA | PB7 / I2C1_SDA |

- OLED 常见地址：`0x3C`（8 位写地址 `0x78`）。
- 模块一般自带 4.7 kΩ 上拉；裸屏需要 SCL/SDA 各接 4.7 kΩ 到 3.3V。

## 2. CubeMX 配置

1. 新建工程，选择 `STM32F103C8Tx`。
2. **RCC**：`HSE` 选 `Crystal/Ceramic Resonator`。
3. **Clock Configuration**：
   - HSE 8 MHz；
   - PLL ×9；
   - SYSCLK 72 MHz；
   - APB1 36 MHz。
4. **I2C1**：
   - Mode：`I2C`；
   - Speed Mode：`Fast Mode`；
   - Clock Speed：`400000`；
   - 引脚 `PB6=I2C1_SCL`、`PB7=I2C1_SDA`。
5. **SYS**：`Debug` 选 `Serial Wire`。
6. **Project Manager**：
   - Toolchain：`MDK-ARM`；
   - 勾选 `Generate peripheral initialization as a pair of .c/.h files`；
   - 生成代码。

## 3. 添加 U8g2

1. 下载 U8g2：<https://github.com/olikraus/u8g2>。
2. 把 `csrc` 目录复制到工程，例如 `Middlewares/u8g2/csrc`。
3. 在 Keil 中把 `csrc` 里的 `.c` 文件加入工程（至少需要 `u8g2_*.c`、`u8x8_*.c`、`u8g2_fonts.c`）。
4. `Options for Target → C/C++ → Include Paths` 加入：
   - `Middlewares/u8g2/csrc`
   - `Core/Inc`
5. C/C++ 勾选 `C99 Mode`。
6. 把本工程的以下文件加入 Keil 工程：

```text
Core/Src/u8g2_hal.c
Core/Src/oled_app.c
Core/Src/main_app.c
```

## 4. 代码文件说明

| 文件 | 作用 |
| --- | --- |
| `Core/Inc/u8g2_hal.h` | U8g2 初始化接口 |
| `Core/Src/u8g2_hal.c` | U8g2 的 I2C 字节回调、GPIO/延时回调 |
| `Core/Inc/oled_app.h` | 姓名/学号宏、显示函数声明 |
| `Core/Src/oled_app.c` | 学号姓名、系统信息、滚动、U8g2 demo、头像动画 |
| `Core/Inc/avatar.h` | 32×32 XBM 笑脸图案，可替换成自己的头像 |
| `Core/Src/main_app.c` | `App_Init()` 和 `App_Loop()` 示例 |

## 5. main.c 中需要加的内容

在 `main.c` 的 `/* USER CODE BEGIN Includes */` 中加入：

```c
#include "app.h"
```

在 `MX_I2C1_Init()` 之后加入：

```c
App_Init();
```

在 `while (1)` 中加入：

```c
App_Loop();
```

## 6. 姓名和学号

打开 `Core/Inc/oled_app.h`：

```c
#define STUDENT_NAME "YOUR_NAME"
#define STUDENT_ID   "YOUR_ID"
```

改成自己的姓名和学号。中文姓名会使用 U8g2 的 `u8g2_font_wqy12_t_chinese1` 字库；如果姓名中有该字库不包含的生僻字，需要用 `bdfconv` 生成自定义字库，或改用拼音/昵称。

## 7. 中文字库与 UTF-8

`u8g2_DrawUTF8()` 按 UTF-8 解析字符串，所以：

- Keil 源文件保存为 **UTF-8**；
- 不要保存成 GB2312/GBK；
- 如果中文显示为乱码，先检查源文件编码；
- 如果编译提示 `u8g2_font_wqy12_t_chinese1` 未定义，把 `u8g2_fonts.c` 加入工程，并确认 U8g2 版本支持该字库。

`u8g2_font_wqy12_t_chinese1` 覆盖常用汉字，适合：

```text
Hello，欢迎来到重庆交通大学物联网205实训室！
```

## 8. 显示流程

`App_Loop()` 会循环执行：

1. `OLED_ShowID()`：显示姓名、学号；
2. `OLED_ShowInfo()`：显示 MCU、OLED、U8g2 信息；
3. `OLED_ScrollTextHorizontal()`：长字符串左右滚动；
4. `OLED_ScrollTextVertical()`：多行文字上下滚动；
5. `OLED_U8g2Demo()`：边框、圆、线、矩形、文字；
6. `OLED_AvatarAnimation()`：32×32 头像图案左右移动。

## 9. 硬件滚动与软件滚动

任务一要求“最好使用硬件刷屏模式”。SSD1306 本身支持硬件水平/垂直滚动，但 U8g2 使用完整显存缓冲区，硬件滚动和 U8g2 的整屏刷新会互相冲突。

本代码采用 **U8g2 软件滚动**，优点是：

- 中文字符串宽度容易计算；
- 可以左右、上下滚动；
- 与 U8g2 的 `DrawUTF8`、`DrawXBMP` 兼容。

如果必须演示 SSD1306 硬件滚动，可在不调用 `u8g2_SendBuffer()` 的情况下，用裸命令开启滚动：

```c
/* 水平向右滚动，页 0~7，间隔 0x00，末页 0x07 */
uint8_t cmd[] = {0x2E, 0x26, 0x00, 0x00, 0x07, 0x00, 0xFF, 0x2F};
```

具体命令要按 SSD1306 数据手册写入；使用 U8g2 时建议以软件滚动为主，报告中说明两者区别即可。

## 10. Keil 虚拟逻辑分析仪能抓到 I2C 波形吗？

**结论：通常抓不到“真实的 I2C 协议波形”，最多只能看到 MCU 引脚的高低电平变化，而且硬件 I2C 的 SCL/SDA 由片上外设驱动，Keil 的 Logic Analyzer 不一定能完整建模。**

原因：

1. Keil 的 Logic Analyzer 观察的是 **MCU 内部信号/变量/GPIO 引脚状态**，不是外部总线；
2. 使用硬件 I2C 时，SCL/SDA 是复用功能输出，仿真器未必把 I2C 外设的时序映射到这两个引脚；
3. 即使能看到高低变化，也不一定带 I2C 的 START、地址、ACK、数据位、STOP 的完整协议信息；
4. 真实的上拉、从机 ACK、总线电容、上升沿等，虚拟逻辑分析仪无法真实反映。

如果改用 **软件 I2C（GPIO 模拟）**，把 SCL/SDA 当普通 GPIO 翻转，Keil Logic Analyzer 可以看到比较像样的波形：

```text
SCL:  ─┐ ┌─┐ ┌─┐ ┌─ ... ─┐ ┌─
SDA:  ──┘   └─┘   └─ ... ──┘
        START  地址位   ACK   DATA   STOP
```

能观察到：

- START：SCL 高时 SDA 由高变低；
- 7 位地址 + R/W 位；
- 第 9 个时钟的 ACK；
- 8 位数据；
- STOP：SCL 高时 SDA 由低变高。

如果无法抓到真实 I2C，推荐替代方法：

1. **外接逻辑分析仪**：Saleae Logic 16、Logic 8、廉价 24 MHz 8 通道分析仪；
2. **示波器**：双通道看 SCL/SDA，手动对照时序；
3. **sigrok / PulseView**：配合兼容逻辑分析仪，自带 I2C 解码器；
4. **软件 I2C + GPIO**：把 SCL/SDA 换成普通 GPIO 位翻转，便于 Keil 观察；
5. **串口打印**：把地址、寄存器、数据打印到 USART1，至少能验证通信内容；
6. **Proteus**：用 I2C Debugger/虚拟终端观察总线；
7. **STM32 硬件 I2C + 额外 GPIO 标记**：在每次传输前后翻转一个空闲 GPIO，用逻辑分析仪看“事务边界”。

## 11. 常见问题

### 11.1 OLED 不亮

- 地址不是 0x3C？用 I2C 扫描确认；
- SCL/SDA 接反；
- 模块没有上拉；
- 供电 3.3V，不要接 5V；
- `U8g2_Init()` 没有调用，或 `u8g2_SetPowerSave(0)` 没调用。

### 11.2 中文乱码

- 源文件不是 UTF-8；
- 字库不包含该汉字；
- `u8g2_DrawUTF8` 与 `u8g2_DrawStr` 用混了。

### 11.3 编译报错 `u8g2_font_wqy12_t_chinese1` 未定义

- 没有把 `u8g2_fonts.c` 加入工程；
- U8g2 版本太旧，换最新版。

### 11.4 Flash 不够

STM32F103C8T6 只有 64 KB Flash。U8g2 加中文字库可能超：

- 换 STM32F103CBT6（128 KB）；
- 用 `bdfconv` 只生成作业中用到的汉字子集；
- 只保留 `u8g2_font_6x10_tf` 和 `u8g2_font_wqy12_t_chinese1`，删掉不用的字体。

## 12. 参考

- U8g2 官方仓库：<https://github.com/olikraus/u8g2>
- 作业参考文章：
  - <https://blog.csdn.net/qq_45237293/article/details/111712565>
  - <https://blog.csdn.net/qq_43279579/article/details/111414037>
  - <https://blog.csdn.net/black_sneak/article/details/126312657>
- STM32F103 + U8g2 硬件 I2C 参考：<https://www.cnblogs.com/njit-sam/p/17659094.html>
