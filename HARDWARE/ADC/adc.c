#include "adc.h"

uint8_t ADC_CR1Array[4] = {0x83, 0x84, 0x87, 0x86}; // 通过更改ADC的CR1寄存器实现ADC通道自动切换
uint8_t ADC_Start = 0x01;

uint16_t SampleData[4];	//母线电压 NTC 平均电流 瞬时电流 

//========================================================================
// 函数: void ADC_Configuration(void)
// 描述: ADC初始化配置.
// 参数: None.
// 返回: None.
// 版本: V1.0, 2022年7月25日15:07:28
//========================================================================
void ADC_Configuration(void)
{
	ADC_InitTypeDef	ADC_InitStruct;
	DMA_InitTypeDef DMA_InitStruct = {0};
	
	ADC_GPIO_CLK_ENABLE;
	ADC_CLK_ENABLE;
	ADC_DMA_CLK_ENABLE;
	
	ADC_GPIO_ANALOG_ENABLE;	//配置ADC测试IO口
	
	ADC_InitStruct.ADC_AccEn = ADC_AccDisable;			//转换结果累加是否使能
	ADC_InitStruct.ADC_Align = ADC_AlignRight;			//ADC转换结果对齐方式(左/右对齐)
	ADC_InitStruct.ADC_ClkDiv = ADC_Clk_Div4;			//ADC时钟选择
	ADC_InitStruct.ADC_DMAEn = ADC_DmaEnable;			//ADC转换完成是/否触发DMA使能
	ADC_InitStruct.ADC_InBufEn = ADC_BufDisable;		//ADC输入增益使能
	ADC_InitStruct.ADC_OpMode = ADC_SingleChOneMode;	//ADC操作模式
	ADC_InitStruct.ADC_SampleTime = ADC_SampTime10Clk;	//ADC采样时间
	ADC_InitStruct.ADC_TsEn = ADC_TsDisable;			//内置温度传感器是/否使能
	ADC_InitStruct.ADC_VrefSel = ADC_Vref_VDDA;			//ADC参考电压
	
	ADC_Init(&ADC_InitStruct);
	
	CW_ADC->CR1_f.CHMUX = 3;	//PA03先转换一次
    ADC_Enable();				//ADC使能
	
	/*
	使用4路DMA通道：CH1、CH2、CH3、 CH4
	CH1 将ADC单次单通道的采样结果传入RAM（ADC_ResultBuff[6]）
	CH2 将ADC的CR1寄存器的配置值从RAM（ADC_CR1Array）传入寄存器
	CH3 将ADC的START寄存器的配置值从RAM（ADC_Start)传入寄存器
	CH1、CH2、CH3由ADC硬件触发
	CH4由ATIM硬件触发,启动ADC
	*/
	
	DMA_InitStruct.DMA_DstAddress = (uint32_t)&SampleData[0];		//目标地址
	DMA_InitStruct.DMA_DstInc = DMA_DstAddress_Increase;				//目标地址递增
	DMA_InitStruct.DMA_Mode = DMA_MODE_BLOCK;						//DMA模式：BLOCK	每传输完成 1 个数据块后会插入一个传输间隙
	DMA_InitStruct.DMA_SrcAddress = (uint32_t)&CW_ADC->RESULT0;		//源地址：ADC的结果寄存器
	DMA_InitStruct.DMA_SrcInc = DMA_SrcAddress_Fix;					//源地址固定
	DMA_InitStruct.DMA_TransferCnt = 0x4;							//DMA传输次数
	DMA_InitStruct.DMA_TransferWidth = DMA_TRANSFER_WIDTH_16BIT;	//数据位宽16bit
	DMA_InitStruct.HardTrigSource = DMA_HardTrig_ADC_TRANSCOMPLETE;	//ADC转换完成硬触发
	DMA_InitStruct.TrigMode = DMA_HardTrig;							//硬触发模式
	DMA_Init(CW_DMACHANNEL1, &DMA_InitStruct);
	DMA_Cmd(CW_DMACHANNEL1, ENABLE);
	
	DMA_InitStruct.DMA_DstAddress = (uint32_t)&CW_ADC->CR1;         // 目标地址
    DMA_InitStruct.DMA_DstInc = DMA_DstAddress_Fix;                 // 目标地址固定
    DMA_InitStruct.DMA_Mode = DMA_MODE_BLOCK;                       // BLOCK传输模式
    DMA_InitStruct.DMA_SrcAddress = (uint32_t)&ADC_CR1Array[0];     // 源地址
    DMA_InitStruct.DMA_SrcInc = DMA_SrcAddress_Increase;            // 源地址递增
    DMA_InitStruct.DMA_TransferCnt = 0x3;                           // DMA传输次数
    DMA_InitStruct.DMA_TransferWidth = DMA_TRANSFER_WIDTH_8BIT;     // 数据位宽8bit
    DMA_InitStruct.HardTrigSource = DMA_HardTrig_ADC_TRANSCOMPLETE; // ADC转换完成硬触发
    DMA_InitStruct.TrigMode = DMA_HardTrig;                         //  硬触发模式
    DMA_Init(CW_DMACHANNEL2, &DMA_InitStruct);
    DMA_Cmd(CW_DMACHANNEL2, ENABLE);

    DMA_InitStruct.DMA_DstAddress = (uint32_t)&CW_ADC->START;       // 目标地址
    DMA_InitStruct.DMA_DstInc = DMA_DstAddress_Fix;                 // 目标地址固定
    DMA_InitStruct.DMA_Mode = DMA_MODE_BLOCK;                       // BLOCK传输模式
    DMA_InitStruct.DMA_SrcAddress = (uint32_t)&ADC_Start;           // 源地址
    DMA_InitStruct.DMA_SrcInc = DMA_SrcAddress_Fix;                 // 源地址固定
    DMA_InitStruct.DMA_TransferCnt = 0x3;                           // DMA传输次数
    DMA_InitStruct.DMA_TransferWidth = DMA_TRANSFER_WIDTH_8BIT;     // 数据位宽8bit
    DMA_InitStruct.HardTrigSource = DMA_HardTrig_ADC_TRANSCOMPLETE; // ADC转换完成硬触发
    DMA_InitStruct.TrigMode = DMA_HardTrig;                         // 硬触发模式
    DMA_Init(CW_DMACHANNEL3, &DMA_InitStruct);
    DMA_Cmd(CW_DMACHANNEL3, ENABLE);
    
    DMA_InitStruct.DMA_DstAddress = (uint32_t)&CW_ADC->START;       // 目标地址
    DMA_InitStruct.DMA_DstInc = DMA_DstAddress_Fix;                 // 目标地址固定
    DMA_InitStruct.DMA_Mode = DMA_MODE_BLOCK;                       // BLOCK传输模式
    DMA_InitStruct.DMA_SrcAddress = (uint32_t)&ADC_Start;           // 源地址
    DMA_InitStruct.DMA_SrcInc = DMA_SrcAddress_Fix;                 // 源地址固定
    DMA_InitStruct.DMA_TransferCnt = 0x1;                           // DMA传输次数
    DMA_InitStruct.DMA_TransferWidth = DMA_TRANSFER_WIDTH_8BIT;     // 数据位宽8bit
    DMA_InitStruct.HardTrigSource = DMA_HardTrig_ATIM_CH1A2A3A4;    // ATIM硬件触发
    DMA_InitStruct.TrigMode = DMA_HardTrig;                         // 硬触发模式
    DMA_Init(CW_DMACHANNEL4, &DMA_InitStruct);
    DMA_Cmd(CW_DMACHANNEL4, ENABLE);

    DMA_ITConfig(CW_DMACHANNEL1, DMA_IT_TC, ENABLE);
	
	__disable_irq();
    NVIC_EnableIRQ(DMACH1_IRQn);
    __enable_irq();
}

