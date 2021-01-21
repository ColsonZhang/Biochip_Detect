/********************************************************************************
* @FileName: sensor.c
* @Author: Zhang Shen
* @Version: 1.0
* @Date: 2021-1-21
* @Description: The functions about using the ADS8332
********************************************************************************/

#include "sensor.h"

/*********************************************************
* Function_Name : ADS8332_Init
* Parameter     : void
* Return        : void
* Description   : Initialize the ADS8332
* Author				: Zhang Shen
* Create_Time   : 2021-01-21
* Modify_Record : null
**********************************************************/
void ADS8332_Init(void)
{
	u16 cfr_config_write = 0x0000 ;
	u16 cfr_config_read	 = 0x0000 ;
	ADS8332_Init_Port();
	ADS8332_Reset();
	
	// Write the config into ADS8322
	cfr_config_write = (ADS8332_WRITE_CFR|ADS8332_INT_CLK|ADS8332_TR_MANUL|ADS8332_SMPL_250K|ADS8332_EOC_LOW\
										|ADS8332_P10_EOC|ADS8332_P10_OUT|ADS8332_AUTONAP_DIS|ADS8332_NAP_DIS|ADS8332_PD_DIS|ADS8332_NOSW_RST);
	ADS8332_Write_CFR(cfr_config_write);
	
	// Check whether the config is correct
	cfr_config_read = ADS8332_Read_CFR();
	if(cfr_config_read != cfr_config_write)
	{
		ADS8332_ERROR_Handler();
	}
}

/*********************************************************
* Function_Name : ADS8332_Init_Port
* Parameter     : void
* Return        : void
* Description   : Initialize the ports used by the ADS8332
* Author				: Zhang Shen
* Create_Time   : 2021-01-21
* Modify_Record : null
**********************************************************/
void ADS8332_Init_Port(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	// Enable the clock
	RCC_APB2PeriphClockCmd(ADS8332_CS_CLK|ADS8332_RESET_CLK|ADS8332_CONVST_CLK|ADS8332_EOC_CLK, ENABLE);
	
	// Initialize CS as Push-Pull-Output
	GPIO_InitStructure.GPIO_Pin = ADS8332_CS_PORT;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(ADS8332_CS_GPIO, &GPIO_InitStructure);
	
	// Initialize RESET as Push-Pull-Output
	GPIO_InitStructure.GPIO_Pin = ADS8332_RESET_PORT;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(ADS8332_RESET_GPIO, &GPIO_InitStructure);
	
	// Intialzie CONVST as Push-Pull-Output
	GPIO_InitStructure.GPIO_Pin = ADS8332_CONVST_PORT;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(ADS8332_CONVST_GPIO, &GPIO_InitStructure);	
	
	// Intialzie EOC as Pull-Up-Input
	GPIO_InitStructure.GPIO_Pin = ADS8332_EOC_PORT;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(ADS8332_EOC_GPIO, &GPIO_InitStructure);	

	// Intialzie SPI2
	SPI2_Init();
}

/*********************************************************
* Function_Name : ADS8332_Reset
* Parameter     : void
* Return        : void
* Description   : Reset the ADS8332
* Author				: Zhang Shen
* Create_Time   : 2021-01-21
* Modify_Record : null
**********************************************************/
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

/*********************************************************
* Function_Name : ADS8332_Write_CFR
* Parameter     :
		@CFR_Data			the config ready for uploading
* Return        : void
* Description   : Write the config into the ADS8332
* Author				: Zhang Shen
* Create_Time   : 2021-01-21
* Modify_Record : null
**********************************************************/
void ADS8332_Write_CFR(u16 CFR_Data)
{
	CFR_Data = (ADS8332_WRITE_CFR | (0x0fff&CFR_Data));
	
	ADS8332_CS	= CS_SELECT ;
	SPI2_Write_U16(CFR_Data);
	ADS8332_CS	= CS_IDLE ;
}

