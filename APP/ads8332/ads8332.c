/**
  ******************************************************************************
  * @file    T_ADS8332.c
  * @author  LuJ
  * @version V3.5.0
  * @date    2015-04-02
  * @brief   ADS8332模数转换器 驱动
  ******************************************************************************
  * (C) Copyright Vango Technoglogy, Inc. 2015
  ******************************************************************************
  */
#include "stm32f10x_spi.h"
#include "HW_lib.h"
#include "st_lib.h"
 
///**
//  * 函数功能:ADS8332模数转换器初始化,主要初始化STM32的SPI1
//  * 输入参数:无
//  * 输出参数:无
//  * 全局变量:无  
//  * 作者:LuJ
//  * 修改日期:2015/04/01
//  * 备注:无
//  */
//void T_ADS8332_Init(void)
//{
//  SPI_InitTypeDef  SPI_InitStructure;
//  GPIO_InitTypeDef GPIO_InitStructure;
//   
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE);
//  RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1, DISABLE);
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 ,ENABLE);
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
//  
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;//SCK   MISO   MOSI
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//  GPIO_Init(GPIOA, &GPIO_InitStructure);
//  
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;//复位信号, 采样启动信号CONVST
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//  GPIO_Init(GPIOC, &GPIO_InitStructure);
//  
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;//采样结束标志信号EOC
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//  GPIO_Init(GPIOA, &GPIO_InitStructure);
//  
//  /* SPI1 configuration */ 
//  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
//  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
//  SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
//  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
//  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
//  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
//  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
//  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
//  SPI_InitStructure.SPI_CRCPolynomial = 7;
//  SPI_Init(SPI1, &SPI_InitStructure);

//  /* Enable SPI1  */
//  SPI_Cmd(SPI1, ENABLE);                  
//}

/**
  * 函数功能:ADS8332模数转换器初始化,主要初始化STM32的GPIO.同时复位ADS8332芯片,写读配置信息
  * 输入参数:无
  * 输出参数:无
* 全局变量:无  
* 作者:LuJ
* 修改日期:2015/04/08
* 备注:无
  */
void T_ADS8332_Init(void)
{
uint16_t cfr_old_config=0x0000,cfr_config=0x0000;
  GPIO_InitTypeDef GPIO_InitStructure;
   
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;//复位信号RESET, 采样启动信号CONVST
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_6;//采样结束标志信号EOC,   MISO
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_7;//SCK  MOSI
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  

T_ADS8332_Reset();//复位ADS8332芯片
cfr_old_config = (ADS8332_WRITE_CFR|ADS8332_INT_CLK|ADS8332_TR_MANUL|ADS8332_SMPL_250K|ADS8332_EOC_LOW\
|ADS8332_P10_EOC|ADS8332_P10_OUT|ADS8332_AUTONAP_DIS|ADS8332_NAP_DIS|ADS8332_PD_DIS|ADS8332_NOSW_RST);//配置工作模式0xE7FD,具体见手册
T_ADS8332_WriteCFR(cfr_old_config);
cfr_config=T_ADS8332_ReadCFR();
while((0x0FFF&cfr_config) != (0x0FFF&cfr_old_config));//比较写入读出的配置相同,否则错误死循环,待特殊处理
}

/**
  * 函数功能:ADS8332模数转换器软件复位
  * 输入参数:无
  * 输出参数:无
* 全局变量:无  
* 作者:LuJ
* 修改日期:2015/04/03
* 备注:无
  */
void T_ADS8332_Reset(void)
{  
ADS8332_SCK_OUT=0;  
T_ADS8332_SPI1_CS1(Bit_SET);//CS=1
ADS8332_MOSI_OUT=1;
ADS8332_CONVST_OUT=1;
ADS8332_RESET_OUT=1;
Delay_ms(1);
ADS8332_RESET_OUT=0;//minimum  25ns
Delay_ms(1);
ADS8332_RESET_OUT=1;
Delay_ms(1);
}

/**
  * 函数功能:ADS8332模数转换器开始转换
  * 输入参数:无
  * 输出参数:无
* 全局变量:无  
* 作者:LuJ
* 修改日期:2015/04/03
* 备注:CONVST信号下降沿开始采集
  */
