/******************************************************************************
* 文 件  名 称:BspADS8332.c
* 文件功能概述:实现ADS8332的接口
* 文 件  作 者:
* 版        本:V1.0.0.0
* 修 订  记 录:2017-6-30创建
******************************************************************************/
 
/***************************相关配置*******************************************
             通过SPI进行操作，ADS8332将在下降沿取数据
               根据ADS8332的spi的极性要求配置CPOL = 1,CHPA=0
******************************************************************************/
 
#include "BspADS8332.h"
 
#define ADS8332_DelayUs(x) DelayUs(x)
 
/*
 * 数据类型-ADS8332的端口信息
 */
typedef struct ADS8332CtrlStruc
{
  E_SPIx     ADS8332xSPIx;
  S_GpioCtrl ADS8332xCS;
  S_GpioCtrl ADS8332xCONST;
  S_GpioCtrl ADS8332xRST;
  S_GpioCtrl ADS8332xEOC;
}S_ADS8332Ctrl;
 
/*
 * 用于记录ADS8332的端口信息
 */
static S_ADS8332Ctrl stADS8332Ctrl[E_ADS8332_Max] = 
{
   /* AD8332_1 */
  {
    E_SPI_2,
    {RCC_AHB1Periph_GPIOG,GPIOG,GPIO_Pin_2},    //CS
    {RCC_AHB1Periph_GPIOG,GPIOG,GPIO_Pin_4},    //CONST
    {RCC_AHB1Periph_GPIOG,GPIOG,GPIO_Pin_5},    //RST
    {RCC_AHB1Periph_GPIOG,GPIOG,GPIO_Pin_3}     //EOC
  },
  /* AD8332_2 */
  {
    E_SPI_1,
    {RCC_AHB1Periph_GPIOF,GPIOF,GPIO_Pin_15},    //CS
    {RCC_AHB1Periph_GPIOF,GPIOF,GPIO_Pin_13},    //CONST
    {RCC_AHB1Periph_GPIOF,GPIOF,GPIO_Pin_12},    //RST
    {RCC_AHB1Periph_GPIOF,GPIOF,GPIO_Pin_11}     //EOC
  },
};
 
/*
 *SPI总线上挂接的设备类型不一样，每次使用前均需对总线进行初始化
 */
static 
void AD8332InitSPIModule(E_ADS8332 eADS8332)
{
	SPI_InitTypeDef SPIxInitStruct;  
 
  if(eADS8332 < E_ADS8332_Max)	
	{
    /* Initialize the SPI_Direction member */
    SPIxInitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    /* initialize the SPI_Mode member */
    SPIxInitStruct.SPI_Mode = SPI_Mode_Master;
    /* initialize the SPI_DataSize member */
	  SPIxInitStruct.SPI_DataSize = SPI_DataSize_16b;
    /* Initialize the SPI_CPOL member */
    SPIxInitStruct.SPI_CPOL = SPI_CPOL_High;
    /* Initialize the SPI_CPHA member */
    SPIxInitStruct.SPI_CPHA = SPI_CPHA_1Edge;
    /* Initialize the SPI_NSS member */
    SPIxInitStruct.SPI_NSS = SPI_NSS_Soft;    
    /* Initialize the SPI_FirstBit member */
    SPIxInitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
    /* Initialize the SPI_CRCPolynomial member */
    SPIxInitStruct.SPI_CRCPolynomial = 7;
	  
	  SPIxUpdateModule(stADS8332Ctrl[eADS8332].ADS8332xSPIx,&SPIxInitStruct);
	}
	else
	{}
}
 
 
static __inline void ADS8332x_RST_SET(E_ADS8332 eADS8332)
{ 
  GPIOxWriteBit(stADS8332Ctrl[eADS8332].ADS8332xRST.GPIOx, 
                stADS8332Ctrl[eADS8332].ADS8332xRST.GPIOxPinx, 
                1);
}
static __inline void ADS8332x_RST_CLEAR(E_ADS8332 eADS8332)
{
	GPIOxWriteBit(stADS8332Ctrl[eADS8332].ADS8332xRST.GPIOx, 
                stADS8332Ctrl[eADS8332].ADS8332xRST.GPIOxPinx, 
                0);
}
 
