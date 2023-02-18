/**
  ******************************************************************************
  * @file    VCP_UART.h
  * @author  Torsten Jaekel
  * @version V1.1.0
  * @date    04-September-2016
  * @brief   The interface for the VCP UART communication
  ******************************************************************************
  */

#ifndef VCP_UART_H_
#define VCP_UART_H_

/**
  ******************************************************************************
  * @file    BSP_DualMCU_uart.h
  * @author  Torsten Jaekel
  * @version V1.0.0
  * @date    09-March-2016
  * @brief   UART functions for command shell or debug
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 Torsten Jaekel</center></h2>
  *
  ******************************************************************************
  */

#include "stm32f7xx_hal.h"

/** @addtogroup BSP
  * @{
  */

/* Declarations and Definitions ----------------------------------------------*/

/**
 * @todo we do not check if buffer overflows, string too long etc.
 */

/** @def UART_BAUDRATE
 * the UART baudrate for the UART Shell
 */
#define UART_BAUDRATE		115200
/** @def UART_TX_BUF_SIZE
 * the size of transmit buffer - make sure large enough of longest single string message sent
 */
#define	UART_TX_BUF_SIZE	1024		/*!< make sure entire help text fits into! */
/** @def UART_RX_STR_SIZE
 * the length of the line (string) receiver buffer used by the UART Shell
 */
#define UART_RX_STR_SIZE	 80			/*!< max. string length for reception */

/* External References -------------------------------------------------------*/

extern UART_HandleTypeDef huart6;

/* Function Prototypes -------------------------------------------------------*/

//! UART1 init
extern void UART_Init(void);
//! send a byte (character) string via UART
extern void UART_Tx(const uint8_t *buf, int len);
//! get the received byte (charater) - non-blocking
extern int  UART_RxChar(void);
//! get a complete string (line) received via UART - nonblocking
extern int  UART_getString(uint8_t *buf, int len);
//! send a text string via UART  - like printf but without to have format specifiers
extern int  UART_putString(uint8_t *buf);

/**
  * @}
  */

#endif /* VCP_UART_H_ */