void T_ADS8332_Convst(void)
{
T_ADS8332_SPI1_CS1(Bit_SET);//CS=1
ADS8332_CONVST_OUT=1;
Delay_ms(1);
ADS8332_CONVST_OUT=0;//minimum 40ns
Delay_ms(1);
ADS8332_CONVST_OUT=1;
}

/**
  * 函数功能:ADS8332模数转换器读配置信息
  * 输入参数:无
  * 输出参数:无
* 全局变量:无  
* 作者:LuJ
* 修改日期:2015/04/03
* 备注:下降沿,D15:12 1100b  D11:D0 Don't care
  */
uint16_t T_ADS8332_ReadCFR(void)
{
uint8_t temp_i;
uint16_t temp_b = 0x8000;
uint8_t temp_c;
uint16_t temp_d = ADS8332_READ_CFR;
uint16_t ConFR  = 0x0000;
ADS8332_SCK_OUT=1;
T_ADS8332_SPI1_CS1(Bit_RESET);//CS=0
for(temp_i = 0;temp_i < 16;temp_i++)
{
if(temp_d & temp_b)
ADS8332_MOSI_OUT=1;
else
ADS8332_MOSI_OUT=0;    
temp_c = ADS8332_MISO_IN;
if(temp_c > 0)
 ConFR = ConFR | temp_b;
else; 
ADS8332_SCK_OUT=0;//注意片选信号拉低时候第一个数据已经传输了,先读取再将时钟产生下降沿
 temp_b = temp_b >> 1;  
ADS8332_SCK_OUT=1;             
}
  T_ADS8332_SPI1_CS1(Bit_SET);//CS=1
return ConFR;  
}

/**
  * 函数功能:ADS8332模数转换器写配置信息
  * 输入参数:cfr_data:写入的配置数据
  * 输出参数:无
* 全局变量:无  
* 作者:LuJ
* 修改日期:2015/04/07
* 备注:下降沿,D15:12 1110b  D11:D0 Don't care
  */
void T_ADS8332_WriteCFR(uint16_t cfr_data)
{
uint8_t temp_a;
uint16_t temp_b = 0x8000;
uint8_t temp_c;
  ADS8332_SCK_OUT=1;
T_ADS8332_SPI1_CS1(Bit_RESET);//CS=0
cfr_data = (ADS8332_WRITE_CFR | (0x0fff&cfr_data));//0xE000=0b 1100 0000 0000 0000
for(temp_a = 0;temp_a < 16;temp_a++)
{
if(cfr_data & temp_b)
ADS8332_MOSI_OUT=1;
else
ADS8332_MOSI_OUT=0;
temp_b = temp_b >> 1;
ADS8332_SCK_OUT=0;  
ADS8332_SCK_OUT=1;             
}
  T_ADS8332_SPI1_CS1(Bit_SET);//CS=1
}

/**
  * 函数功能:ADS8332模数转换器通道选择
  * 输入参数:ADS8332_Channel_x:选择的通道
  * 输出参数:无
* 全局变量:无  
* 作者:LuJ
* 修改日期:2015/04/07
* 备注:
  */
void T_ADS8332_ChannelSel(uint16_t ADS8332_Channel_x)
{
uint8_t temp_i;
uint16_t temp_b = 0x8000;
uint16_t temp_c = 0x0000;

ADS8332_SCK_OUT=1;    
T_ADS8332_SPI1_CS1(Bit_RESET);//CS=0
temp_c = temp_c | ADS8332_Channel_x;
for(temp_i = 0;temp_i < 16;temp_i++)
{
if(temp_c & temp_b)
ADS8332_MOSI_OUT=1;
else
ADS8332_MOSI_OUT=0;
temp_b = temp_b >> 1;
ADS8332_SCK_OUT=0;     
ADS8332_SCK_OUT=1;   
}
  T_ADS8332_SPI1_CS1(Bit_SET);//CS=1
}

/**
  * 函数功能:ADS8332模数转换器ADC采集,读取数据
  * 输入参数:ADS8332_Channel_x:选择的通道
  * 输出参数:读取到的数据
* 全局变量:无  
* 作者:LuJ
* 修改日期:2015/04/02
* 备注:无
  */
