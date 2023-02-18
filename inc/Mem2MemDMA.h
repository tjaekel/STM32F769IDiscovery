/**
  ******************************************************************************
  * @file    Mem2MemDMA.h
  * @author  Torsten Jaekel
  * @version V1.1.0
  * @date    04-September-2016
  * @brief   The use of DMA for background memcpy
  ******************************************************************************
  */

#ifndef MEM2MEMDMA_H_
#define MEM2MEMDMA_H_

#include "main.h"

/* Mem-to-Mem DMA stream definition */
#define MEM_DMAx_CLK_ENABLE()         	 			__HAL_RCC_DMA2_CLK_ENABLE()
#define MEM_DMAx_STREAM               	 			DMA2_Stream3		//use the unused stream! */
#define MEM_DMAx_CHANNEL              	 			DMA_CHANNEL_0

HAL_StatusTypeDef DMA_Mem2Mem_Init(void);
HAL_StatusTypeDef DMA_Mem2Mem_Start(uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataWLength);

#endif /* MEM2MEMDMA_H_ */
