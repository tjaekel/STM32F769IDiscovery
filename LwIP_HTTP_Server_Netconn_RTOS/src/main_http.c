/**
  ******************************************************************************
  * @file    main_http.h
  * @author  Torsten Jaekel
  * @version V1.1.0
  * @date    04-September-2016
  * @brief   The main function for the HTTP web server
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "lwip/netif.h"
#include "lwip/tcpip.h"
#include <lwip/sockets.h>
#include "cmsis_os.h"
#include "ethernetif.h"
#include "app_ethernet.h"
#include "lcd_log.h"
#include "httpserver-netconn.h"

#include "udp.h"
#include "ip4.h"
#include "raw.h"
#include "init.h"

#include "config.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
struct netif gnetif; 			/* network interface structure */
struct netif gnetifUDANTE; 		/* network interface structure */

/* Private function prototypes -----------------------------------------------*/
extern void SystemClock_Config(void);
extern void MPU_Config(void);
extern void CPU_CACHE_Enable(void);

extern int GNetworkMode;

void StartHTTPDThread(void const * argument);
osThreadId httpThreadID, dhcpThreadID;

#if 0
static void BSP_Config(void);
#endif
static void Netif_Config(void);

/* Private functions ---------------------------------------------------------*/

void http_taskCreate(void)
{
	/* Init thread */
	osThreadDef(Start, StartHTTPDThread, osPriorityAboveNormal, 0, configMINIMAL_STACK_SIZE);
	httpThreadID = osThreadCreate(osThread(Start), NULL);
}

#if 1
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main_http(void)
{
  /* Configure the MPU attributes as Write Through */
  MPU_Config();

  /* Enable the CPU Cache */
  CPU_CACHE_Enable();

  /* STM32F7xx HAL library initialization:
       - Configure the Flash ART accelerator on ITCM interface
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Global MSP (MCU Support Package) initialization
     */
  HAL_Init();  
  
  /* Configure the system clock to 200 MHz */
  SystemClock_Config(); 

  /* Init thread */
  http_taskCreate();
  
  /* Start scheduler */
  osKernelStart();
  
  /* We should never get here as control is now taken by the scheduler */
  for( ;; );

  return 0;
}
#endif

//PCBs for all the UDP ports we need (open some or all for all potential audio streams)
struct udp_pcb *pcb0;
#if 0
//we could open all potential UDP port, or just the one we found
struct udp_pcb *pcb1, *pcb2;
#endif

//buffer for one network audio packet, as 24bit format
static unsigned char aBuf[48 * 2 * 3];
int recCnt = 0;

void UDanteUDP_Receive(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
	(void)addr;
	(void)arg;
	(void)pcb;
	(void)port;

	unsigned char *pAudio;

	unsigned char *paBuf;
	u16_t len;

	//should we use a semaphore here? it is not reentrant, not thread-safe!
	if (recCnt)
	{
		//error!
		BSP_LED_On(LED1);
		while (1) ;
	}

	recCnt++;

	len = p->len;
	if (len == 48 * 2 * 3 + 1 + 8)
	{
		//audio packet starts at: p + 1 + 8 (if without ETH header)
		pAudio  = (unsigned char *)p->payload;
		pAudio += 1 + 8;
		paBuf = aBuf;
		//all samples are always 24bit, little endian - but we had to convert to big endian (PCM) here
		{
			int i;
			//flip the endian for the audio samples - network audio is big endian
			for (i = 0; i < (288); i += 3)
			{
				*(paBuf + 1) = *pAudio++;
				*(paBuf + 0) = *pAudio++;
				pAudio++;
				paBuf += 2;
			}
		}

		//48 * 2 * 2 for 16bit audio
		AUDIO_PLAYER_QueueBuf(aBuf, 192);
	}

	pbuf_free(p);
	recCnt--;
}

void main_UDanteUDP(void)
{
	err_t err;

	//udp_init();			//it is actually empty
	pcb0 = udp_new();
#if 0
	pcb1 = udp_new();
	pcb2 = udp_new();
#endif

#if 0
	err = udp_bind(pcb0, IP_ADDR_ANY, 14337);
	if (err == ERR_OK)
	{
	    udp_recv(pcb0, UDanteUDP_Receive, pcb0);
	}

#if 1
	err = udp_bind(pcb1, IP_ADDR_ANY, 14339);
	if (err == ERR_OK)
	{
	    udp_recv(pcb1, UDanteUDP_Receive, pcb1);
	}

	err = udp_bind(pcb2, IP_ADDR_ANY, 14341);
	if (err == ERR_OK)
	{
	    udp_recv(pcb2, UDanteUDP_Receive, pcb2);
	}
#endif
#else
	extern u16_t UDANTE_UDPPort;
	err = udp_bind(pcb0, IP_ADDR_ANY, UDANTE_UDPPort);
	if (err == ERR_OK)
	{
	    udp_recv(pcb0, UDanteUDP_Receive, pcb0);
	}
#endif
}

/**
  * @brief  Start Thread 
  * @param  argument not used
  * @retval None
  */