static void ADS8332x_CONST_SET(E_ADS8332 eADS8332)
{
	GPIOxWriteBit(stADS8332Ctrl[eADS8332].ADS8332xCONST.GPIOx, 
                stADS8332Ctrl[eADS8332].ADS8332xCONST.GPIOxPinx, 
                1);
}
static __inline void ADS8332x_CONST_CLEAR(E_ADS8332 eADS8332)
{
	GPIOxWriteBit(stADS8332Ctrl[eADS8332].ADS8332xCONST.GPIOx, 
                stADS8332Ctrl[eADS8332].ADS8332xCONST.GPIOxPinx, 
                0);
}
 
static __inline void ADS8332x_CS_SELECTED(E_ADS8332 eADS8332)
{
	GPIOxWriteBit(stADS8332Ctrl[eADS8332].ADS8332xCS.GPIOx, 
                stADS8332Ctrl[eADS8332].ADS8332xCS.GPIOxPinx, 
                0);
}
static __inline void ADS8332x_CS_UNSELECTED(E_ADS8332 eADS8332)
{
	GPIOxWriteBit(stADS8332Ctrl[eADS8332].ADS8332xCS.GPIOx, 
                stADS8332Ctrl[eADS8332].ADS8332xCS.GPIOxPinx, 
                1);
}
 
static __inline uint8 ADS8332x_EOC_READ(E_ADS8332 eADS8332)
{
	return GPIOxReadBit(stADS8332Ctrl[eADS8332].ADS8332xEOC.GPIOx, 
                      stADS8332Ctrl[eADS8332].ADS8332xEOC.GPIOxPinx);
}
 
/*******************************************************************************
 * 函 数 名:static sint8 ADS8332xInitModule(E_ADS8332 eADS8332)
 * 参    数:E_ADS8332 eADS8332 :E_ADS8332_1 或  E_ADS8332_2
 * 返    回:0 ：正常返回 -1 ：发生错误
 * 创 建 人:
 * 创建时间:2017-6-30
 * 详    述:ADS8332模块初始化
            选用内部时钟做CCLK时:范围为10.9MHz-12.6Mhz  典型值为11.5MHz             
            选用SPI的时钟做CCLK来源时，CCLK=sclk/2:0.5-10.5MHz,故SPI的SCLK的范围应在1MHz-21MHz之间
 * 修改记录:2017-6-30创建
*******************************************************************************/
static sint8 ADS8332xInitModule(E_ADS8332 eADS8332)
{
  uint16 data=0x0000;
  
	if(eADS8332 < E_ADS8332_Max)
  {
	
    /* clock:internal; trigger:manual; sample；500kps */
    data = (WRITE_CFR | SELECT_INTERNAL_OSC | MANUAL_TRIGGER)<<8;
    
    //     EOC/INT:active low;      pin used:EOC; pin10:  EOC/INT output;
    //     Auto-NAP PWDN:disabled;  Nap PWDN:disable;     Deep PWDN:disable
    //     TAG bit:output disable;  operation:normal
    data |= EOC_ACTIVE_LOW | EOC_FUNCTION |
            AUTO_PWDN_DISABLE | NAP_PWDN_DISABLE | 
            DEEP_PWDN_DISABLE | NORMAL_OPERATION;
 
  	ADS8332x_CS_SELECTED(eADS8332);
      
  	SPIxWriteBytes(stADS8332Ctrl[eADS8332].ADS8332xSPIx,&data,1);
        
    ADS8332x_CS_UNSELECTED(eADS8332);
  }
	else
	{
		return -1;
	}
      
  return 0;
}
 
