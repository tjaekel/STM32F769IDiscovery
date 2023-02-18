/**
  ******************************************************************************
  * @file    Mem2MemDMA.c
  * @author  Torsten Jaekel
  * @version V1.1.0
  * @date    04-September-2016
  * @brief   The use of DMA for memcpy done in background
  ******************************************************************************
  */

#include "Mem2MemDMA.h"
/*
 * DMA for Mem-to-Mem
 */

DMA_HandleTypeDef hMemDMA = {0};

HAL_StatusTypeDef DMA_Mem2Mem_Init(void)
{
	HAL_StatusTypeDef st;

	/* DMA controller clock enable */
	MEM_DMAx_CLK_ENABLE();

	/* Configure the hdma_saiTx handle parameters */
	hMemDMA.Init.Channel             = MEM_DMAx_CHANNEL;
	hMemDMA.Init.Direction           = DMA_MEMORY_TO_MEMORY;
	hMemDMA.Init.PeriphInc           = DMA_PINC_ENABLE;
	hMemDMA.Init.MemInc              = DMA_MINC_ENABLE;
	hMemDMA.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	hMemDMA.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
	hMemDMA.Init.Mode                = DMA_NORMAL;
	hMemDMA.Init.Priority            = DMA_PRIORITY_LOW;
	hMemDMA.Init.FIFOMode            = DMA_FIFO_THRESHOLD_FULL;
	hMemDMA.Init.MemBurst            = DMA_MBURST_SINGLE;
	hMemDMA.Init.PeriphBurst         = DMA_MBURST_SINGLE;
	hMemDMA.Instance 				 = MEM_DMAx_STREAM;

	MEM_DMAx_CLK_ENABLE();

	/* Deinitialize the Stream for new transfer */
	HAL_DMA_DeInit(&hMemDMA);

	/* Configure the DMA Stream */
	st = HAL_DMA_Init(&hMemDMA);

	return st;
}

/*
 * make sure to run cache-coherant !
 */
HAL_StatusTypeDef DMA_Mem2Mem_Start(uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataWLength)
{
	HAL_DMA_Abort(&hMemDMA);		//works only with this call !
	return HAL_DMA_Start(&hMemDMA, SrcAddress, DstAddress, DataWLength);
}

