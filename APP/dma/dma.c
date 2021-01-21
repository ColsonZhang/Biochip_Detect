#include "dma.h"

/*******************************************************************************
* 函 数 名         : DMAx_Send_Init
* 函数功能		   : DMA_Send初始化函数
* 输    入         : 
					 DMAy_Channelx:DMA通道选择,@ref DMA_channel DMA_Channel_0~DMA_Channel_7
					 par:外设地址
					 mar:存储器地址
					 ndtr:数据传输量
* 输    出         : 无
*******************************************************************************/ 
void DMAx_Send_Init(DMA_Channel_TypeDef* DMAy_Channelx,u32 par,u32 mar,u16 ndtr)
{
	DMA_InitTypeDef  DMA_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//DMA1时钟使能 
	
	DMA_DeInit(DMAy_Channelx);
	
	/* 配置 DMA */
	DMA_InitStructure.DMA_PeripheralBaseAddr = par;//DMA外设地址
	DMA_InitStructure.DMA_MemoryBaseAddr = mar;//DMA 存储器0地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;//存储器到外设
	DMA_InitStructure.DMA_BufferSize = ndtr;//数据传输量 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设非增量模式
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//存储器增量模式
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据长度:8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//存储器数据长度:8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// 使用普通模式 
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;//高优先级
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
	DMA_Init(DMAy_Channelx, &DMA_InitStructure);//初始化DMA 
	
	DMA_Cmd(DMAy_Channelx, DISABLE);
	// DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,ENABLE);  //开启DMA中断
}

/*******************************************************************************
* 函 数 名         : DMAx_Rx_Init
* 函数功能		   : DMA_Rx初始化函数
* 输    入         : 
					 DMAy_Channelx:DMA通道选择,@ref DMA_channel DMA_Channel_0~DMA_Channel_7
					 par:外设地址
					 mar:存储器地址
					 ndtr:数据传输量
* 输    出         : 无
*******************************************************************************/ 
void DMAx_Rx_Init(DMA_Channel_TypeDef* DMAy_Channelx,u32 par,u32 mar,u16 ndtr)
{
	DMA_InitTypeDef  DMA_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//DMA1时钟使能 
	
	DMA_DeInit(DMAy_Channelx);
	
	/* 配置 DMA */
	DMA_InitStructure.DMA_PeripheralBaseAddr = par;//DMA外设地址
	DMA_InitStructure.DMA_MemoryBaseAddr = mar;//DMA 存储器0地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//外设到存储器模式
	DMA_InitStructure.DMA_BufferSize = ndtr;//数据传输量 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设非增量模式
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//存储器增量模式
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据长度:8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//存储器数据长度:8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//循环模式 
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;//最高优先级
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
	DMA_Init(DMAy_Channelx, &DMA_InitStructure);//初始化DMA 
	
}

/*******************************************************************************
* 函 数 名         : DMAx_Enable
* 函数功能		   : 开启一次DMA传输
* 输    入         : DMAy_Channelx:DMA通道选择,@ref DMA_channel DMA_Channel_0~DMA_Channel_7
					 ndtr:数据传输量
* 输    出         : 无
*******************************************************************************/ 
void DMAx_Enable(DMA_Channel_TypeDef *DMAy_Channelx,u16 ndtr)
{
 
	DMA_Cmd(DMAy_Channelx, DISABLE);                      //关闭DMA传输 
	
	DMA_SetCurrDataCounter(DMAy_Channelx,ndtr);          //数据传输量  
 
	DMA_Cmd(DMAy_Channelx, ENABLE);                      //开启DMA传输 
}	  



void DMA_SEND_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;     
  
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;     
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;          
  NVIC_Init(&NVIC_InitStructure);
}

void DMA_RX_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
	
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;     
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;          
  NVIC_Init(&NVIC_InitStructure);
}


void DMA1_Channel4_IRQHandler(void)
{
    if(DMA_GetFlagStatus(DMA1_FLAG_TC4))
    {
        printf("sended");
        DMA_ClearFlag(DMA1_FLAG_TC4);
    }
}

void DMA1_Channel5_IRQHandler(void)
{
    if(DMA_GetFlagStatus(DMA1_FLAG_TC5))
    {
        printf("sended");
        DMA_ClearFlag(DMA1_FLAG_TC5);
    }
}
