#include "sensor.h"

void Sensor_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(SENSOR_CS_CLK, ENABLE);

	GPIO_InitStructure.GPIO_Pin = SENSOR_CS_PORT;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG, &GPIO_InitStructure);//≥ı ºªØ
	GPIO_SetBits(SENSOR_CS_GPIO,SENSOR_CS_PORT);
	
	SENSOR_CS = 1 ;
	SPI2_Init();
	
}

void Sensor_Read(void)
{
	u8 index = 0;
	u8 index2 = 0 ;
	index = SPI2_ReadWriteByte(0x90);
	index2 = SPI2_ReadWriteByte(0x90);
	printf("%o\t%o\t",index,index2);
	// to do !!!
	
}