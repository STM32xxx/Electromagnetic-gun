# Electromagnetic-gun
2019年“TI”杯全国大学生电子设计竞赛H题，模拟电磁曲射炮，STM32程序及OpenMV程序

演示视频：https://www.bilibili.com/video/BV1v4411D7yP?share_source=copy_web

使用STM32F103C8T6主控，串口屏交互，OpenMV3检测靶标角度及距离

**系统框图：**

<div align=left><img width=550 src="image\系统框图.jpg">




**主控电路：**

<div align=left><img width=800 src="image\主控电路.jpg">


**I/O资源分配表：**

| I/O引脚    | 内部资源 | 功能             |
| ---------- | -------- | ---------------- |
| PA0        | ADC      | 储能电容电压采集 |
| PA2、PA3   | USART2   | 与串口屏通信     |
| PA6、PA7   | PWM      | 控制舵机         |
| PA9、PA10  | USART1   | 调试串口         |
| PB3        | 普通I/O  | 控制蜂鸣器       |
| PB4        | 普通I/O  | 控制继电器       |
| PB10、PB11 | USART3   | 与OpenMV通信     |
| PC13       | 普通I/O  | 控制可控硅       |

**电磁炮控制电路：**

<div align=left><img width=800 src="image\电磁炮充放电电路.jpg">

