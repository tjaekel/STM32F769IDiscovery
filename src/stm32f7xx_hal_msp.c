/**
  ******************************************************************************
  * @file    stm32f7xx_hal_msp.c
  * @author  Torsten Jaekel
  * @version V1.1.0
  * @date    04-September-2016
  * @brief   The low level hardware initialization
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32F7xx_HAL_Examples
  * @{
  */

/** @defgroup SPDIFRX_Loopback
  * @brief HAL MSP module.
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
RCC_PeriphCLKInitTypeDef     		PeriphClkInitStruct;


extern DMA_HandleTypeDef			hdma_spdif_rx_dt;

extern SAI_HandleTypeDef 			spdiftxhsai;
extern DMA_HandleTypeDef            hSaiDma;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions
  * @{
  */

/**
  * @brief  SAI MSP Init.
  * @param  hsai : pointer to a SAI_HandleTypeDef structure that contains
  *                the configuration information for SAI module.
  * @retval None
  */
void HAL_SAI_MspInit(SAI_HandleTypeDef *hsai)
{
  GPIO_InitTypeDef  GPIO_Init;

  if (hsai == &spdiftxhsai)
  {
	  /* Enable SAI2 clock */
	  __HAL_RCC_SAI2_CLK_ENABLE();
  
	  /* Configure GPIOs used for SAI2 */
	  __HAL_RCC_GPIOD_CLK_ENABLE();
  
	  GPIO_Init.Mode      = GPIO_MODE_AF_PP;
	  GPIO_Init.Pull      = GPIO_NOPULL;
	  GPIO_Init.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
	  GPIO_Init.Alternate = GPIO_AF10_SAI2;
	  GPIO_Init.Pin       = GPIO_PIN_11;
	  HAL_GPIO_Init(GPIOD, &GPIO_Init);
  
	  /* Configure DMA used for SAI2 */
	  __HAL_RCC_DMA2_CLK_ENABLE();

	  if(hsai->Instance == SAI2_Block_A)
	  {
		  hSaiDma.Init.Channel             = DMA_CHANNEL_10;
		  hSaiDma.Init.Direction           = DMA_MEMORY_TO_PERIPH;
		  hSaiDma.Init.PeriphInc           = DMA_PINC_DISABLE;
		  hSaiDma.Init.MemInc              = DMA_MINC_ENABLE;
		  hSaiDma.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
		  hSaiDma.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
		  hSaiDma.Init.Mode                = DMA_CIRCULAR;
		  hSaiDma.Init.Priority            = DMA_PRIORITY_HIGH;
		  hSaiDma.Init.FIFOMode            = DMA_FIFOMODE_ENABLE;
		  hSaiDma.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
		  hSaiDma.Init.MemBurst            = DMA_MBURST_INC4;
		  hSaiDma.Init.PeriphBurst         = DMA_PBURST_INC4;

		  /* Select the DMA instance to be used for the transfer : DMA2_Stream4 */
		  hSaiDma.Instance                 = DMA2_Stream2;
  
		  /* Associate the DMA handle */
		  __HAL_LINKDMA(hsai, hdmatx, hSaiDma);

		  /* Deinitialize the Stream for new transfer */
		  HAL_DMA_DeInit(&hSaiDma);

		  /* Configure the DMA Stream */
		  if (HAL_OK != HAL_DMA_Init(&hSaiDma))
		  {
			  return;
		  }
	  }
	
	  HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 12, 0);
	  HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);
  
	  PeriphClkInitStruct.PLLSAI.PLLSAIN = 344;
	  PeriphClkInitStruct.PLLSAI.PLLSAIQ = 7;
	  PeriphClkInitStruct.PLLSAIDivQ = 1;
	  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SAI2;
	  PeriphClkInitStruct.Sai2ClockSelection = RCC_SAI2CLKSOURCE_PLLSAI;
	  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
	  __HAL_RCC_SAI2_CLK_ENABLE();

	  HAL_NVIC_SetPriority(SAI2_IRQn, 12, 0);
	  HAL_NVIC_EnableIRQ(SAI2_IRQn);
  }
}

/**
* @brief SPDIFRX MSP Init
* @param hspdif: SPDIFRX handle
* @retval None
*/

