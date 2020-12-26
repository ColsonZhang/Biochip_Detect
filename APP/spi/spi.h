#ifndef _spi_H
#define _spi_H

#include "system.h"

#define SPI1_PIN 						GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7
#define SPI1_PORT						GPIOA
#define SPI1_PORT_RCC				RCC_APB2Periph_GPIOA

#define SPI2_PIN						GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15
#define SPI2_PORT						GPIOB
#define SPI2_PORT_RCC				RCC_APB2Periph_GPIOB



void SPI1_Init(void);			 //初始化SPI1口
void SPI1_SetSpeed(u8 SpeedSet); //设置SPI1速度   
u8 SPI1_ReadWriteByte(u8 TxData);//SPI1总线读写一个字节

void SPI2_Init(void);			 //初始化SPI2口
void SPI2_SetSpeed(u8 SpeedSet); //设置SPI2速度   
u8 SPI2_ReadWriteByte(u8 TxData);//SPI2总线读写一个字节

#endif
