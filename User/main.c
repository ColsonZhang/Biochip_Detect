#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "spi.h"
#include "dma.h"
#include "protocal.h"
#include "sensor.h"

void heart_beat(void)
{
			CommSendBuf[0]=0x55;
			CommSendBuf[1]=0x00;
			CommSendBuf[2]=0x01;
			CommSendBuf[3]=0x00;
			CommSendBuf[4]=0x00;
			CommSendBuf[5]=0xAA;
			USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
			DMAx_Enable(DMA1_Channel4,6);		
			while(1)
			{
				if(DMA_GetFlagStatus(DMA1_FLAG_TC4)!=0)//判断通道4传输完成
				{
					DMA_ClearFlag(DMA1_FLAG_TC4);
					break;
				}
			}
}
/*******************************************************************************
* 函 数 名         : main
* 函数功能		   : 主函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
int main()
{
	u8 i=0; 
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  // 中断优先级分组 分2组
	
	DMAx_Rx_Init(DMA1_Channel5,(u32)&USART1->DR,(u32)CommRxBuf,COMM_RX_BUF_SIZE);	// DMA_Channel5初始化
	DMAx_Send_Init(DMA1_Channel4,(u32)&USART1->DR,(u32)CommSendBuf,COMM_SEND_BUF_SIZE);	// DMA_Channel4初始化
	
	Sensor_Init();
	LED_Init();
	USART1_Init(9600);

	while(1)
	{
		i++;

		if(i%20==0)
		{
			led1=!led1;
		}
		if(i==200)
		{
			//heart_beat();
			Sensor_Read();
			i=0;
		}
		delay_ms(10);
	}
}


