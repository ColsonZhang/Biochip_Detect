/******************************************************************************
* 文 件  名 称:BspADS8332.h
* 文件功能概述:实现ADS8332驱动接口声明
* 文 件  作 者:赵四方
* 版        本:V1.0.0.0
* 修 订  记 录:2017-6-30创建
******************************************************************************/
 
#ifndef __BSP_ADS8332_H__
#define __BSP_ADS8332_H__
 
 
/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "..\BspInterface.h"
/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
 
/******************内部寄存器 CMR（高4位） CFR（低12位）***************************************/ 
 
/*******************CMR 位定义********************************/ 
#define SELECT_CHANNEL_0 (0x00 << 4)
#define SELECT_CHANNEL_1 (0x01 << 4)
#define SELECT_CHANNEL_2 (0x02 << 4)
#define SELECT_CHANNEL_3 (0x03 << 4)
#define SELECT_CHANNEL_4 (0x04 << 4)
#define SELECT_CHANNEL_5 (0x05 << 4)
#define SELECT_CHANNEL_6 (0x06 << 4)
#define SELECT_CHANNEL_7 (0x07 << 4)
                         
#define WAKE_UP          (0x0B << 4)
                         
#define READ_CFR         (0x0C << 4)
#define READ_DATA        (0x0D << 4)
#define WRITE_CFR        (0x0E << 4)
                         
#define DEFAULT_MODE     (0x0F << 4)
 
/******************CFR 位定义***********************************/
#define AUTO_CHANNEL_SELECT   (1 << 3)
#define SELECT_INTERNAL_OSC   (1 << 2)
#define MANUAL_TRIGGER        (1 << 1)
#define SAMPLE_250KPS         (1 << 0)
 
#define EOC_ACTIVE_LOW        (1 << 7)
#define EOC_FUNCTION          (1 << 6)
#define PIN10_FOR_EOC         (1 << 5)
#define AUTO_PWDN_DISABLE     (1 << 4)
#define NAP_PWDN_DISABLE      (1 << 3)
#define DEEP_PWDN_DISABLE     (1 << 2)
#define TAG_BIT_OUTPUT        (1 << 1)
#define NORMAL_OPERATION      (1 << 0)     
 
/**********************************************/
#define BIT(X) (1ul << X)
#define ADS8332_CMD_SEL_Ch_0    0x0000
#define ADS8332_CMD_SEL_Ch_1    0x1000
#define ADS8332_CMD_SEL_Ch_2    0x2000
#define ADS8332_CMD_SEL_Ch_3    0x3000
#define ADS8332_CMD_SEL_Ch_4    0x4000
#define ADS8332_CMD_SEL_Ch_5    0x5000
#define ADS8332_CMD_SEL_Ch_6    0x6000
#define ADS8332_CMD_SEL_Ch_7    0x7000
#define ADS8332_CMD_WAKE_UP     0xB000
#define ADS8332_CMD_READ_CFR    0xC000
#define ADS8332_CMD_READ_DATA   0xD000
#define ADS8332_CMD_WRITE_CFR   0xE000
#define ADS8332_CMD_DEFAULT_CFR 0xF000
 
//
//配置寄存器 (CFR)位定义
//
#define ADS8332_CFR_AUTO_CH        		BIT(11)
#define ADS8332_CFR_CCLK_INTERNAL    	BIT(10)
#define ADS8332_CFR_MAN_TRG          	BIT(9)
#define ADS8332_CFR_SAMPLE_RATE      	BIT(8)
#define ADS8332_CFR_POL_INT_EOC_LOW  	BIT(7)
#define ADS8332_CFR_PIN10_EOC        	BIT(6)
#define ADS8332_CFR_PIN10_OUTPUT     	BIT(5)
#define ADS8332_CFR_AUTO_NAP_DISABLE  BIT(4)
#define ADS8332_CFR_NAP_PD_DISABLE    BIT(3)
#define ADS8332_CFR_DEEP_PWD_DISABLE  BIT(2)
#define ADS8332_CFR_TAG            		BIT(1)
#define ADS8332_CFR_RESET         		BIT(0)
 
/*********************************************/
 
#define ADS8332_RATIO  (0xFFFF)  //精度是65535
#define ADS8332_REF    (2500)   //单位是mv
 
typedef enum
{
  E_ADS8332_1=0x00,
  E_ADS8332_2,
  
	E_ADS8332_Max,
  
  E_ADS8332_Invalid
}E_ADS8332;
 
/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/
 
/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/
 
/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/
 
/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/
 
/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/
 
/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/
 
sint8 ADS8332xInit(E_ADS8332 eADS8332);
 
sint8 ADS8332xGetValue(E_ADS8332 eADS8332, uint8 channel, uint16 *p_data);
 
sint8 ADS8332xReset(E_ADS8332 eADS8332);
 
float ADS8332xVolToValue(uint16 value);
 
#endif
