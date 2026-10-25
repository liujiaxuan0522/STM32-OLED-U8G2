# 任务一：Keil 里具体怎么加文件

> 任务一不用 CubeMX，使用 **Keil + STM32 标准外设库 SPL + 软件 I2C**。  
> 下面先给出你电脑上已经找到的实际路径，再说明文件加到哪个 Group。

## 先修当前这个报错

你的编译报错是：

```text
stm32f10x.h(8302): error: 'stm32f10x_conf.h' file not found
```

解决：

`Options for Target`（魔术棒）→ `C/C++` → `Include Paths` → 添加：

```text
F:\deepseek-harness-workspace\STM32_OLED_U8G2\Task1_NoCubeMX\User
```

如果工程文件在 `Task1_NoCubeMX\MDK-ARM\` 下，也可以写：

```text
..\User
```

加完以后 `Rebuild`。

`stm32f10x_conf.h` 已经在这个目录里：

```text
F:\deepseek-harness-workspace\STM32_OLED_U8G2\Task1_NoCubeMX\User\stm32f10x_conf.h
```

> 只把 `stm32f10x_conf.h` 加到 Group 里没有用，必须把 `User` 目录加到 **Include Paths**。

## 0. 你电脑上的实际路径

我在你的电脑上找到了 Keil 的 STM32F1 器件包，SPL 和 CMSIS 都在这里：

```text
E:\Keil\packs\Keil\STM32F1xx_DFP\2.2.0\
```

### 0.1 SPL 驱动

```text
E:\Keil\packs\Keil\STM32F1xx_DFP\2.2.0\Device\StdPeriph_Driver\
├─ inc\
│  ├─ stm32f10x_gpio.h
│  └─ stm32f10x_rcc.h
└─ src\
   ├─ stm32f10x_gpio.c
   └─ stm32f10x_rcc.c
```

### 0.2 CMSIS 设备头文件

```text
E:\Keil\packs\Keil\STM32F1xx_DFP\2.2.0\Device\Include\
├─ stm32f10x.h
└─ system_stm32f10x.h
```

### 0.3 system 源文件和启动文件

```text
E:\Keil\packs\Keil\STM32F1xx_DFP\2.2.0\Device\Source\
├─ system_stm32f10x.c
└─ ARM\
   └─ startup_stm32f10x_md.s
