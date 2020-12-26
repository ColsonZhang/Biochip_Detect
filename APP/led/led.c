#include "led.h"

/*******************************************************************************
* �� �� ��         : LED_Init
* ��������		   : LED��ʼ������
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void LED_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;//����ṹ�����
	
	RCC_APB2PeriphClockCmd(LED0_PORT_RCC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=LED0_PIN;  //ѡ����Ҫ���õ�IO��
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //�����������ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //���ô�������
	GPIO_Init(LED0_PORT,&GPIO_InitStructure); 	   /* ��ʼ��GPIO */
	
	GPIO_SetBits(LED0_PORT,LED0_PIN);   //��LED�˿����ߣ�Ϩ������LED
	
	RCC_APB2PeriphClockCmd(LED1_PORT_RCC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=LED1_PIN;  //ѡ����Ҫ���õ�IO��
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //�����������ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //���ô�������
	GPIO_Init(LED1_PORT,&GPIO_InitStructure); 	   /* ��ʼ��GPIO */
	
	GPIO_ResetBits(LED1_PORT,LED1_PIN);   //��LED�˿����ͣ�����
}



