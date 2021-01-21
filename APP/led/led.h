#ifndef _led_H
#define _led_H

#include "system.h"

//*********************LED操作说明********************/
//led0--PA8
//led1--PD2

/*  LED时钟端口、引脚定义 */
#define LED0_PORT 			GPIOA   
#define LED0_PIN 			GPIO_Pin_8
#define LED0_PORT_RCC		RCC_APB2Periph_GPIOA

#define LED1_PORT 			GPIOD   
#define LED1_PIN 			GPIO_Pin_2
#define LED1_PORT_RCC		RCC_APB2Periph_GPIOD

#define	led1  PAout(8) 
#define	led2  PDout(2)

void LED_Init(void);

#endif