void HAL_SPDIFRX_MspInit(SPDIFRX_HandleTypeDef* hspdifrx)
{
	  GPIO_InitTypeDef GPIO_InitStruct;
	  if(hspdifrx->Instance == SPDIFRX)
	  {
	  /* USER CODE BEGIN SPDIFRX_MspInit 0 */

	  /* USER CODE END SPDIFRX_MspInit 0 */
	    /* Peripheral clock enable */
	    __HAL_RCC_SPDIFRX_CLK_ENABLE();

	    /**SPDIFRX GPIO Configuration
	    PG12     ------> SPDIFRX_IN1
	    */
	    GPIO_InitStruct.Pin = GPIO_PIN_12;
	    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	    GPIO_InitStruct.Pull = GPIO_NOPULL;
	    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	    GPIO_InitStruct.Alternate = GPIO_AF7_SPDIFRX;
	    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

	    /* Peripheral DMA init*/

	    hdma_spdif_rx_dt.Instance = DMA1_Stream1;
	    hdma_spdif_rx_dt.Init.Channel = DMA_CHANNEL_0;
	    hdma_spdif_rx_dt.Init.Direction = DMA_PERIPH_TO_MEMORY;
	    hdma_spdif_rx_dt.Init.PeriphInc = DMA_PINC_DISABLE;
	    hdma_spdif_rx_dt.Init.MemInc = DMA_MINC_ENABLE;
	    hdma_spdif_rx_dt.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	    hdma_spdif_rx_dt.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
	    hdma_spdif_rx_dt.Init.Mode = DMA_CIRCULAR;
	    hdma_spdif_rx_dt.Init.Priority = DMA_PRIORITY_MEDIUM;
	    hdma_spdif_rx_dt.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
	    hdma_spdif_rx_dt.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
	    hdma_spdif_rx_dt.Init.MemBurst = DMA_MBURST_SINGLE;
	    hdma_spdif_rx_dt.Init.PeriphBurst = DMA_PBURST_SINGLE;
	    if (HAL_DMA_Init(&hdma_spdif_rx_dt) != HAL_OK)
	    {
	      return;
	    }

	    __HAL_LINKDMA(hspdifrx,hdmaDrRx,hdma_spdif_rx_dt);

	  /* USER CODE BEGIN SPDIFRX_MspInit 1 */

	  /* USER CODE END SPDIFRX_MspInit 1 */
	  }
}

void HAL_SPDIFRX_MspDeInit(SPDIFRX_HandleTypeDef* hspdifrx)
{

  if(hspdifrx->Instance == SPDIFRX)
  {
  /* USER CODE BEGIN SPDIFRX_MspDeInit 0 */

  /* USER CODE END SPDIFRX_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_SPDIFRX_CLK_DISABLE();

    /**SPDIFRX GPIO Configuration
    PG12     ------> SPDIFRX_IN1
    */
    HAL_GPIO_DeInit(GPIOG, GPIO_PIN_12);

    /* Peripheral DMA DeInit*/
    HAL_DMA_DeInit(hspdifrx->hdmaDrRx);
  }
  /* USER CODE BEGIN SPDIFRX_MspDeInit 1 */

  /* USER CODE END SPDIFRX_MspDeInit 1 */

}

#ifdef USE_DMA2D
/**
  * @fn void HAL_DMA2D_MspInit(DMA2D_HandleTypeDef* hdma2d)
  * @brief  initialize the DMA2D for vector memcpy
  * @param  hdma2d: device handler for DMA2D
  * @retval None
  */
void HAL_DMA2D_MspInit(DMA2D_HandleTypeDef* hdma2d)
{
  if(hdma2d->Instance==DMA2D)
  {
  /* USER CODE BEGIN DMA2D_MspInit 0 */

  /* USER CODE END DMA2D_MspInit 0 */
    /* Peripheral clock enable */
    __DMA2D_CLK_ENABLE();
  /* USER CODE BEGIN DMA2D_MspInit 1 */

  /* USER CODE END DMA2D_MspInit 1 */
  }
}

/**
  * @fn void HAL_DMA2D_MspDeInit(DMA2D_HandleTypeDef* hdma2d)
  * @brief  de-initialize the DMA2D
  * @param  hdma2d: device handler for DMA2D
  * @retval None
  */
void HAL_DMA2D_MspDeInit(DMA2D_HandleTypeDef* hdma2d)
{
  if(hdma2d->Instance==DMA2D)
  {
  /* USER CODE BEGIN DMA2D_MspDeInit 0 */

  /* USER CODE END DMA2D_MspDeInit 0 */
    /* Peripheral clock disable */
    __DMA2D_CLK_DISABLE();
  }
  /* USER CODE BEGIN DMA2D_MspDeInit 1 */

  /* USER CODE END DMA2D_MspDeInit 1 */
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