void DMACH1_IRQHandlerCallBack(void)
{	
    CW_DMA->ICR_f.TC1 = 0x00;    // 清中断标志
    CW_ATIM->ICR  = 0x00;        // 清ATIM的中断标志
    CW_ADC->ICR = 0x00;          // 请ADC的中断标志
    CW_ADC->CR1 = 0x83;     // AdcIn = ch0
    //------------------------------------------------
    // DMA.CH1 : ADC_Result -> RAM
    CW_DMACHANNEL1->CNT = 0x10004; //传4个
    CW_DMACHANNEL1->SRCADDR = (uint32_t)(&CW_ADC->RESULT0);
    CW_DMACHANNEL1->DSTADDR = (uint32_t)(&SampleData[0]);
    CW_DMACHANNEL1->CSR_f.EN = 1;


    //------------------------------------------------
    // DMA.CH2 : RAM -> ADC_CR1
    CW_DMACHANNEL2->CNT = 0x10003; //传3个
    CW_DMACHANNEL2->SRCADDR = (uint32_t)(&ADC_CR1Array[1]);
    CW_DMACHANNEL2->DSTADDR = (uint32_t)(&CW_ADC->CR1);
    CW_DMACHANNEL2->CSR_f.EN = 1;

    //------------------------------------------------
    // DMA.CH3 : RAM -> ADC_START
    CW_DMACHANNEL3->CNT = 0x10003; //传3个
    CW_DMACHANNEL3->SRCADDR = (uint32_t)(&ADC_Start);
    CW_DMACHANNEL3->DSTADDR = (uint32_t)(&CW_ADC->START);
    CW_DMACHANNEL3->CSR_f.EN = 1;
    
    //------------------------------------------------
    // DMA.CH4 : RAM -> ADC_START
    CW_DMACHANNEL4->CNT = 0x10001; //传1个
    CW_DMACHANNEL4->SRCADDR = (uint32_t)(&ADC_Start);
    CW_DMACHANNEL4->DSTADDR = (uint32_t)(&CW_ADC->START);
    CW_DMACHANNEL4->CSR_f.EN = 1;		
}

//采集电压电流
void SampleVI(void)
{
	float t;
	static unsigned char IErCount = 0,	VErCount = 0;	//过压 过流次数
	
	//电机电压比较
	if(SampleData[2] <= DIin)	CanshuI=0;
	else 
	{
		t = (SampleData[2] - DIin);	//运放公式，算出ISEN电压除以采样电阻100R，为电流
		t = t*1.61;					//t=t/4096*3300/4.3/0.1; //0.1欧
		CanshuI = t;				
	}
	if(CanshuI>=ISH*1000 && Err_Code==0) 	//过流保护
	{
		IErCount++;
		if(IErCount >= NumErr)	Err_Code = 4;	//编号4：过流
	}
	else IErCount=0;
	
	t = SampleData[0];				//母线电压
	t = t/4096*3.3/RV1*(RV1+RV2); 	//输入电压与总线电压之比：为RV1:(RV1+RV2)
	CanshuV=t*10;					//采集母线电压放大10倍

	if(CanshuV>=VSH*10 && Err_Code==0)  //过压判断
	{
		VErCount++;
		if(VErCount >= NumErr)	Err_Code = 6;	//编号6：过压
	}
	else VErCount=0;	
}


