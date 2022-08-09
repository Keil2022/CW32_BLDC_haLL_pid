#include "adc.h"

uint8_t ADC_CR1Array[4] = {0x83, 0x84, 0x87, 0x86}; // ͨ������ADC��CR1�Ĵ���ʵ��ADCͨ���Զ��л�
uint8_t ADC_Start = 0x01;

uint16_t SampleData[4];	//ĸ�ߵ�ѹ NTC ƽ������ ˲ʱ���� 

//========================================================================
// ����: void ADC_Configuration(void)
// ����: ADC��ʼ������.
// ����: None.
// ����: None.
// �汾: V1.0, 2022��7��25��15:07:28
//========================================================================
void ADC_Configuration(void)
{
	ADC_InitTypeDef	ADC_InitStruct;
	DMA_InitTypeDef DMA_InitStruct = {0};
	
	ADC_GPIO_CLK_ENABLE;
	ADC_CLK_ENABLE;
	ADC_DMA_CLK_ENABLE;
	
	ADC_GPIO_ANALOG_ENABLE;	//����ADC����IO��
	
	ADC_InitStruct.ADC_AccEn = ADC_AccDisable;			//ת������ۼ��Ƿ�ʹ��
	ADC_InitStruct.ADC_Align = ADC_AlignRight;			//ADCת��������뷽ʽ(��/�Ҷ���)
	ADC_InitStruct.ADC_ClkDiv = ADC_Clk_Div4;			//ADCʱ��ѡ��
	ADC_InitStruct.ADC_DMAEn = ADC_DmaEnable;			//ADCת�������/�񴥷�DMAʹ��
	ADC_InitStruct.ADC_InBufEn = ADC_BufDisable;		//ADC��������ʹ��
	ADC_InitStruct.ADC_OpMode = ADC_SingleChOneMode;	//ADC����ģʽ
	ADC_InitStruct.ADC_SampleTime = ADC_SampTime10Clk;	//ADC����ʱ��
	ADC_InitStruct.ADC_TsEn = ADC_TsDisable;			//�����¶ȴ�������/��ʹ��
	ADC_InitStruct.ADC_VrefSel = ADC_Vref_VDDA;			//ADC�ο���ѹ
	
	ADC_Init(&ADC_InitStruct);
	
	CW_ADC->CR1_f.CHMUX = 3;	//PA03��ת��һ��
    ADC_Enable();				//ADCʹ��
	
	/*
	ʹ��4·DMAͨ����CH1��CH2��CH3�� CH4
	CH1 ��ADC���ε�ͨ���Ĳ����������RAM��ADC_ResultBuff[6]��
	CH2 ��ADC��CR1�Ĵ���������ֵ��RAM��ADC_CR1Array������Ĵ���
	CH3 ��ADC��START�Ĵ���������ֵ��RAM��ADC_Start)����Ĵ���
	CH1��CH2��CH3��ADCӲ������
	CH4��ATIMӲ������,����ADC
	*/
	
	DMA_InitStruct.DMA_DstAddress = (uint32_t)&SampleData[0];		//Ŀ���ַ
	DMA_InitStruct.DMA_DstInc = DMA_DstAddress_Increase;				//Ŀ���ַ����
	DMA_InitStruct.DMA_Mode = DMA_MODE_BLOCK;						//DMAģʽ��BLOCK	ÿ������� 1 �����ݿ������һ�������϶
	DMA_InitStruct.DMA_SrcAddress = (uint32_t)&CW_ADC->RESULT0;		//Դ��ַ��ADC�Ľ���Ĵ���
	DMA_InitStruct.DMA_SrcInc = DMA_SrcAddress_Fix;					//Դ��ַ�̶�
	DMA_InitStruct.DMA_TransferCnt = 0x4;							//DMA�������
	DMA_InitStruct.DMA_TransferWidth = DMA_TRANSFER_WIDTH_16BIT;	//����λ��16bit
	DMA_InitStruct.HardTrigSource = DMA_HardTrig_ADC_TRANSCOMPLETE;	//ADCת�����Ӳ����
	DMA_InitStruct.TrigMode = DMA_HardTrig;							//Ӳ����ģʽ
	DMA_Init(CW_DMACHANNEL1, &DMA_InitStruct);
	DMA_Cmd(CW_DMACHANNEL1, ENABLE);
	
	DMA_InitStruct.DMA_DstAddress = (uint32_t)&CW_ADC->CR1;         // Ŀ���ַ
    DMA_InitStruct.DMA_DstInc = DMA_DstAddress_Fix;                 // Ŀ���ַ�̶�
    DMA_InitStruct.DMA_Mode = DMA_MODE_BLOCK;                       // BLOCK����ģʽ
    DMA_InitStruct.DMA_SrcAddress = (uint32_t)&ADC_CR1Array[0];     // Դ��ַ
    DMA_InitStruct.DMA_SrcInc = DMA_SrcAddress_Increase;            // Դ��ַ����
    DMA_InitStruct.DMA_TransferCnt = 0x3;                           // DMA�������
    DMA_InitStruct.DMA_TransferWidth = DMA_TRANSFER_WIDTH_8BIT;     // ����λ��8bit
    DMA_InitStruct.HardTrigSource = DMA_HardTrig_ADC_TRANSCOMPLETE; // ADCת�����Ӳ����
    DMA_InitStruct.TrigMode = DMA_HardTrig;                         //  Ӳ����ģʽ
    DMA_Init(CW_DMACHANNEL2, &DMA_InitStruct);
    DMA_Cmd(CW_DMACHANNEL2, ENABLE);

    DMA_InitStruct.DMA_DstAddress = (uint32_t)&CW_ADC->START;       // Ŀ���ַ
    DMA_InitStruct.DMA_DstInc = DMA_DstAddress_Fix;                 // Ŀ���ַ�̶�
    DMA_InitStruct.DMA_Mode = DMA_MODE_BLOCK;                       // BLOCK����ģʽ
    DMA_InitStruct.DMA_SrcAddress = (uint32_t)&ADC_Start;           // Դ��ַ
    DMA_InitStruct.DMA_SrcInc = DMA_SrcAddress_Fix;                 // Դ��ַ�̶�
    DMA_InitStruct.DMA_TransferCnt = 0x3;                           // DMA�������
    DMA_InitStruct.DMA_TransferWidth = DMA_TRANSFER_WIDTH_8BIT;     // ����λ��8bit
    DMA_InitStruct.HardTrigSource = DMA_HardTrig_ADC_TRANSCOMPLETE; // ADCת�����Ӳ����
    DMA_InitStruct.TrigMode = DMA_HardTrig;                         // Ӳ����ģʽ
    DMA_Init(CW_DMACHANNEL3, &DMA_InitStruct);
    DMA_Cmd(CW_DMACHANNEL3, ENABLE);
    
    DMA_InitStruct.DMA_DstAddress = (uint32_t)&CW_ADC->START;       // Ŀ���ַ
    DMA_InitStruct.DMA_DstInc = DMA_DstAddress_Fix;                 // Ŀ���ַ�̶�
    DMA_InitStruct.DMA_Mode = DMA_MODE_BLOCK;                       // BLOCK����ģʽ
    DMA_InitStruct.DMA_SrcAddress = (uint32_t)&ADC_Start;           // Դ��ַ
    DMA_InitStruct.DMA_SrcInc = DMA_SrcAddress_Fix;                 // Դ��ַ�̶�
    DMA_InitStruct.DMA_TransferCnt = 0x1;                           // DMA�������
    DMA_InitStruct.DMA_TransferWidth = DMA_TRANSFER_WIDTH_8BIT;     // ����λ��8bit
    DMA_InitStruct.HardTrigSource = DMA_HardTrig_ATIM_CH1A2A3A4;    // ATIMӲ������
    DMA_InitStruct.TrigMode = DMA_HardTrig;                         // Ӳ����ģʽ
    DMA_Init(CW_DMACHANNEL4, &DMA_InitStruct);
    DMA_Cmd(CW_DMACHANNEL4, ENABLE);

    DMA_ITConfig(CW_DMACHANNEL1, DMA_IT_TC, ENABLE);
	
	__disable_irq();
    NVIC_EnableIRQ(DMACH1_IRQn);
    __enable_irq();
}