/*******************************************************************************
 * 函 数 名:sint8 ADS8332xInit(E_ADS8332 eADS8332)
 * 参    数:E_ADS8332 eADS8332:：详见E_AD8332
 * 返    回:0  ： 初始化完成       -1 ：SPI总线初始化失败
            -2 ： 端口初始化失败   -3 ：ADS8332配置失败
 * 创 建 人:
 * 创建时间:2017-6-30
 * 详    述:ADS8332初始化
            注意手册29页关于CPOL和CPHA的说明
 * 修改记录:2017-6-30创建
*******************************************************************************/
sint8 ADS8332xInit(E_ADS8332 eADS8332)
{
  sint8 ret = 0;
 
  if(eADS8332 < E_ADS8332_Max)
  {
    GPIO_InitTypeDef GPIO_InitStructure;
 
  	RCC_AHB1PeriphClockCmd(stADS8332Ctrl[eADS8332].ADS8332xCS.GPIOxSource
                         | stADS8332Ctrl[eADS8332].ADS8332xCONST.GPIOxSource
                         | stADS8332Ctrl[eADS8332].ADS8332xRST.GPIOxSource
                         | stADS8332Ctrl[eADS8332].ADS8332xEOC.GPIOxSource,
                           ENABLE);
    
    /***************************配置端口************************/   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    /*************配置CS端口*************/        
    GPIO_InitStructure.GPIO_Pin = stADS8332Ctrl[eADS8332].ADS8332xCS.GPIOxPinx;
    GPIO_Init(stADS8332Ctrl[eADS8332].ADS8332xCS.GPIOx, &GPIO_InitStructure); 
    
    /************配置CONST端口***********/
    GPIO_InitStructure.GPIO_Pin = stADS8332Ctrl[eADS8332].ADS8332xCONST.GPIOxPinx;
    GPIO_Init(stADS8332Ctrl[eADS8332].ADS8332xCONST.GPIOx, &GPIO_InitStructure);  
    
    /************配置RST端口***********/
    GPIO_InitStructure.GPIO_Pin = stADS8332Ctrl[eADS8332].ADS8332xRST.GPIOxPinx;
    GPIO_Init(stADS8332Ctrl[eADS8332].ADS8332xRST.GPIOx, &GPIO_InitStructure);  
    
    /************配置EOC端口**************/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Pin = stADS8332Ctrl[eADS8332].ADS8332xEOC.GPIOxPinx;
    GPIO_Init(stADS8332Ctrl[eADS8332].ADS8332xEOC.GPIOx, &GPIO_InitStructure); 
 
    ADS8332x_RST_SET(eADS8332);
    ADS8332x_CONST_SET(eADS8332);
    ADS8332x_CS_UNSELECTED(eADS8332);
    
    AD8332InitSPIModule(eADS8332);
    
    ADS8332xInitModule(eADS8332);
  }
  else
  {
    ret = -1;
  }
  
  return ret;
}
 
/*******************************************************************************
 * 函 数 名:static sint8 ADS8332xConvert(E_ADS8332 eADS8332)
 * 参    数:E_ADS8332 eADS8332 :E_ADS8332_1 或  E_ADS8332_2
 * 返    回:0 ： 转换正常完成  -1 ：转换超时 
 * 创 建 人:
 * 创建时间:2017-6-30
 * 详    述:ADS8332开始转换并等待转换完成
 * 修改记录:2017-6-30创建
*******************************************************************************/
static sint8 ADS8332xConvert(E_ADS8332 eADS8332)
{
  uint16 i = 0;
	
	if(eADS8332 < E_ADS8332_Max)
  {
	  ADS8332x_CONST_SET(eADS8332);
	  ADS8332_DelayUs(20);
    ADS8332x_CONST_CLEAR(eADS8332);
    ADS8332_DelayUs(20);
    ADS8332x_CONST_SET(eADS8332);
    ADS8332_DelayUs(20);
    while(ADS8332x_EOC_READ(eADS8332) == 0)
    {
      //wait eoc=1    
      if(i >= 1000)
      {
        return -1;
      }
       
      i ++;
    }  
	}
	else
	{
		return -1;
	}	
  
  return 0;
}
 
/*******************************************************************************
 * 函 数 名:static sint8 ADS8332xSelectChannel(E_ADS8332 eADS8332, uint8 channel)
 * 参    数:E_ADS8332 eADS8332 :E_ADS8332_1 或  E_ADS8332_2
            uint8 channel        ：要选择的通道
 * 返    回:0 ： 正常完成  -1 ：发生错误
 * 创 建 人:
 * 创建时间:2017-6-30
 * 详    述:ADS8332选择通道
 * 修改记录:2017-6-30创建
*******************************************************************************/
static sint8 ADS8332xSelectChannel(E_ADS8332 eADS8332, uint8 channel)
{
  uint16 data[2] = {0, 0};
  
	if(eADS8332 < E_ADS8332_Max)
  {
    data[0] = channel<<8;
	  
		ADS8332x_CS_SELECTED(eADS8332);
  
    SPIxWriteBytes(stADS8332Ctrl[eADS8332].ADS8332xSPIx,&data,1);
		
    ADS8332x_CS_UNSELECTED(eADS8332);	
  }
	else
	{
		return -1;
	}
  return 0;
}
 