void StartHTTPDThread(void const * argument)
{
  (void)argument;
#if 0
  /* Initialize LCD and LEDs */
  BSP_Config();
#endif
  
  /* Create tcp_ip stack thread */
  if ( ! GNetworkMode)
	  tcpip_init(NULL, NULL);
  else
	  lwip_init();	/* if just network audio - call it w/o tcpip_init ! */

  /* Initialize the LwIP stack */
  Netif_Config();
  
  if ( ! GNetworkMode)
	  /* Initialize web server */
	  http_server_netconn_init();
  
  /* Notify user about the network interface config */
  User_notification(&gnetif);
  
  if (( ! GNetworkMode) && (SysConfig.netConfig & NET_DHCP))
  {
	  /* Start DHCPClient */
	  osThreadDef(DHCP, DHCP_thread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
	  dhcpThreadID = osThreadCreate(osThread(DHCP), &gnetif);
  }

  for( ;; )
  {
    /* Delete the Init Thread */ 
    osThreadTerminate(NULL);
  }
}

void Netif_AddUDANTE(ip_addr_t ipAddr)
{
  ip_addr_t ipaddr;
  ip_addr_t netmask;
  ip_addr_t gw;

  /* IP address setting */
  ip_addr_copy_from_ip4(ipaddr, ipAddr);
  IP4_ADDR(&netmask, NETMASK_ADDR0, NETMASK_ADDR1 , NETMASK_ADDR2, NETMASK_ADDR3);
  IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);

  /* - netif_add(struct netif *netif, ip_addr_t *ipaddr,
  ip_addr_t *netmask, ip_addr_t *gw,
  void *state, err_t (* init)(struct netif *netif),
  err_t (* input)(struct pbuf *p, struct netif *netif))

  Adds your network interface to the netif_list. Allocate a struct
  netif and pass a pointer to this structure as the first argument.
  Give pointers to cleared ip_addr structures when using DHCP,
  or fill them with sane numbers otherwise. The state pointer may be NULL.

  The init function pointer must point to a initialization function for
  your ethernet netif interface. The following code illustrates it's use.*/

  netif_add(&gnetifUDANTE, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, (netif_input_fn)&ip4_input);

  /*  Registers the default network interface. */
  netif_set_default(&gnetifUDANTE);

  if (netif_is_link_up(&gnetifUDANTE))
  {
    /* When the netif is fully configured this function must be called.*/
    netif_set_up(&gnetifUDANTE);
  }
  else
  {
    /* When the netif link is down this function must be called */
    netif_set_down(&gnetifUDANTE);
  }
}

/**
  * @brief  Initializes the lwIP stack
  * @param  None
  * @retval None
  */
static void Netif_Config(void)
{
  ip_addr_t ipaddr;
  ip_addr_t netmask;
  ip_addr_t gw;	
  
  /* IP address setting */
  IP4_ADDR(&ipaddr, IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
  IP4_ADDR(&netmask, NETMASK_ADDR0, NETMASK_ADDR1 , NETMASK_ADDR2, NETMASK_ADDR3);
  IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
  
  /* - netif_add(struct netif *netif, ip_addr_t *ipaddr,
  ip_addr_t *netmask, ip_addr_t *gw,
  void *state, err_t (* init)(struct netif *netif),
  err_t (* input)(struct pbuf *p, struct netif *netif))
  
  Adds your network interface to the netif_list. Allocate a struct
  netif and pass a pointer to this structure as the first argument.
  Give pointers to cleared ip_addr structures when using DHCP,
  or fill them with sane numbers otherwise. The state pointer may be NULL.
  
  The init function pointer must point to a initialization function for
  your ethernet netif interface. The following code illustrates it's use.*/
  
  if ( ! GNetworkMode)
	  netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &tcpip_input);
  else
	  netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, (netif_input_fn)&ip4_input);
  
  /*  Registers the default network interface. */
  netif_set_default(&gnetif);
  
  if (netif_is_link_up(&gnetif))
  {
    /* When the netif is fully configured this function must be called.*/
    netif_set_up(&gnetif);
  }
  else
  {
    /* When the netif link is down this function must be called */
    netif_set_down(&gnetif);
  }
}

#if 0
/**
  * @brief  Initializes the STM32756G-EVAL's LCD and LEDs resources.
  * @param  None
  * @retval None
  */
static void BSP_Config(void)
{
  /* Initialize the LCD */
  BSP_LCD_Init();
  
  /* Initialize the LCD Layers */
  BSP_LCD_LayerDefaultInit(1, LCD_FB_START_ADDRESS);
  
  /* Set LCD Foreground Layer  */
  BSP_LCD_SelectLayer(1);
  
  BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
  
  /* Initialize LCD Log module */
  LCD_LOG_Init();
  
  /* Show Header and Footer texts */
  LCD_LOG_SetHeader((uint8_t *)"Webserver Application Netconn API");
  LCD_LOG_SetFooter((uint8_t *)"STM32F769I-DISCO board");
  
  LCD_UsrLog((char *)"  State: Ethernet Initialization ...\n");
}
#endif

#if 0
/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 200000000
  *            HCLK(Hz)                       = 200000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 25
  *            PLL_N                          = 400
  *            PLL_P                          = 2
  *            PLL_Q                          = 9
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 7
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 400;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;

  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }

  /* Activate the OverDrive to reach the 200 MHz Frequency */
  ret = HAL_PWREx_EnableOverDrive();
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
}

/**
  * @brief  Configure the MPU attributes as Write Through for SRAM1/2.
  * @note   The Base Address is 0x20010000 since this memory interface is the AXI.
  *         The Region Size is 256KB, it is related to SRAM1 and SRAM2  memory size.
  * @param  None
  * @retval None
  */
static void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct;
  
  /* Disable the MPU */
  HAL_MPU_Disable();

  /* Configure the MPU attributes as WT for SRAM */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0x20010000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_256KB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

/**
  * @brief  CPU L1-Cache enable.
  * @param  None
  * @retval None
  */
static void CPU_CACHE_Enable(void)
{
  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();
}
#endif

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  
  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