/*********************************************************
* Function_Name : ADS8332_Read_CFR
* Parameter     : void
* Return        :
		@ConFR 				the config read from the ADS8332
* Description   : Read the config from the ADS8332
* Author				: Zhang Shen
* Create_Time   : 2021-01-21
* Modify_Record : null
**********************************************************/
u16 ADS8332_Read_CFR(void)
{
	u16 ConFR = 0x0000 ; 
	u16 Send_Data = ADS8332_READ_CFR ;
	
	ADS8332_CS	= CS_SELECT ;
	ConFR = SPI2_ReadWrite_U16(Send_Data);
	ADS8332_CS	= CS_IDLE ;
	
	return ConFR;
}

/*********************************************************
* Function_Name : ADS8332_Convst
* Parameter     : void
* Return        : void
* Description   : Start the conversion by controling the 
									CONVST port.
* Author				: Zhang Shen
* Create_Time   : 2021-01-21
* Modify_Record : null
**********************************************************/
void ADS8332_Convst(void)
{
	ADS8332_CS			= CS_IDLE ;
	ADS8332_CONVST	= 1 ;
	delay_ms(1);
	ADS8332_CONVST	= 0 ;//minimum 40ns
	delay_ms(1);
	ADS8332_CONVST	= 1 ;
}

/*********************************************************
* Function_Name : ADS8332_Channel_Sel
* Parameter     :
		@Channel_x		the AD channel selected
* Return        : void
* Description   : Select the AD channel
* Author				: Zhang Shen
* Create_Time   : 2021-01-21
* Modify_Record : null
**********************************************************/
void ADS8332_Channel_Sel(u16 Channel_x)
{
	ADS8332_CS = CS_SELECT ;
	SPI2_Write_U16(Channel_x);
	ADS8332_CS = CS_IDLE ;
}

/*********************************************************
* Function_Name : ADS8332_Read_Data
* Parameter     :
		@Channel_x		the AD channel selected
* Return        :
		@ConFR				the data read from the ADS8332
* Description   : Read data from the selected AD channel
* Author				: Zhang Shen
* Create_Time   : 2021-01-21
* Modify_Record : null
**********************************************************/
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

/*********************************************************
* Function_Name : ADS8332_ERROR_Handler
* Parameter     : void
* Return        : void
* Description   : Handle the error condition
* Author				: Zhang Shen
* Create_Time   : 2021-01-21
* Modify_Record : null
**********************************************************/
void ADS8332_ERROR_Handler(void)
{
	printf("ADS8332 error !!!\n");
}

/*********************************************************
* Function_Name : ADS8332_Vol_to_Value
* Parameter     :
		@value				the value directly read from ADS8332
* Return        :
		@Voltage			the voltage transformed from the value
* Description   : Transform the value to voltage
* Author				: Zhang Shen
* Create_Time   : 2021-01-21
* Modify_Record : null
**********************************************************/
float ADS8332_Vol_to_Value(u16 value)
{
	float Voltage = 0 ;
	Voltage = (float)((float)(value) / ADS8332_RATIO);
	Voltage = (float)(Voltage * ADS8332_REF); 
	return Voltage;
}

/*********************************************************
* Function_Name : ADS8332_Test
* Parameter     : void
* Return        : void
* Description   : The test code for this driver;
									Please use the funciton in the main();
									BEFORE THE FUNC(),DO CALL THE INITILAI().
* Author				: Zhang Shen
* Create_Time   : 2021-01-21
* Modify_Record : null
**********************************************************/
void ADS8332_Test(void)
{
	u16 ads8332_val;
	float Voltage_val;
	ads8332_val=ADS8332_Read_Data(ADS8332_Channel_0);
	Voltage_val = ADS8332_Vol_to_Value(ads8332_val);
	//Voltage_val=((float)ads8332_val*3.3f)/65536.0f;
	printf("\r\nvPower_test1=%6f  \r\n",Voltage_val);
}