/*******************************************************************************
 * 函 数 名:static sint8 ADS8332xReadValue(E_ADS8332 eADS8332, uint16 *p_data)
 * 参    数:E_ADS8332 eADS8332 :E_ADS8332_1 或  E_ADS8332_2
            uint8 *p_data        :读取的数据 
 * 返    回:0 ： 正常完成  -1 ：发生错误
 * 创 建 人:
 * 创建时间:2017-6-30
 * 详    述:ADS8332发送数据读取命令并读取数据
 * 修改记录:2017-6-30创建
*******************************************************************************/
static sint8 ADS8332xReadValue(E_ADS8332 eADS8332, uint16 *p_data)
{
  uint16 data[2] = {0xFFFF,0xFFFF};
	
  if(eADS8332 < E_ADS8332_Max)
  {
    data[0] = 0xD000;
 
	  ADS8332x_CS_SELECTED(eADS8332);
      
    SPIxWriteReadByte(stADS8332Ctrl[eADS8332].ADS8332xSPIx, &data[0], &data[1]);
      
    ADS8332x_CS_UNSELECTED(eADS8332);
 
	  *p_data = data[1];
  }
	else
	{
		return -1;
	}
	
  return 0;
}
 
/*******************************************************************************
 * 函 数 名:sint8 ADS8332xGetValue(E_ADS8332 eADS8332, uint8 channel, uint16 *p_data)
 * 参    数:E_ADS8332 eADS8332 : E_ADS8332_1 或  E_ADS8332_2
            uint8 channel        : 通道
             uint16 *p_data      : 数据地址 
 * 返    回:0 ： 正常完成  -1 ：发生错误
 * 创 建 人:
 * 创建时间:2017-6-30
 * 详    述:ADS8332发送转换命令并读取转换结果
 * 修改记录:2017-6-30创建
*******************************************************************************/
sint8 ADS8332xGetValue(E_ADS8332 eADS8332, uint8 channel, uint16 *p_data)
{
  if(eADS8332 < E_ADS8332_Max)
  {
	  AD8332InitSPIModule(eADS8332);//SPI总线上挂接的设备类型不一样，每次使用前均需对总线进行初始化
	  
    ADS8332xSelectChannel(eADS8332, channel);
        
    if(ADS8332xConvert(eADS8332) == -1)
    {
      return -1;
    }
	  else
	  {}
    
    ADS8332xReadValue(eADS8332, p_data);        
	}
	else
	{
		return -1;
	}
	
  return 0;
}
 
/*******************************************************************************
 * 函 数 名:sint8 ADS8332xReset(E_ADS8332 eADS8332)
 * 参    数:E_ADS8332 eADS8332 : E_ADS8332_1 或  E_ADS8332_2
 * 返    回:0 ： 正常完成  -1 ：发生错误
 * 创 建 人:
 * 创建时间:2017-6-30
 * 详    述:ADS8332复位   拉低至少25ns
 * 修改记录:2017-6-30创建
*******************************************************************************/
sint8 ADS8332xReset(E_ADS8332 eADS8332)
{
	if(eADS8332 < E_ADS8332_Max)
  {
	  ADS8332x_RST_CLEAR(eADS8332);
  
    ADS8332_DelayUs(10);
  
    ADS8332x_RST_SET(eADS8332);
  }
	else
	{
		return -1;
	}
  return 0;
}
 
/*******************************************************************************
 * 函 数 名:float ADS8332xVolToValue(uint16 value)
 * 参    数:uint16 value: 需要转化的值
 * 返    回:对应的电压值
 * 创 建 人:
 * 创建时间:2017-6-30
 * 详    述:获得指定值对应的电压值，单位是mv
 * 修改记录:2017-6-30创建
*******************************************************************************/
float ADS8332xVolToValue(uint16 value)
{
  float temp = 0;
  
  temp = (float)((float)(value) / ADS8332_RATIO);
  
  return  (float)(temp * ADS8332_REF);
}