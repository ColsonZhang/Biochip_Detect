#ifndef _UART_PROTOCAL_H
#define _UART_PROTOCAL_H

#include "system.h"
#include "usart.h"
#include "led.h"

/*******************************Э���ʽ*************************************/
/*0x55    0x00    0xXX    0xXX    |0xXX 0xXX 0xXX 0xXX	|    0x00    0xAA		*/
/*֡ͷ            ID   ���ݳ���  	|   1 float   (4*u8)  |            ֡β 	*/
/****************************************************************************/

#define COMM_RX_BUF_SIZE       12
#define COMM_SEND_BUF_SIZE    15

/*			��Ϣ����״̬λ			*/
#define COMM_RX_FREE_1         0
#define COMM_RX_FREE_2         1
#define COMM_RX_FLAG					 2
#define COMM_RX_START          3
#define COMM_RX_DATA           4
#define COMM_RX_END            5

/*       ����ID          */
#define HEART_ID						0x01 

/*				���ݹ涨����   */
#define HEART_DATA_LENGTH			0

extern u8 CommRxBuf[COMM_RX_BUF_SIZE];
extern u8 CommSendBuf[COMM_SEND_BUF_SIZE];

void CommRx_IRQ(void);
void USART_Deal(u8 CmdFlag);

#endif
