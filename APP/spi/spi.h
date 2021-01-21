#ifndef _spi_H
#define _spi_H

#include "system.h"

#define SPI1_PIN 						GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7
#define SPI1_PORT						GPIOA
#define SPI1_PORT_RCC				RCC_APB2Periph_GPIOA

#define SPI2_PIN						GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15
#define SPI2_PORT						GPIOB
#define SPI2_PORT_RCC				RCC_APB2Periph_GPIOB

#define SPI2_CLK						PBout(13)
#define SPI2_MOSI						PBout(15)

void SPI1_Init(void);			 //��ʼ��SPI1��
void SPI1_SetSpeed(u8 SpeedSet); //����SPI1�ٶ�   
u8 SPI1_ReadWriteByte(u8 TxData);//SPI1���߶�дһ���ֽ�

void SPI2_Init(void);			 //��ʼ��SPI2��
void SPI2_SetSpeed(u8 SpeedSet); //����SPI2�ٶ�   
u8 SPI2_ReadWriteByte(u8 TxData);//SPI2���߶�дһ���ֽ�

u16 SPI2_ReadWrite_U16(u16 TxData);
void SPI2_Write_U16(u16 TxData);
u16 SPI2_Read_U16(void);
	
#endif