void DMACH1_IRQHandlerCallBack(void)
{	
    CW_DMA->ICR_f.TC1 = 0x00;    // ���жϱ�־
    CW_ATIM->ICR  = 0x00;        // ��ATIM���жϱ�־
    CW_ADC->ICR = 0x00;          // ��ADC���жϱ�־
    CW_ADC->CR1 = 0x83;     // AdcIn = ch0
    //------------------------------------------------
    // DMA.CH1 : ADC_Result -> RAM
    CW_DMACHANNEL1->CNT = 0x10004; //��4��
    CW_DMACHANNEL1->SRCADDR = (uint32_t)(&CW_ADC->RESULT0);
    CW_DMACHANNEL1->DSTADDR = (uint32_t)(&SampleData[0]);
    CW_DMACHANNEL1->CSR_f.EN = 1;


    //------------------------------------------------
    // DMA.CH2 : RAM -> ADC_CR1
    CW_DMACHANNEL2->CNT = 0x10003; //��3��
    CW_DMACHANNEL2->SRCADDR = (uint32_t)(&ADC_CR1Array[1]);
    CW_DMACHANNEL2->DSTADDR = (uint32_t)(&CW_ADC->CR1);
    CW_DMACHANNEL2->CSR_f.EN = 1;

    //------------------------------------------------
    // DMA.CH3 : RAM -> ADC_START
    CW_DMACHANNEL3->CNT = 0x10003; //��3��
    CW_DMACHANNEL3->SRCADDR = (uint32_t)(&ADC_Start);
    CW_DMACHANNEL3->DSTADDR = (uint32_t)(&CW_ADC->START);
    CW_DMACHANNEL3->CSR_f.EN = 1;
    
    //------------------------------------------------
    // DMA.CH4 : RAM -> ADC_START
    CW_DMACHANNEL4->CNT = 0x10001; //��1��
    CW_DMACHANNEL4->SRCADDR = (uint32_t)(&ADC_Start);
    CW_DMACHANNEL4->DSTADDR = (uint32_t)(&CW_ADC->START);
    CW_DMACHANNEL4->CSR_f.EN = 1;		
}