uint16_t T_ADS8332_ReadData(uint16_t ADS8332_Channel_x)
{
uint8_t temp_a;
uint16_t temp_b = 0x8000;
uint8_t temp_c;
uint16_t temp_d = 0xD000;
uint16_t ConFR = 0x0000;
T_ADS8332_ChannelSel(ADS8332_Channel_x);
//channel sel
T_ADS8332_Convst();   
temp_a = ADS8332_EOC_IN;
while(!temp_a)
{
temp_a = ADS8332_EOC_IN;
}
//check convert over
ADS8332_SCK_OUT=1;   
T_ADS8332_SPI1_CS1(Bit_RESET);//CS=0
for(temp_a = 0;temp_a < 16;temp_a++)
{
if(temp_d & temp_b)
ADS8332_MOSI_OUT=1;
else
ADS8332_MOSI_OUT=0;    
temp_c = ADS8332_MISO_IN;
if(temp_c > 0)
 ConFR = ConFR | temp_b;
else;
ADS8332_SCK_OUT=0;//注意片选信号拉低时候第一个数据已经传输了,先读取再将时钟产生下降沿
temp_b = temp_b >> 1;  
ADS8332_SCK_OUT=1;   
}  
  T_ADS8332_SPI1_CS1(Bit_SET);//CS=1
return ConFR;  
}
 
/**
  * 函数功能:ADS8332模数转换器测试
  * 输入参数:无
  * 输出参数:无
* 全局变量:无  
* 作者:LuJ
* 修改日期:2015/04/01
* 备注:在主函数中调用
  */
void T_ADS8332_Test(void)
{
uint16_t ads8332_val;
float Voltage_val;
V_74HC595_Init();//初始化移位寄存器,TLV5618的片选信号和模拟开关控制信号在此
T_TLV5618_Init();//初始化STM32的GPIO
T_Target_VDCIN(Bit_SET);//VDCIN=1
T_Target_V3V(Bit_RESET);//V3V=0
T_Target_MODEx(Bit_RESET,Bit_SET);//MODE0=0,MODE1=1
T_TLV5618_Conver(0x08bb,0x0000,TLV5618_Channal_A,TLV5618_Fast_mode);//A通道输出,快速模式,电源控制位正常
T_Target_vPower_ctr(Target_SET_vPower_ctr1);//打开电源控制开关
T_Target_RSTn();
T_Target_LDO33_ctr(Target_SET_LDO33_ctr2);
T_ADS8332_Init();//初始化STM32使用到的GPIO
while (1)
{
ads8332_val=T_ADS8332_ReadData(ADS8332_Channel_0);
Voltage_val=((float)ads8332_val*3.3f)/65536.0f;
printf("\r\nvPower_test1=%6f  \r\n",Voltage_val);
ads8332_val=T_ADS8332_ReadData(ADS8332_Channel_1);
Voltage_val=((float)ads8332_val*3.3f)/65536.0f;
printf("\r\nvPower_test2=%6f  \r\n",Voltage_val);
ads8332_val=T_ADS8332_ReadData(ADS8332_Channel_2);
Voltage_val=((float)ads8332_val*3.3f)/65536.0f;
printf("\r\nvPower_test3=%6f  \r\n",Voltage_val);
ads8332_val=T_ADS8332_ReadData(ADS8332_Channel_3);
Voltage_val=((float)ads8332_val*3.3f)/65536.0f;
printf("\r\nvPower_test4=%6f  \r\n",Voltage_val);
ads8332_val=T_ADS8332_ReadData(ADS8332_Channel_4);
Voltage_val=((float)ads8332_val*3.3f)/65536.0f;
printf("\r\nT_REF=%6f  \r\n",Voltage_val);
ads8332_val=T_ADS8332_ReadData(ADS8332_Channel_5);
Voltage_val=((float)ads8332_val*3.3f)/65536.0f;
printf("\r\nT_LDO1.65=%6f  \r\n",Voltage_val);
ads8332_val=T_ADS8332_ReadData(ADS8332_Channel_6);
Voltage_val=((float)ads8332_val*3.3f)/65536.0f;
printf("\r\nT_DVCC=%6f  \r\n",Voltage_val);
ads8332_val=T_ADS8332_ReadData(ADS8332_Channel_7);
Voltage_val=((float)ads8332_val*3.3f)/65536.0f;
printf("\r\nT_BAT=%6f  \r\n",Voltage_val);
Delay_ms(500);
}
}