```

### 0.4 CMSIS Core 头文件

你这个 Keil 环境是 CMSIS 5，`core_cm3` **只有头文件，没有 `core_cm3.c`**：

```text
E:\Keil\packs\ARM\CMSIS\5.9.0\CMSIS\Core\Include\
└─ core_cm3.h
```

所以任务一不需要加 `core_cm3.c`，只需要把上面的 `Include` 目录加进 Include Paths。

### 0.5 stm32f10x_conf.h

模板里有：

```text
E:\Keil\packs\Keil\STM32F1xx_DFP\2.2.0\Device\StdPeriph_Driver\templates\stm32f10x_conf.h
```

但本工程 `User` 目录里已经放了一个最小版本：

```text
Task1_NoCubeMX\User\stm32f10x_conf.h
```

二选一即可，建议直接用 `User` 里的，避免模板里包含太多无关外设。

## 1. 新建 Keil 工程

1. `Project → New uVision Project`；
2. 工程放：

```text
STM32_OLED_U8G2\Task1_NoCubeMX\MDK-ARM\
```

3. 器件选：

```text
STMicroelectronics → STM32F103 → STM32F103C8
```

4. `Manage Run-Time Environment` 弹窗点 `Cancel`，不用 RTE。

## 2. 建 4 个 Group

`Project → Manage Project Items`，建：

```text
Startup
CMSIS
StdPeriph_Driver
User
```

## 3. 往 Group 里加文件

### 3.1 Startup

右键 `Startup` → `Add Existing Files to Group 'Startup'`，文件类型选 `All files (*.*)`，加：

```text
E:\Keil\packs\Keil\STM32F1xx_DFP\2.2.0\Device\Source\ARM\startup_stm32f10x_md.s
```

> C8T6 是中等容量，必须用 `md`。

### 3.2 CMSIS

右键 `CMSIS` → `Add Existing Files`，加：

```text
E:\Keil\packs\Keil\STM32F1xx_DFP\2.2.0\Device\Source\system_stm32f10x.c
```

> 不要加 `core_cm3.c`，你的 CMSIS 5 里没有这个文件；只加 `core_cm3.h` 的 Include 路径。

### 3.3 StdPeriph_Driver

右键 `StdPeriph_Driver` → `Add Existing Files`，加：

```text
E:\Keil\packs\Keil\STM32F1xx_DFP\2.2.0\Device\StdPeriph_Driver\src\stm32f10x_gpio.c
E:\Keil\packs\Keil\STM32F1xx_DFP\2.2.0\Device\StdPeriph_Driver\src\stm32f10x_rcc.c
```

任务一只用 GPIO 和 RCC，不要加 `stm32f10x_i2c.c`。

### 3.4 User

右键 `User` → `Add Existing Files`，进入：

```text
STM32_OLED_U8G2\Task1_NoCubeMX\User\
```

加这 6 个 `.c`：

```text
main.c
soft_i2c.c
oled.c
font_ascii.c
font_cn.c
delay.c
```

`.h` 不要加到 Group 里。

## 4. 加 Include Paths

`Options for Target`（魔术棒）→ `C/C++` → `Include Paths`，加入：

```text
STM32_OLED_U8G2\Task1_NoCubeMX\User
E:\Keil\packs\Keil\STM32F1xx_DFP\2.2.0\Device\Include
E:\Keil\packs\Keil\STM32F1xx_DFP\2.2.0\Device\StdPeriph_Driver\inc
E:\Keil\packs\ARM\CMSIS\5.9.0\CMSIS\Core\Include
```

如果 `.uvprojx` 在 `MDK-ARM` 下，User 也可以写相对路径：

```text
..\User
```

### 为什么不需要 `core_cm3.c`

CMSIS 5 的 `core_cm3.h` 是头文件实现（inline/static），没有 `core_cm3.c`。  
加 `E:\Keil\packs\ARM\CMSIS\5.9.0\CMSIS\Core\Include` 就够了。

## 5. Define 宏

`Options for Target → C/C++ → Define`：

```text
USE_STDPERIPH_DRIVER,STM32F10X_MD
```

- `USE_STDPERIPH_DRIVER`：让 `stm32f10x.h` 包含 `stm32f10x_conf.h`；
- `STM32F10X_MD`：C8T6 是中等容量。

## 6. C99

同页勾选：

```text
C99 Mode
```

## 7. 时钟和下载

### Target

- `Xtal (MHz)`：`8.0`；
- `system_stm32f10x.c` 默认会配置到 72 MHz。

### Debug

- 选你的下载器，例如 `ST-Link Debugger`；
- `Settings → Flash Download` 勾选 `Reset and Run`；
- Flash 算法选 `STM32F10x Med-density Flash 64KB`。

## 8. 最终工程结构

```text
Target 1
├─ Startup
│  └─ startup_stm32f10x_md.s
├─ CMSIS
│  └─ system_stm32f10x.c
├─ StdPeriph_Driver
│  ├─ stm32f10x_gpio.c
│  └─ stm32f10x_rcc.c
└─ User
   ├─ main.c
   ├─ soft_i2c.c
   ├─ oled.c
   ├─ font_ascii.c
   ├─ font_cn.c
   └─ delay.c
```

Include Paths：

```text
..\User
E:\Keil\packs\Keil\STM32F1xx_DFP\2.2.0\Device\Include
E:\Keil\packs\Keil\STM32F1xx_DFP\2.2.0\Device\StdPeriph_Driver\inc
E:\Keil\packs\ARM\CMSIS\5.9.0\CMSIS\Core\Include
```

## 9. 常见错误

### `stm32f10x.h: No such file`

少了：

```text
E:\Keil\packs\Keil\STM32F1xx_DFP\2.2.0\Device\Include
```

### `core_cm3.h: No such file`

少了：

```text
E:\Keil\packs\ARM\CMSIS\5.9.0\CMSIS\Core\Include
```

### `stm32f10x_conf.h: No such file`

少了：

```text
..\User
```

或者删掉了 `User\stm32f10x_conf.h`。

### `GPIO_InitTypeDef` 未定义

没有加 `stm32f10x_gpio.c`，或没有定义 `USE_STDPERIPH_DRIVER`。

### `SystemInit` 未定义

没有加 `system_stm32f10x.c`。

### `Undefined symbol main`

`User\main.c` 没有加到工程，或者模板里原来的 `main.c` 还在。

### 原来的模板也有 `main.c`

右键原 `main.c` → `Options for File` → 取消 `Include in Target Build`，或直接移除。一个工程只能有一个 `main()`。

## 10. 不要加任务三的东西

任务一不要加：

```text
u8g2_hal.c
oled_app.c
main_app.c
任何 U8g2 的 csrc 文件
任何 HAL 文件
任何 stm32f1xx_hal_*.c
```

这些属于：

```text
STM32_OLED_U8G2\Task3_CubeMX_U8g2\
```
