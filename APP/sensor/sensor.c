#include "sensor.h"

/* ADS8332 初始化 */
void ADS8332_Init(void)
{
	u16 cfr_config_write = 0x0000 ;
	u16 cfr_config_read	 = 0x0000 ;
	ADS8332_Init_Port();
	ADS8332_Reset();
	// 写入配置信息
	cfr_config_write = (ADS8332_WRITE_CFR|ADS8332_INT_CLK|ADS8332_TR_MANUL|ADS8332_SMPL_250K|ADS8332_EOC_LOW\
										|ADS8332_P10_EOC|ADS8332_P10_OUT|ADS8332_AUTONAP_DIS|ADS8332_NAP_DIS|ADS8332_PD_DIS|ADS8332_NOSW_RST);
	ADS8332_Write_CFR(cfr_config_write);
	// 检查配置信息是否正确写入
	cfr_config_read = ADS8332_Read_CFR();
	if(cfr_config_read != cfr_config_write)
	{
		ADS8332_ERROR_Handler();
	}
	
}

/* ADS8332 需要用到的端口进行初始化 */
void ADS8332_Init_Port(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	// 开启时钟
	RCC_APB2PeriphClockCmd(ADS8332_CS_CLK|ADS8332_RESET_CLK|ADS8332_CONVST_CLK|ADS8332_EOC_CLK, ENABLE);
	
	// 设置CS端口为推挽输出
	GPIO_InitStructure.GPIO_Pin = ADS8332_CS_PORT;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(ADS8332_CS_GPIO, &GPIO_InitStructure);
	
	// 设置RESET端口为推挽输出
	GPIO_InitStructure.GPIO_Pin = ADS8332_RESET_PORT;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(ADS8332_RESET_GPIO, &GPIO_InitStructure);
	
	// 设置CONVST端口为推挽输出
	GPIO_InitStructure.GPIO_Pin = ADS8332_CONVST_PORT;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(ADS8332_CONVST_GPIO, &GPIO_InitStructure);	
	
	// 设置EOC端口为上拉输入
	GPIO_InitStructure.GPIO_Pin = ADS8332_EOC_PORT;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(ADS8332_EOC_GPIO, &GPIO_InitStructure);	

	// SPI2端口初始化
	SPI2_Init();
}

/* ADS8332复位 */
void ADS8332_Reset(void)
{
	SPI2_CLK 				= 0 ;
	SPI2_MOSI				= 1 ;
	ADS8332_CS			= CS_IDLE ;
	ADS8332_CONVST	= 1 ;
	
	ADS8332_RESET		= 1 ;
	delay_ms(1);
	ADS8332_RESET		= 0 ;//minimum  25ns
	delay_ms(1);
	ADS8332_RESET		= 1 ;
	delay_ms(1);	
}

/* ADS8332 写入配置信息 */
void ADS8332_Write_CFR(u16 CFR_Data)
{
	CFR_Data = (ADS8332_WRITE_CFR | (0x0fff&CFR_Data));
	
	ADS8332_CS	= CS_SELECT ;
	SPI2_Write_U16(CFR_Data);
	ADS8332_CS	= CS_IDLE ;
}

/* ADS8332 读取配置信息 */
u16 ADS8332_Read_CFR(void)
{
	u16 ConFR = 0x0000 ; 
	u16 Send_Data = ADS8332_READ_CFR ;
	
	ADS8332_CS	= CS_SELECT ;
	ConFR = SPI2_ReadWrite_U16(Send_Data);
	ADS8332_CS	= CS_IDLE ;
	
	return ConFR;
}

/* ADS8332 模数转换器开始转换 
	CONVST 信号下降开始采集数据 */
void ADS8332_Convst(void)
{
	ADS8332_CS			= CS_IDLE ;
	ADS8332_CONVST	= 1 ;
	delay_ms(1);
	ADS8332_CONVST	= 0 ;//minimum 40ns
	delay_ms(1);
	ADS8332_CONVST	= 1 ;
}

/* ADS8332 通道选择 */
void ADS8332_Channel_Sel(u16 Channel_x)
{
	ADS8332_CS = CS_SELECT ;
	SPI2_Write_U16(Channel_x);
	ADS8332_CS = CS_IDLE ;
}

/* ADS8332 读取转换数据 */
u16 ADS8332_Read_Data(u16 Channel_x)
{
	u8 	Status_EOC = 1 ;
	u16 ConFR = 0x0000 ;
	
	// channel select
	ADS8332_Channel_Sel(Channel_x);
	// start convert
	ADS8332_Convst();
	
	// check the EOC status
	Status_EOC = ADS8332_EOC_IN ;
	while(!Status_EOC)
	{
		Status_EOC = ADS8332_EOC_IN ;
	}
	// read the data from ADS8332
	ADS8332_CS = CS_SELECT ;
	ConFR = SPI2_Read_U16();
	ADS8332_CS = CS_IDLE ;
	
	return ConFR ;
}

/* ADS8332 异常处理 */
void ADS8332_ERROR_Handler(void)
{
	printf("ADS8332 error !!!\n");
}

/* ADS8332 测试代码
   在主函数调用   
	请在调用前先进行初始化 */
void ADS8332_Test(void)
{
	u16 ads8332_val;
	float Voltage_val;
	ads8332_val=ADS8332_Read_Data(ADS8332_Channel_0);
	Voltage_val=((float)ads8332_val*3.3f)/65536.0f;
	printf("\r\nvPower_test1=%6f  \r\n",Voltage_val);
}
