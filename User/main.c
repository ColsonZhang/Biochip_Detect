/********************************************************************************
* @FileName: main.c
* @Author: Zhang Shen
* @Version: 1.0
* @Date: 2021-1-21
* @Description: The main function of the Project
********************************************************************************/
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
				// Judge whetheer the transmission finished
				// 判断传输是否完成
				if(DMA_GetFlagStatus(DMA1_FLAG_TC4)!=0)
				{
					DMA_ClearFlag(DMA1_FLAG_TC4);
					break;
				}
			}
}

/*********************************************************
* Function_Name : main
* Parameter     : void
* Return        : void
* Description   : The main function
* Author				: Zhang Shen
* Create_Time   : 2021-01-21
* Modify_Record : null
**********************************************************/
int main()
{
	u8 i=0; 
	SysTick_Init(72);
	// Divide the NVIC into 2 group
	// 将优先级分为两组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	// Initialize the DMA_Channel5 & Channel4
	// 初始化 DMA_Channel5 & DMA_Channel4 
	DMAx_Rx_Init(DMA1_Channel5,(u32)&USART1->DR,(u32)CommRxBuf,COMM_RX_BUF_SIZE);	
	DMAx_Send_Init(DMA1_Channel4,(u32)&USART1->DR,(u32)CommSendBuf,COMM_SEND_BUF_SIZE);
	
	ADS8332_Init();
	LED_Init();
	USART1_Init(9600);
	ADS8332_Init();
	
	while(1)
	{
		i++;

		if(i%20==0)
		{
			led1=!led1;
		}
		if(i==200)
		{
			heart_beat();
			//Sensor_Read();
			i=0;
		}
		delay_ms(10);
	}
}


