/********************************************************************************
* @FileName: sensor.h
* @Author: Zhang Shen
* @Version: 1.0
* @Date: 2021-1-21
* @Description: The head file of sensor.c
********************************************************************************/

#ifndef _ADS8332_H
#define _ADS8332_H

#include "system.h"
#include "spi.h"
#include "SysTick.h"
#include "usart.h"

/*---------------------------------------*/
/*        THE CLK&GPIO&PORT DEFINE       */
/*---------------------------------------*/
// CS 
#define ADS8332_CS_CLK 				RCC_APB2Periph_GPIOB
#define ADS8332_CS_GPIO 			GPIOB
#define ADS8332_CS_PORT 			GPIO_Pin_6
// RESET
#define ADS8332_RESET_CLK 		RCC_APB2Periph_GPIOB
#define ADS8332_RESET_GPIO 		GPIOB
#define ADS8332_RESET_PORT 		GPIO_Pin_0
// CONVST
#define ADS8332_CONVST_CLK 		RCC_APB2Periph_GPIOB
#define ADS8332_CONVST_GPIO 	GPIOB
#define ADS8332_CONVST_PORT 	GPIO_Pin_10
// EOC/INT/CDI
#define ADS8332_EOC_CLK 			RCC_APB2Periph_GPIOB
#define ADS8332_EOC_GPIO 			GPIOB
#define ADS8332_EOC_PORT 			GPIO_Pin_1

/*---------------------------------------*/
/*         THE BIT OUT&IN DEFINE         */
/*---------------------------------------*/
#define	ADS8332_RESET 				PBout(0)
#define	ADS8332_CS 						PBout(6)
#define	ADS8332_CONVST 				PBout(10)
#define	ADS8332_EOC_IN	 			PBin(1)

/*---------------------------------------*/
/*         THE SIGNAL FLAG DEFINE        */
/*---------------------------------------*/
#define CS_SELECT 		0				//CS Selected
#define CS_IDLE   		1				//CS Unselected

/*---------------------------------------*/
/*  ADS8332 REGISTER & COMMAND DEFINES   */
/*---------------------------------------*/
#define ADS8332_Channel_0            ((uint16_t)0x0000)//Select analog input channel 0
#define ADS8332_Channel_1            ((uint16_t)0x1000)//Select analog input channel 1
#define ADS8332_Channel_2            ((uint16_t)0x2000)//Select analog input channel 2
#define ADS8332_Channel_3            ((uint16_t)0x3000)//Select analog input channel 3
#define ADS8332_Channel_4            ((uint16_t)0x4000)//Select analog input channel 4
#define ADS8332_Channel_5            ((uint16_t)0x5000)//Select analog input channel 5
#define ADS8332_Channel_6            ((uint16_t)0x6000)//Select analog input channel 6
#define ADS8332_Channel_7            ((uint16_t)0x7000)//Select analog input channel 7
#define ADS8332_WAKE_UP              ((uint16_t)0xB000)//Wake up
#define ADS8332_READ_CFR             ((uint16_t)0xC000)//Read CFR
#define ADS8332_READ_DATA            ((uint16_t)0xD000)//Read data
#define ADS8332_WRITE_CFR            ((uint16_t)0xE000)//Write CFR
#define ADS8332_DEFAULT_CFR          ((uint16_t)0xF000)//Default mode

#define ADS8332_CH_AUTO              ((uint16_t)0x0800)//Channel select mode
#define ADS8332_INT_CLK              ((uint16_t)0x0400)//Conversion clock (CCLK) source select
#define ADS8332_TR_MANUL             ((uint16_t)0x0200)//Trigger (conversion start) select
#define ADS8332_SMPL_250K            ((uint16_t)0x0100)//Sample rate for Auto-Trigger moden
#define ADS8332_EOC_LOW              ((uint16_t)0x0080)//Pin 10 polarity select when used as an
#define ADS8332_P10_EOC              ((uint16_t)0x0040)//Pin 10 polarity select when used as an
#define ADS8332_P10_OUT              ((uint16_t)0x0020)//Pin 10 I/O select for daisy-chain mode operation
#define ADS8332_AUTONAP_DIS          ((uint16_t)0x0010)//Auto-Nap Power-Down enable/disable
#define ADS8332_NAP_DIS              ((uint16_t)0x0008)//Nap Power-Down.
#define ADS8332_PD_DIS               ((uint16_t)0x0004)//Deep Power-Down
#define ADS8332_TAG_OUT              ((uint16_t)0x0002)//TAG bit output enable
#define ADS8332_NOSW_RST             ((uint16_t)0x0001)//Software reset

/*---------------------------------------*/
/*            ADS8332 PARAMETERS         */
/*---------------------------------------*/
#define ADS8332_RATIO  (0xFFFF)  //Precise Ratio
#define ADS8332_REF    (2500)    //Unit = mV

/*---------------------------------------*/
/*         THE FUNCTIONS DEFINE          */
/*---------------------------------------*/
void ADS8332_Init(void);
void ADS8332_Init_Port(void);
void ADS8332_Reset(void);
void ADS8332_Write_CFR(u16 cfr_data);
u16 ADS8332_Read_CFR(void);
void ADS8332_Convst(void);
void ADS8332_Channel_Sel(u16 Channel_x);
u16 ADS8332_Read_Data(u16 Channel_x);
void ADS8332_ERROR_Handler(void);
float ADS8332_Vol_to_Value(u16 value);
void ADS8332_Test(void);

#endif
