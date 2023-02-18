/**
  ******************************************************************************
  * @file    VCP_UART.c
  * @author  Torsten Jaekel
  * @version V1.1.0
  * @date    04-September-2016
  * @brief   The VCP UART communication handler
  ******************************************************************************
  */

#include "VCP_UART.h"
#include "cmsis_os.h"
#include "semphr.h"
#include <string.h>			/* for memcpy() */

//! UART6 device handler
UART_HandleTypeDef huart1;

/** @addtogroup BSP
  * @{
  */

/****************************************************************************/
/**
  * @brief UART Transmit
  * @param huart: uart handle
  * @retval None
  */

/*
 * @details
 *
 * @attention
 * ATT 1: DTCM_RAM is not initialized!
 * @attention
 * ATT 2: we cannot use DMA for USART1 - all Streams are used for SAIx and SD Card1 - we had to use USART INTs
 *
 * Remark:
 * we could make it faster by these approaches:
 * - send just a '\n' and let terminal translate into "\n\r" (no need to send additional character in remote echo)
 * - let the terminal do the local echo, no need to echo here remotely (much faster, saves half the speed needed for echo)
 * - local echo in terminal gives us also option to let receive a complete string, e.g. DMA for 80 characters,
 *   just check via dmarx->Instance->NDTR register how much characters received and if there is a '\r' -> stop the DMA,
 *   so we do not need a character based reception, checking every received character, just max. length for DMA and check
 *   how much received to stop on line end
 *
 *   @todo implement a different flavor with '\r' conversion and local echo on terminal - better performance by receiving
 *         a complete string via DMA, not single-character based
 */

//buffers used by the DMAs
//! internal UART transmit buffer - make large enough for long single string help text!
static uint8_t uartTxBuf[UART_TX_BUF_SIZE]    /*__attribute__((section("DTCM_RAM")))*/;
//! internal UART receiver buffer - we use Circular with 2 bytes, so we can receive single characters (for remote echo)
static uint8_t uartRxBuf[ 2]   				  /*__attribute__((section("DTCM_RAM")))*/;	//circular buffer
//! internal UART string receiver buffer, for UART Shell commands
static uint8_t uartRxStrBuf[UART_RX_STR_SIZE] /*__attribute__((section("DTCM_RAM")))*/;

#ifdef UART_THREAD
extern SemaphoreHandle_t xSemaphoreUART;
#endif

/**
  * @fn void HAL_UART_MspInit(UART_HandleTypeDef* huart)
  * @brief  initialize the UART
  * @param  huart: pointer for device handler
  * @retval None
  */
void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(huart->Instance == USART1)
  {
	/* USER CODE BEGIN usart1_MspInit 0 */

	  /* GPIO Ports Clock Enable */
	__GPIOA_CLK_ENABLE();

	/* USER CODE END usart1_MspInit 0 */
    /* Peripheral clock enable */
    __USART1_CLK_ENABLE();

    /**usart1 GPIO Configuration
    PA9     ------> usart1_TX
    PA10    ------> usart1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Peripheral interrupt init */
    /* a lower value is higher priority! - do not go lower as FreeRTOS prio ! */
    HAL_NVIC_SetPriority(USART1_IRQn, 15, 15);
    HAL_NVIC_EnableIRQ(USART1_IRQn);

    /* USER CODE BEGIN usart1_MspInit 1 */

    /* USER CODE END usart1_MspInit 1 */
  }
}

/**
  * @fn void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
  * @brief  de-initialize the UART
  * @param  huart: pointer for device handler
  * @retval None
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
  if(huart->Instance == USART1)
  {
  /* USER CODE BEGIN usart1_MspDeInit 0 */

  /* USER CODE END usart1_MspDeInit 0 */
    /* Peripheral clock disable */
	__USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA1     ------> USART1_RX
    PA9     ------> USART1_TX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_10);
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9);

    /* Peripheral interrupt DeInit*/
    HAL_NVIC_DisableIRQ(USART1_IRQn);

  }
  /* USER CODE BEGIN usart1_MspDeInit 1 */

  /* USER CODE END usart1_MspDeInit 1 */
}

/** usart1 init function */
void UART_Init(void)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = UART_BAUDRATE;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONEBIT_SAMPLING_DISABLED;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  HAL_UART_Init(&huart1);

  HAL_UART_MspInit(&huart1);
}

/**
  * @brief  UART transmit
  * @param  buf: pointer to the byte buffer to send
  * @param  len: the number of bytes to send
  * @retval None
  */
void UART_Tx(const uint8_t *buf, int len)
{
	//polling mode
	//HAL_UART_Transmit(&huart1, buf, len, 1000);

	//INT mode - we cannot use DMA, all Streams occupied for SAI, SPI!
	//ATT: an ongoing UART_Tx is killed by another UART_Tx!
	//Make sure to finish a transmission before a new one is queued.
	//Or: modify to have a real FiFo or to block (but do not block audio loop!).

	if (len > UART_TX_BUF_SIZE)
		len = UART_TX_BUF_SIZE;

	//we need a local DMA buffer, the original source buffer can become invalid during DMA
	memcpy(uartTxBuf, buf, len);
	huart1.gState = HAL_UART_STATE_READY;
	HAL_UART_Transmit_IT(&huart1, uartTxBuf, len);
}

