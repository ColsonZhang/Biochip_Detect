/**
  ******************************************************************************
  * @file    T_ADS8332.h
  * @author  LuJ
  * @version V3.5.0
  * @date    2015-04-02
  * @brief   16位ADC头文件管理
  ******************************************************************************
  * (C) Copyright Vango Technoglogy, Inc. 2015
  ******************************************************************************
  */
#ifndef __T_ADS8332_H
#define __T_ADS8332_H   

#include "stm32f10x.h"

//ADS8332 register and command defines
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
#define ADS8332_PD_DIS                ((uint16_t)0x0004)//Deep Power-Down
#define ADS8332_TAG_OUT              ((uint16_t)0x0002)//TAG bit output enable
#define ADS8332_NOSW_RST             ((uint16_t)0x0001)//Software reset

//位带操作,实现51类似的GPIO控制功能
//具体实现思想,参考<<CM3权威指南>>第五章(87页~92页).
#define ADS8332_RESET_OUT   *((volatile unsigned long  *)((((GPIOC_BASE+12) & 0xF0000000)+0x2000000+(((GPIOC_BASE+12) &0xFFFFF)<<5)+(4<<2))))//PC4
#define ADS8332_CONVST_OUT  *((volatile unsigned long  *)((((GPIOC_BASE+12) & 0xF0000000)+0x2000000+(((GPIOC_BASE+12) &0xFFFFF)<<5)+(5<<2))))//PC5
#define ADS8332_EOC_IN      *((volatile unsigned long  *)((((GPIOA_BASE+8 ) & 0xF0000000)+0x2000000+(((GPIOA_BASE+8 ) &0xFFFFF)<<5)+(4<<2))))//PA4
#define ADS8332_SCK_OUT     *((volatile unsigned long  *)((((GPIOA_BASE+12) & 0xF0000000)+0x2000000+(((GPIOA_BASE+12) &0xFFFFF)<<5)+(5<<2))))//PA5
#define ADS8332_MISO_IN     *((volatile unsigned long  *)((((GPIOA_BASE+8 ) & 0xF0000000)+0x2000000+(((GPIOA_BASE+8 ) &0xFFFFF)<<5)+(6<<2))))//PA6
#define ADS8332_MOSI_OUT    *((volatile unsigned long  *)((((GPIOA_BASE+12) & 0xF0000000)+0x2000000+(((GPIOA_BASE+12) &0xFFFFF)<<5)+(7<<2))))//PA7

void T_ADS8332_Init(void);//初始化
void T_ADS8332_Reset(void);
void T_ADS8332_Convst(void);
uint16_t T_ADS8332_ReadCFR(void);
void T_ADS8332_WriteCFR(uint16_t cfr_data);
void T_ADS8332_ChannelSel(uint16_t ADS8332_Channel_x);
uint16_t T_ADS8332_ReadData(uint16_t ADS8332_Channel_x);
void T_ADS8332_Test(void);
        
#endif
