/**
  ******************************************************************************
  * @file    uart_cmd.h
  * @author  Torsten Jaekel
  * @version V1.1.0
  * @date    04-September-2016
  * @brief   The interface for the VCP UART command interpreter
  ******************************************************************************
  */

#ifndef UART_CMD_H_
#define UART_CMD_H_

extern void UART_cmd_init(void);
extern int UART_cmd(uint8_t *uartBuf, int len);
extern int UART_task(void);

#endif /* UART_CMD_H_ */
