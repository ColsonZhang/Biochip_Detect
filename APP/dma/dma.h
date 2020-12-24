#ifndef _dma_H
#define _dma_H

#include "system.h"
#include "usart.h"

void DMAx_Send_Init(DMA_Channel_TypeDef* DMAy_Channelx,u32 par,u32 mar,u16 ndtr);
void DMAx_Rx_Init(DMA_Channel_TypeDef* DMAy_Channelx,u32 par,u32 mar,u16 ndtr);
void DMAx_Enable(DMA_Channel_TypeDef *DMAy_Channelx,u16 ndtr);
void DMA_SEND_NVIC_Configuration(void);
void DMA_RX_NVIC_Configuration(void);
void DMA1_Channel4_IRQHandler(void);
void DMA1_Channel5_IRQHandler(void);

#endif
