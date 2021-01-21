/********************************************************************************
* @FileName: protocal.h
* @Author: Zhang Shen
* @Version: 1.0
* @Date: 2020-12-24
* @Description: The head file of protocal.c
********************************************************************************/

#ifndef _UART_PROTOCAL_H
#define _UART_PROTOCAL_H

#include "system.h"
#include "usart.h"
#include "led.h"

/***************************** PROTOCAL FORM ********************************/
/*0x55    0x00    0xXX    0xXX    |0xXX 0xXX 0xXX 0xXX	|    0x00    0xAA		*/
/*HEAD            ID     LENGTH 	|   1 float   (4*u8)  |            TAIL	  */
/****************************************************************************/

/*******************************协议格式*************************************/
/*0x55    0x00    0xXX    0xXX    |0xXX 0xXX 0xXX 0xXX	|    0x00    0xAA		*/
/*帧头            ID   数据长度  	|   1 float   (4*u8)  |            帧尾 	*/
/****************************************************************************/


/*---------------------------------------*/
/*       THE BUFFER(SEND&RX) SIZE        */
/*         发送&接受缓冲区 大小          */
/*---------------------------------------*/
#define COMM_RX_BUF_SIZE       12
#define COMM_SEND_BUF_SIZE    15

/*---------------------------------------*/
/*  THE STATE OF DEALING WITH MESSAGE    */
/*            消息处理状态位             */
/*---------------------------------------*/
#define COMM_RX_FREE_1         0
#define COMM_RX_FREE_2         1
#define COMM_RX_FLAG					 2
#define COMM_RX_START          3
#define COMM_RX_DATA           4
#define COMM_RX_END            5

/*---------------------------------------*/
/*            THE FUNCITON ID            */
/*                功能ID                 */
/*---------------------------------------*/
#define HEART_ID						0x01 

/*---------------------------------------*/
/*            THE DATA LENGTH            */
/*             数据规定长度              */
/*---------------------------------------*/
#define HEART_DATA_LENGTH			0

/*---------------------------------------*/
/*            EXTERN VARIABLES           */
/*               外部变量                */
/*---------------------------------------*/
extern u8 CommRxBuf[COMM_RX_BUF_SIZE];
extern u8 CommSendBuf[COMM_SEND_BUF_SIZE];

/*---------------------------------------*/
/*         THE FUNCTIONS DEFINE          */
/*                功能函数               */
/*---------------------------------------*/
void CommRx_IRQ(void);
void USART_Deal(u8 CmdFlag);

#endif
