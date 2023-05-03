/**
  ******************************************************************************
  * @file    stm32f7xx_it.c
  * @author  Torsten Jaekel
  * @version V1.1.0
  * @date    04-September-2016
  * @brief   The interrupt handlers for the system
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f7xx_it.h"
#include "ethernetif.h"

/** @addtogroup STM32F7xx_HAL_Examples
  * @{
  */

/** @addtogroup LCD_DSI_CmdMode_DoubleBuffering
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/ 
extern DSI_HandleTypeDef 	hdsi_discovery;
extern PCD_HandleTypeDef 	hpcd;
extern HCD_HandleTypeDef	hhcd_USB_OTG_HS;
extern SD_HandleTypeDef  	uSdHandle;
extern DMA_HandleTypeDef 	hdma_spdif_rx_dt;
#ifndef DIGI_FP
extern SAI_HandleTypeDef 	spdiftxhsai;
#else
extern SAI_HandleTypeDef    haudio_in_src_sai;
#endif
extern UART_HandleTypeDef huart1;

extern int gUSBInterface;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M7 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

volatile unsigned int stacked_r0;
volatile unsigned int stacked_r1;
volatile unsigned int stacked_r2;
volatile unsigned int stacked_r3;
volatile unsigned int stacked_r12;
volatile unsigned int stacked_lr;
volatile unsigned int stacked_pc;
volatile unsigned int stacked_psr;

/**
  * @brief  save parameter to figure out in debug mode where crash happened
  * @param  hardfault_args - the pointer to the stack context
  * @retval None
  */
void hard_fault_handler_c(unsigned int *hardfault_args)
{
	stacked_r0 	= ((unsigned long) hardfault_args[0]);
	stacked_r1 	= ((unsigned long) hardfault_args[1]);
	stacked_r2 	= ((unsigned long) hardfault_args[2]);
	stacked_r3 	= ((unsigned long) hardfault_args[3]);

	stacked_r12 = ((unsigned long) hardfault_args[4]);
	stacked_lr 	= ((unsigned long) hardfault_args[5]);
	stacked_pc 	= ((unsigned long) hardfault_args[6]);
	stacked_psr = ((unsigned long) hardfault_args[7]);

	//use the variable to have it available in debugger
	if (stacked_pc)
		BSP_LED_On(LED1);
	if (stacked_lr)
		BSP_LED_On(LED1);

	while(1) ;
}

#if 0
/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while(1) ;
}
#endif

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

#ifdef WITHOUT_RTOS
/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}
#endif

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

#ifdef WITHOUT_RTOS
/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}
#endif

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
#ifndef WITHOUT_RTOS
  osSystickHandler();
#endif
}

/******************************************************************************/
/*                 STM32F7xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f7xx.s).                                               */
/******************************************************************************/
/**
  * @brief  This function handles DSI Handler.
  * @param  None
  * @retval None
  */
void DSI_IRQHandler(void)
{
  HAL_DSI_IRQHandler(&hdsi_discovery);
}

/**
  * @brief  This function handles USB-On-The-Go FS/HS global interrupt request.
  * @param  None
  * @retval None
  */
#ifdef USE_USB_FS
void OTG_FS_IRQHandler(void)
#else
void OTG_HS_IRQHandler(void)
#endif
{
  if (gUSBInterface == 2)
  {
	  HAL_HCD_IRQHandler(&hhcd_USB_OTG_HS);
  }
  else
  {
	  HAL_PCD_IRQHandler(&hpcd);
  }
}

/**
  * @brief  This function handles SDIO interrupt request.
  * @param  None
  * @retval None
  */
void BSP_SDMMC_IRQHandler(void)
{
  HAL_SD_IRQHandler(&uSdHandle);
}

/*
 * ATT: SD Card DMA INT conflicts with DFSDMx Digital MIC !
 */
#if 0
/**
  * @brief  This function handles DMA2 Stream 5 interrupt request.
  * @param  None
  * @retval None
  */
void BSP_SDMMC_DMA_Tx_IRQHandler(void)
{
	HAL_DMA_IRQHandler(uSdHandle.hdmatx);
}

/**
  * @brief  This function handles DMA2 Stream 0 interrupt request.
  * @param  None
  * @retval None
  */
void BSP_SDMMC_DMA_Rx_IRQHandler(void)
{
	HAL_DMA_IRQHandler(uSdHandle.hdmarx);
}
#endif

//Q: which DMA is used for SAI1 out?

/**
  * @brief This function handles DMA2 Stream 1 interrupt request.
  * @param None
  * @retval None
  */
void AUDIO_OUT_SAIx_DMAx_IRQHandler(void)
{
  HAL_DMA_IRQHandler(haudio_out_sai.hdmatx);
}

/*
 * ATT: SD Card DMA INT conflicts with DFSDMx Digital MIC !
 */
/**
  * @brief This function handles DMA2 Stream 0 interrupt request.
  * @param None
  * @retval None
  */
void AUDIO_DFSDMx_DMAx_TOP_LEFT_IRQHandler(void)
{
	if (SysInterfaces.ifSelection == INIF_MIC)
		HAL_DMA_IRQHandler(hAudioInTopLeftFilter.hdmaReg);
	else
		HAL_DMA_IRQHandler(uSdHandle.hdmarx);
}

/**
  * @brief This function handles DMA2 Stream 5 interrupt request.
  * @param None
  * @retval None
  */
void AUDIO_DFSDMx_DMAx_TOP_RIGHT_IRQHandler(void)
{
	if (SysInterfaces.ifSelection == INIF_MIC)
		HAL_DMA_IRQHandler(hAudioInTopRightFilter.hdmaReg);
	else
		HAL_DMA_IRQHandler(uSdHandle.hdmatx);
}

/**
  * @brief  This function handles SPDIFRX interrupts.
  * @param  None
  * @retval None
  */
void SPDIF_RX_IRQHandler(void)
{
}

/**
* @brief This function handles DMA1 stream1 global interrupt.
*/
void DMA1_Stream1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream1_IRQn 0 */

  /* USER CODE END DMA1_Stream1_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_spdif_rx_dt);
  /* USER CODE BEGIN DMA1_Stream1_IRQn 1 */

  /* USER CODE END DMA1_Stream1_IRQn 1 */
}

#ifndef DIGI_FP
/**
  * @brief  This function handles SAI DMA interrupt request for SPDIF Tx.
  * @param  None
  * @retval None
  */
void DMA2_Stream2_IRQHandler(void)
{
  HAL_DMA_IRQHandler(spdiftxhsai.hdmatx);
}
#else
/**
  * @brief This function handles DMA2 Stream 2 interrupt request for SRC I2S input.
  * @param None
  * @retval None
  */
void AUDIO_IN_SAI_SRC_DMAx_IRQHandler(void)
{
  HAL_DMA_IRQHandler(haudio_in_src_sai.hdmarx);
}
#endif

/**
  * @brief  This function handles SAI DMA interrupt request.
  * @param  None
  * @retval None
  */
void DMA2_Stream4_IRQHandler(void)
{
  HAL_DMA_IRQHandler(haudio_in_sai.hdmarx);
}

/**
  * @brief  This function handles Ethernet interrupt request.
  * @param  None
  * @retval None
  */
void ETH_IRQHandler(void)
{
  ETHERNET_IRQHandler();
}

/**
* @brief This function handles USART1 global interrupt.
*/
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */

  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */

  /* USER CODE END USART1_IRQn 1 */
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
