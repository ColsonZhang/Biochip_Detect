/********************************************************************************
* @FileName: protocal.c
* @Author: Zhang Shen
* @Version: 1.0
* @Date: 2020-12-24
* @Description: The protocal dealing with the communication
********************************************************************************/

#include "protocal.h"

u8 CommRxBuf[COMM_RX_BUF_SIZE]={0};
u8 CommSendBuf[COMM_SEND_BUF_SIZE]={0};

/*********************************************************
* Function_Name : CommRx_IRQ
* Parameter     : void
* Return        : void
* Description   : Sort the data from the serial;
									Actually this is a state-machine.
* Author				: Zhang Shen
* Create_Time   : 2021-01-21
* Modify_Record : null
**********************************************************/
void CommRx_IRQ(void){
	static u8 Comm_Rx_Status = COMM_RX_FREE_1;
	static u8 ucPit = 0;
	static u8 ucDataLength = 0;
	u8 i=0;
	u8 ucData = 0 ;
	u8 CmdFlag = 0 ;
	for(i=0;i<12;i++)
	{
		ucData = CommRxBuf[i];
		switch(Comm_Rx_Status)
		{
			// ----- case 1 -----
			case COMM_RX_FREE_1 :
				if(ucData == 0x55)
				{
					// Begin the movement of receiving data
					// 自由状态下接收到0x55认为开始
					Comm_Rx_Status = COMM_RX_FREE_2;
				}
				break;
			
			// ----- case 2 -----
			case COMM_RX_FREE_2:
				if(ucData == 0x00)
				{
					// Receive the head-data
					// 接收帧头开始接收数据
					Comm_Rx_Status = COMM_RX_FLAG;  
				}
				else
				{
					Comm_Rx_Status = COMM_RX_FREE_1;
				}
				break;
			// ----- case 3 -----
			case COMM_RX_FLAG:
				CmdFlag = HEART_ID;
				ucDataLength = HEART_DATA_LENGTH;
				Comm_Rx_Status = COMM_RX_START;
				break;
			// ----- case 4 -----
			case COMM_RX_START:
				if(ucData == ucDataLength)
				{
					Comm_Rx_Status = COMM_RX_DATA;
					ucPit =0;
				}
				else
				{
					Comm_Rx_Status = COMM_RX_FREE_1;
				}
				break;
			// ----- case 5 -----
			case COMM_RX_DATA:
				if(ucPit < ucDataLength)
				{
					*(CommRxBuf + ucPit) = ucData;
					ucPit++;					
				}
				else
				{
					if(ucData == 0x00)
					{
						Comm_Rx_Status = COMM_RX_END;
					}
					else
					{
						Comm_Rx_Status = COMM_RX_FREE_1;
					}
				}
				break;
			// ----- case 6 -----
			case COMM_RX_END:
				if(ucData == 0xAA)
				{
					USART_Deal(CmdFlag);
					//printf("received");
				}
				Comm_Rx_Status = COMM_RX_FREE_1;
				break;

			default:
				break;
		}
	}
}


/*********************************************************
* Function_Name : USART_Deal
* Parameter     : void
* Return        : void
* Description   : Deal the events according to the flag
* Author				: Zhang Shen
* Create_Time   : 2021-01-21
* Modify_Record : null
**********************************************************/
void USART_Deal(u8 CmdFlag)
{
	static u8 heart = 0 ;
	switch(CmdFlag)
	{
		case HEART_ID: 
		{
			heart = heart + 1 ;
			if( heart <= 2 )
			{
				led2 = 0;
			}
			else
			{
				led2 = 1 ;
				heart = 0 ;
			}
			break;	
		}
		default:
			break;
		
	}
}
