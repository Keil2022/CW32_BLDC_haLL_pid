MCU: CW32F030C8T6

实验目的：驱动无刷电机
IO端口选择根据 功能 及 数据手册 进行修改

//=====系统时钟源========

外部高速HSE时钟输入，8MHz, PLL倍频到64MHz

//=======================



//=====LED===============

功能：IO输出

LED1: PA11
LED2: PA12

//=======================



//=====KEY===============

功能：IO输入

KEY1: PF6
KEY2: PF7

//=======================



//=====蜂鸣器============

功能：IO输出

BUZ: 


//=======================



//====三路PWM 三路IO=======

功能：PWM,IO输出

U+: PA8 
V+: PA9
W+: PA10

U-: PB13 
V-: PB14
W-: PB15

六路PWM,上管调制，下管IO，上、管高电平驱动（具体看所使用的栅极驱动IC手册）

//========================




//======霍尔 GTIM2=======

功能：

HA: PA15
HB: PB3
HC: PA2

//=======================




//========编码器=========

功能：IO输入

开启编码器模式
使用定时器GTIM1

INA: PB5
INC: PB4

//=======================




//========串口=========

TXD: PA8
RXD: PA9

//=======================



//========ADC=========

使用DMA传输

NTC:		PA4
母线电压：	PA3
电机瞬时电压：	PA6
电机平均电压：	PA7


//=======================