/**
  * @brief  UART receiver - nonblocking - it checks if UART DMA Rx has received anything
  * @param  none:
  * @retval it returns -1 for nothing available or the received character (byte)
  */
int UART_RxChar(void)
{
	static int startRx = 0;
	int ch;

	if ( ! startRx)
	{
		//INT mode
		HAL_UART_Receive_IT(&huart1, uartRxBuf, 1);
		startRx = 1;
	}
	else
	{
#ifdef UART_THREAD
		if (xSemaphoreTake(xSemaphoreUART, portMAX_DELAY) == pdTRUE)
#endif
		{
			if (huart1.gState == HAL_UART_STATE_READY)
			{
				ch = huart1.Instance->RDR;
				startRx = 0;
				return ch;
			}
		}
	}
#ifdef UART_THREAD
	taskYIELD();
#endif
	return -1;
}

/**
  * @brief  UART string receiver - it checks for a complete string (with newline, carriage return)
  * @param  buf: pointer to the buffer where to store the string
  * @param  len: the max. string length expected (max. size of provided buffer)
  * @retval it returns the length for the received string, 0 if no string (UART line) available)
  */
int UART_getString(uint8_t *buf, int len)
{
	static int idx = 0;
	static int ignoreNext = 0;
	int ch;
	int l;

	if ((ch = UART_RxChar()) != -1)
	{
		//following ESC sequence characters - ignore it
		if (ignoreNext)
		{
			ignoreNext--;
			return 0;
		}

		//handle backspace
		if (ch == 0x08)
		{
			if (idx > 0)
			{
				idx--;
				UART_Tx((uint8_t *)"\010 \010", 3);
			}
			else
				;

			return 0;
		}

		//ignore ESC sequences - do not echo, assume just few escape sequences here
		if (ch == 0x1B)
		{
			ignoreNext = 2;			//assuming just arrows up, down etc.
									//as: 1B [ A  (or 1B [ B etc.)
			return 0;
		}

		uartRxStrBuf[idx] = (uint8_t)ch;

		//local echo
		UART_Tx(&uartRxStrBuf[idx], 1);
		if (uartRxStrBuf[idx] == '\r')
			UART_Tx((uint8_t *)"\n", 1);
		if (uartRxStrBuf[idx] == '\n')
			UART_Tx((uint8_t *)"\r", 1);

		idx++;
		if (idx >= (len - 1))
		{
			memcpy(buf, uartRxStrBuf, len - 1);
			*(buf + len - 1) = 0;
			idx = 0;
			return len - 1;
		}

		if ((uartRxStrBuf[idx - 1] == '\r') || (uartRxStrBuf[idx - 1] == '\n'))
		{
			memcpy(buf, uartRxStrBuf, idx);
			*(buf + idx - 1) = '\r';
			*(buf + idx) = '\n';
			*(buf + idx + 1) = 0;
			l = idx + 1;
			idx = 0;
			return l;
		}
	}

	return 0;
}

/**
  * @brief  UART send string - send a NUL-termianted string via UART
  * @param  buf: pointer to the string, terminated with NUL
  * @retval it returns the length of the string sent
  */
int UART_putString(uint8_t *buf)
{
	int l;

	l = strlen((char *)buf);
	UART_Tx(buf, l);

	return l;
}

/*
 * **** DMA INT handlers ***********************************************************
 */

/**
  * @fn void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
  * @brief  the UART DMA handler
  * @param  huart: pointer to the UART device
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	//this is not used if not CIRCULAR, just TxHalfCpltCallback()
	UNUSED(huart);
}

/**
  * @fn void HAL_UART_TxHalfCpltCallback(UART_HandleTypeDef *huart)
  * @brief  the UART DMA handler
  * @param  huart: pointer to the UART device
  * @retval None
  */
void HAL_UART_TxHalfCpltCallback(UART_HandleTypeDef *huart)
{
	UNUSED(huart);
}

/**
  * @fn HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
  * @brief  the UART DMA handler
  * @param  huart: pointer to the UART device
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	UNUSED(huart);

#ifdef UART_THREAD
	static BaseType_t xHigherPriorityTaskWokenUART;

	xHigherPriorityTaskWokenUART = pdFALSE;
	xSemaphoreGiveFromISR(xSemaphoreUART, &xHigherPriorityTaskWokenUART);
    portYIELD_FROM_ISR(xHigherPriorityTaskWokenUART);
#endif
}

/**
  * @fn void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
  * @brief  the UART DMA handler
  * @param  huart: pointer to the UART device
  * @retval None
  */
void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
	UNUSED(huart);
}

/**
  * @fn void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
  * @brief  the UART DMA handler
  * @param  huart: pointer to the UART device
  * @retval None
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	UNUSED(huart);
}

/**
  * @}
  */
