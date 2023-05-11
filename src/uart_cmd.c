/**
  ******************************************************************************
  * @file    uart_cmd.c
  * @author  Torsten Jaekel
  * @version V1.1.0
  * @date    04-September-2016
  * @brief   The UART shell (TeraTerm) command interpreter
  ******************************************************************************
  */

#include <stdint.h>
#include <string.h>

#include "main.h"
#include "VCP_UART.h"

#include "AudioPlayer.h"

//for UART text transmission
static char text[50];

/** @addtogroup System
  * @{
  */

/** @defgroup Commands UART Shell commands
  * @{
  */

static void print_prompt(void)
{
	osDelay(10);
	UART_Tx((const uint8_t *)"> ", 2);
}

//! @brief the help text sent as one single string (better performance)
static char sHelpText[] = {
"\
Help:\r\n\
h                - display help (commands)\r\n\
L                - display CPU load\r\n\
+                - increase volume\r\n\
-                - decrease volume\r\n\
> "
};

/**
  * @brief  print the help text, send via UART
  * @param  none:
  * @retval None
  */
static void print_help(void)
{
	UART_Tx((const uint8_t *)sHelpText, strlen(sHelpText));
}

/**
  * @brief  print the CPU load in percentage, send via UART
  * @param  none:
  * @retval None
  */
static void print_cpuLoad(void)
{
	int l;

	//ATT: send all the time one single, complete string: our UART_Tx cannot handle
	//send requests if previous is still running!
	//Two UART_Tx will kill the ongoing transmission!

	//floating point sprintf not supported!
	////sprintf(text, "%3.1f\n\r", (float)((100.0 * (float)time_diff) / (float)time_total));
	if (time_total)
		l = (int)((100 * time_diff) / time_total);
	else
		l = 0;
	sprintf(text, "%3d\r\n> ", l);		//with prompt - avoid to kill UART_Tx!
	l = strlen(text);
	UART_Tx((const uint8_t *)text, l);
}

static void increase_volume(void)
{
	if (AUDIO_PLAYER_RunState())
		AUDIO_Volume(1);
	print_prompt();
}

static void decrease_volume(void)
{
	if (AUDIO_PLAYER_RunState())
		AUDIO_Volume(0);
	print_prompt();
}

static void show_volume(void)
{
	int vol = AUDIO_GetVolume();

	sprintf(text, "%3d\r\n> ", vol);		//with prompt - avoid to kill UART_Tx!
	vol = strlen(text);
	UART_Tx((const uint8_t *)text, vol);
}

/**
  * @fn void UART_cmd_init(void)
  * @brief  Initialize the UART once on startup
  * @param  none:
  * @retval None
  */
void UART_cmd_init(void)
{
	//init UART1
	UART_Init();

	//print welcome message, print prompt
	UART_Tx((const uint8_t *)"\r\nLyrebird APP\r\n> ", 18);
}

/**
  * @fn int UART_cmd(uint8_t *uartBuf, int len)
  * @brief  Decode the received UART string as a command line
  * @param  uartBuf: the pointer to the received UART string
  * @param  len: the length of the received string
  * @retval 0 for OK
  */
int UART_cmd(uint8_t *uartBuf, int len)
{
	(void)len;
	char cmd = '\0';
	int p1, p2, p3, p4, p5;

	//setup default values
	p1 = p2 = p3 = p4 = p5 = 0;

	sscanf((char *)uartBuf, "%c %i %i %i %i %i", &cmd, &p1, &p2, &p3, &p4, &p5);

	switch(cmd)
	{
		case 'h' : print_help();							//!< 'h' : help command
			   	   break;
		case 'L' : print_cpuLoad();							//!< 'L' : display CPU-Load
				   break;
		case '+' : increase_volume();
				   break;
		case '-' : decrease_volume();
				   break;
		case 'v' : show_volume();
				   break;
		case '\n': 											//!< NL/CR : empty line
		case '\r':
		case '\0': print_prompt();							//!< print prompt on empty line
				   break;
		default:   UART_Tx((const uint8_t *)"*E: unknown\r\n> ", 15);
	}

	//command prompt - help prints already prompt, otherwise we would be blocked here
	//ATT: an ongoing UART_Tx is killed by another UART_Tx!
	//if (cmd != 'h')
	//	UART_Tx((const uint8_t *)"> ", 2);

	return 0;			//no error
}

int UART_task(void)
{
//ATT: must be larger as largest message with echo, e.g. the weolcome message!
#define UART_RX_STRLEN	40
	//static task variables - they survive the call
	static uint8_t uartRxBuf[UART_RX_STRLEN];

	//local, temporary task variables
	int len;

#if 0
	//just for testing via remote echo
	{
		int ch;
		if ((ch = UART_RxChar()) != -1)
		{
			UART_Tx((uint8_t *)&ch, 1);
		}
	}
#endif

	if ((len = UART_getString(uartRxBuf, UART_RX_STRLEN)))
	{
		UART_cmd(uartRxBuf, len);
	}

	return 0;				//all OK, do not break the main loop
}

/**
  * @}
  */

/**
  * @}
  */

