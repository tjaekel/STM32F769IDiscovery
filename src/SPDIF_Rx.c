/**
  ******************************************************************************
  * @file    SPDIF_Rx.c
  * @author  Torsten Jaekel
  * @version V1.1.0
  * @date    04-September-2016
  * @brief   The S/PDIF receiver on STM board
  ******************************************************************************
  */

#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* SPDIFRX init function */
void SPDIF_RX_Msp_Init(void)
{
  hspdif.Instance = SPDIFRX;
  hspdif.Init.InputSelection = SPDIFRX_INPUT_IN1;
  hspdif.Init.Retries = SPDIFRX_MAXRETRIES_NONE;
  hspdif.Init.WaitForActivity = SPDIFRX_WAITFORACTIVITY_OFF; //SPDIFRX_WAITFORACTIVITY_ON; //SPDIFRX_WAITFORACTIVITY_OFF;
  hspdif.Init.ChannelSelection = SPDIFRX_CHANNEL_A;
  hspdif.Init.DataFormat = SPDIFRX_DATAFORMAT_32BITS; 		//SPDIFRX_DATAFORMAT_LSB;
  hspdif.Init.StereoMode = SPDIFRX_STEREOMODE_ENABLE; 		//SPDIFRX_STEREOMODE_DISABLE;
  hspdif.Init.PreambleTypeMask = SPDIFRX_PREAMBLETYPEMASK_OFF;
  hspdif.Init.ChannelStatusMask = SPDIFRX_CHANNELSTATUS_OFF;
  hspdif.Init.ValidityBitMask = SPDIFRX_VALIDITYMASK_OFF;
  hspdif.Init.ParityErrorMask = SPDIFRX_PARITYERRORMASK_OFF;
  if (HAL_SPDIFRX_Init(&hspdif) != HAL_OK)
  {
    return;
  }
}

/**
  * Enable DMA controller clock
  */
void SPDIF_RX_DMA_Init(void)
{
  //needed for SPDIF Rx

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 9, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);
}

/** Pinout Configuration
*/
void SPDIF_RX_Init(void)
{
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOG_CLK_ENABLE();
}

/**
  * @brief  Start the SPDIF Rx reception
  * @param  None
  * @retval None
  */
int SPDIF_RX_Start(void)
{
	uint8_t *rxBuf;
	int rxBufLen;

	rxBufLen = AUDIO_PLAYER_GetBuffer(&rxBuf);

	HAL_SPDIFRX_ReceiveDataFlow_DMA(&hspdif, (uint32_t *)rxBuf, rxBufLen / sizeof(int32_t));

	//if(SpdifrxHandle.ErrorCode != HAL_SPDIFRX_ERROR_NONE)
	//{
	//    return 0;
	//}

	return 1;
}
