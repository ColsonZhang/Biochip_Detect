#include "dma.h"

/*******************************************************************************
* �� �� ��         : DMAx_Send_Init
* ��������		   : DMA_Send��ʼ������
* ��    ��         : 
					 DMAy_Channelx:DMAͨ��ѡ��,@ref DMA_channel DMA_Channel_0~DMA_Channel_7
					 par:�����ַ
					 mar:�洢����ַ
					 ndtr:���ݴ�����
* ��    ��         : ��
*******************************************************************************/ 
void DMAx_Send_Init(DMA_Channel_TypeDef* DMAy_Channelx,u32 par,u32 mar,u16 ndtr)
{
	DMA_InitTypeDef  DMA_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//DMA1ʱ��ʹ�� 
	
	DMA_DeInit(DMAy_Channelx);
	
	/* ���� DMA */
	DMA_InitStructure.DMA_PeripheralBaseAddr = par;//DMA�����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = mar;//DMA �洢��0��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;//�洢��������
	DMA_InitStructure.DMA_BufferSize = ndtr;//���ݴ����� 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//���������ģʽ
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�洢������ģʽ
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//�������ݳ���:8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//�洢�����ݳ���:8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// ʹ����ͨģʽ 
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;//�����ȼ�
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMAy_Channelx, &DMA_InitStructure);//��ʼ��DMA 
	
	DMA_Cmd(DMAy_Channelx, DISABLE);
	// DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,ENABLE);  //����DMA�ж�
}

/*******************************************************************************
* �� �� ��         : DMAx_Rx_Init
* ��������		   : DMA_Rx��ʼ������
* ��    ��         : 
					 DMAy_Channelx:DMAͨ��ѡ��,@ref DMA_channel DMA_Channel_0~DMA_Channel_7
					 par:�����ַ
					 mar:�洢����ַ
					 ndtr:���ݴ�����
* ��    ��         : ��
*******************************************************************************/ 
void DMAx_Rx_Init(DMA_Channel_TypeDef* DMAy_Channelx,u32 par,u32 mar,u16 ndtr)
{
	DMA_InitTypeDef  DMA_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//DMA1ʱ��ʹ�� 
	
	DMA_DeInit(DMAy_Channelx);
	
	/* ���� DMA */
	DMA_InitStructure.DMA_PeripheralBaseAddr = par;//DMA�����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = mar;//DMA �洢��0��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//���赽�洢��ģʽ
	DMA_InitStructure.DMA_BufferSize = ndtr;//���ݴ����� 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//���������ģʽ
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�洢������ģʽ
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//�������ݳ���:8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//�洢�����ݳ���:8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//ѭ��ģʽ 
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;//������ȼ�
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMAy_Channelx, &DMA_InitStructure);//��ʼ��DMA 
	
}

/*******************************************************************************
* �� �� ��         : DMAx_Enable
* ��������		   : ����һ��DMA����
* ��    ��         : DMAy_Channelx:DMAͨ��ѡ��,@ref DMA_channel DMA_Channel_0~DMA_Channel_7
					 ndtr:���ݴ�����
* ��    ��         : ��
*******************************************************************************/ 
void DMAx_Enable(DMA_Channel_TypeDef *DMAy_Channelx,u16 ndtr)
{
 
	DMA_Cmd(DMAy_Channelx, DISABLE);                      //�ر�DMA���� 
	
	DMA_SetCurrDataCounter(DMAy_Channelx,ndtr);          //���ݴ�����  
 
	DMA_Cmd(DMAy_Channelx, ENABLE);                      //����DMA���� 
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