//�ɼ���ѹ����
void SampleVI(void)
{
	float t;
	static unsigned char IErCount = 0,	VErCount = 0;	//��ѹ ��������
	
	//�����ѹ�Ƚ�
	if(SampleData[2] <= DIin)	CanshuI=0;
	else 
	{
		t = (SampleData[2] - DIin);	//�˷Ź�ʽ�����ISEN��ѹ���Բ�������100R��Ϊ����
		t = t*1.61;					//t=t/4096*3300/4.3/0.1; //0.1ŷ
		CanshuI = t;				
	}
	if(CanshuI>=ISH*1000 && Err_Code==0) 	//��������
	{
		IErCount++;
		if(IErCount >= NumErr)	Err_Code = 4;	//���4������
	}
	else IErCount=0;
	
	t = SampleData[0];				//ĸ�ߵ�ѹ
	t = t/4096*3.3/RV1*(RV1+RV2); 	//�����ѹ�����ߵ�ѹ֮�ȣ�ΪRV1:(RV1+RV2)
	CanshuV=t*10;					//�ɼ�ĸ�ߵ�ѹ�Ŵ�10��

	if(CanshuV>=VSH*10 && Err_Code==0)  //��ѹ�ж�
	{
		VErCount++;
		if(VErCount >= NumErr)	Err_Code = 6;	//���6����ѹ
	}
	else VErCount=0;	
}


