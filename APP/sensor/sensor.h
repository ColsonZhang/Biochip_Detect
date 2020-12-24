#ifndef _SENSOR_H
#define _SENSOR_H

#include "system.h"
#include "spi.h"
#include "usart.h"

#define SENSOR_CS_CLK RCC_APB2Periph_GPIOB
#define SENSOR_CS_GPIO GPIOB
#define SENSOR_CS_PORT GPIO_Pin_6

#define	SENSOR_CS 		PBout(6)  		//SENSORµÄÆ¬Ñ¡ÐÅºÅ

void Sensor_Init(void);
void Sensor_Read(void);

#endif
