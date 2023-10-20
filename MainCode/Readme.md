## HAL库基本代码架构(带oled外设)

**代码结构**：Startup **启动文件**

​				   /User**主程序与中断**

​				   /Users/Header.h**全局头文件**

​				   Drivers/xxHal_Driver **底层驱动文件**

​				   Drivers/BSP **系统级外设初始化文件**

​				   Middlewares **中间层板级驱动文件**

​				   Control **核心控制逻辑** 其他复杂外设独立文件

---

**实现效果：**四线模拟SPI刷新oled屏幕

**使用引脚：**PB3-DC，PB4-RES，PC14-SCLK，PB5-SDIN

**更新时间：**2023/7/15

**注意事项：**采用stlink烧录程序，系统默认复位使PB3 & PB4无法控制，**最后初始化重复用禁用jtag功能**，**并在后续不能再进行时钟总线初始化**，否则将重新分配复用功能(oled不能工作)

**oled模块**：**/Drivers/BSP/gpio.c&h** 所需系统gpio初始化

​					**/Middlewares/oled/oledfont.h** oled字符库

​					**/Middlewares/oled/bsp_oled.c&h** 模拟spi与oled初始化

​					**/Control/show.c&.h** 控制逻辑与显示函数

---

**实现效果：**单通道ADC采集特定IO口(PC0)电压值和显示，可用于电位器测量和电池电压检测

**使用引脚：**PC0-Analog

**更新时间：**2023/7/15

**注意事项：**功能简易无复杂驱动，在系统外设初始化中定义功能实现

**ADC采集**：**/Drivers/BSP/adc.c&h** 所需系统gpio和adc初始化，多次均值函数

​					

