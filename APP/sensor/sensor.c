#include "sensor.h"

void Sensor_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(SENSOR_CS_CLK, ENABLE);

	GPIO_InitStructure.GPIO_Pin = SENSOR_CS_PORT;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG, &GPIO_InitStructure);//��ʼ��
	GPIO_SetBits(SENSOR_CS_GPIO,SENSOR_CS_PORT);
	
	SENSOR_CS = 1 ;
	SPI2_Init();
}