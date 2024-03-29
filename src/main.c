/**
  ******************************************************************************
  * @file    main.c
  * @author  Torsten Jaekel
  * @version V1.1.0
  * @date    04-September-2016
  * @brief   This is the main entry function for the Lyrebird APP firmware
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "cmsis_os.h"
#include "FreeRTOSConfig.h"

#include "main.h"
#include "FFT.h"
#include "GUI_pages.h"

#include <string.h>
#include <stdio.h>

#include "ip_addr.h"

#include "Mem2MemDMA.h"

#include "SPKR_BassBoost.h"

#include "config.h"

/* version info to display */
const char VERSION_STR[7] = "V1.2.0";

extern uint8_t IP_ADDR[4];

volatile uint32_t time_begin, time_start, time_end, time_diff, time_total;
TIM_HandleTypeDef hTim2;

/** @addtogroup STM32F7xx_HAL_Examples
  * @{
  */

/** @addtogroup LCD_DSI_CmdMode_DoubleBuffering
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
DSI_VidCfgTypeDef 					hdsivideo_handle;
DSI_CmdCfgTypeDef					CmdCfg;
DSI_LPCmdTypeDef					LPCmd;
DSI_PLLInitTypeDef					dsiPllInit;
static RCC_PeriphCLKInitTypeDef		PeriphClkInitStruct;
DMA_HandleTypeDef					hSaiDma;

SPDIFRX_HandleTypeDef				hspdif;
DMA_HandleTypeDef					hdma_spdif_rx_dt;

TSYSInterfaces SysInterfaces = {
		0,
		0,
		AUDIO_FREQ_DEFAULT
};

/* Private define ------------------------------------------------------------*/

#define VSYNC               1 
#define VBP                 1 
#define VFP                 1
#define VACT                480
#define HSYNC               1
#define HBP                 1
#define HFP                 1
#define HACT                800

#define LAYER0_ADDRESS               	(LCD_FB_START_ADDRESS)

/* Private macro -------------------------------------------------------------*/
#define DEFAULT_THREAD_STACKSIZE        500
#define AUDIO_THREAD_PRIO    			(tskIDLE_PRIORITY) 	//osPriorityNormal
#define UARTCMD_THREAD_PRIO    			(tskIDLE_PRIORITY+1) 	//osPriorityNormal

/* Private variables ---------------------------------------------------------*/
static __IO int32_t  front_buffer   	= 0;
static __IO int32_t  pend_buffer   		= -1;

static const uint32_t Buffers[] = 
{
  LAYER0_ADDRESS,
  LAYER0_ADDRESS + (800*480*4),  
};

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MPU_Config(void);
void CPU_CACHE_Enable(void);

static void OnError_Handler(uint32_t condition);
#if 0
static void CopyBuffer(uint32_t *pSrc, 
                           uint32_t *pDst, 
                           uint16_t x, 
                           uint16_t y, 
                           uint16_t xsize, 
                           uint16_t ysize);
#endif
static uint8_t LCD_Init(void);
void LTDC_Init(void);
static int GetInterfaceSelection(int *outSelection, int *sampleRate);
static void ProcessConfig(void);
static void LCD_Config(void);

static void Audio_Thread(void const * argument);

void Display_FFTPage(void);

extern void http_taskCreate(void);

extern uint8_t sSDBuffer[AUDIO_TOTAL_BUF_SIZE];

#ifdef USE_DMA2D
static void MX_DMA2D_Init(void);
//! DMA2D device handler
DMA2D_HandleTypeDef hdma2d;
#endif

//! DMA Mem-to-Mem device handler

SemaphoreHandle_t xSemaphoreAudio;
xTaskHandle CreatedTaskAudio;

int GNetworkMode = 0;				//0 = web browser, all other ETH audio, no browser

static int processVolume = 0;

int gLineInPT = 0;

#ifdef UART_THREAD
static void UARTCmd_Thread(void const * argument);
SemaphoreHandle_t xSemaphoreUART;
xTaskHandle CreatedTaskUART;
#endif

#define SCRATCH_BUFF_SIZE (AUDIO_OUT_BUFFER_SIZE) 		/* (AUDIO_OUT_BUFFER_SIZE / sizeof(int16_t))	//XXXX seems to be the correct value! */
int32_t Scratch [SCRATCH_BUFF_SIZE];					/* for two MIC channels, each a 16bit */

int USBConfig = 0;

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  On Error Handler on condition TRUE.
  * @param  condition : Can be TRUE or FALSE
  * @retval None
  */
static void OnError_Handler(uint32_t condition)
{
  if(condition)
  {
    BSP_LED_On(LED1);
    while(1) { ; } /* Blocking on error */
  }
}

static void TIM_Start(void)
{
  /* for stop watch */
  __HAL_RCC_TIM2_CLK_ENABLE();
  __HAL_RCC_TIMCLKPRESCALER(RCC_TIMPRES_ACTIVATED);
  hTim2.Instance = TIM2;
  hTim2.Instance->ARR = 0xFFFFFFFF;
  __HAL_TIM_ENABLE(&hTim2);
}

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main_audio(void)
{
  /* Enable the MPU */
  MPU_Config();

  /* Enable the CPU Cache */
  CPU_CACHE_Enable();

  /* Configure the system clock to 216 or 200 MHz if with DDR RAM */
  SystemClock_Config();
  SystemCoreClockUpdate();

  /* STM32F7xx HAL library initialization:
       - Configure the Flash prefetch
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  HAL_Init();

  /* Initialize used Leds */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);

  /* Initialize the SDRAM - needed for DSI LCD */
  BSP_SDRAM_Init();

  /* read the system configuration from QSPI flash device */
  ReadSysConfig(&DefSysConfig);

  //initialize QSPI
  //BSP_QSPI_Init();

  //for clock debug:
  BSP_GPIO_Init();

  //Enable QSPI memory mapped mode
  BSP_QSPI_EnableMemoryMappedMode();

  /* Initialize the LCD */
  LCD_Config();

  //DMA2D for vectored memcpy
#ifdef USE_DMA2D
  MX_DMA2D_Init();
#endif

  /* Initialize Touch Screen controller */
  BSP_TS_Init(800, 480);

  /* Initialize Push Button PB */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);

#ifdef DIGI_FP
  /* Initialize I2C and SRC chip */
  SRC_I2C_Configure();
#endif

  /* Mem to Mem DMA */
  //works only on cache-coherent memory, DTCM! not if DDR is used with caches!
  DMA_Mem2Mem_Init();

  /* create semaphores for the threads - we have to do before we start scheduler! */			//Give must be called, otherwise semaphore is empty
  vSemaphoreCreateBinary(xSemaphoreAudio);
#ifdef UART_THREAD
  vSemaphoreCreateBinary(xSemaphoreUART);
#endif

#if 0
  /* Init threads - we have to create all tasks before scheduler launched! */
  xTaskCreate((TaskFunction_t)Audio_Thread, (const portCHAR *)"Audio", DEFAULT_THREAD_STACKSIZE, NULL, AUDIO_THREAD_PRIO, &CreatedTaskAudio);
#ifdef UART_THREAD
  UART_cmd_init();
  xTaskCreate((TaskFunction_t)UARTCmd_Thread, (const portCHAR *)"UARTCmd", DEFAULT_THREAD_STACKSIZE, NULL, UARTCMD_THREAD_PRIO, &CreatedTaskUART);
#endif

  /* Start scheduler */
  vTaskStartScheduler();
#else
#ifdef UART_THREAD
  UART_cmd_init();
  osThreadDef(UART, UARTCmd_Thread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 2);
  osThreadCreate(osThread(UART), NULL);
#endif

  /* ATT: prio must be lower as HTTPD prio! */
  osThreadDef(Audio, Audio_Thread, /*osPriorityBelowNormal*/ osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 2);
  osThreadCreate(osThread(Audio), NULL);

  /* Start scheduler */
  osKernelStart();
#endif

  /* We should never get here as control is now taken by the scheduler */
  for( ;; );
}

#if 0
uint8_t inBuffer[AUDIO_OUT_BUFFER_SIZE] __attribute__((aligned(4)));
#endif

/**
  * @brief  Audio thread for FreeRTOS OS
  * @param  argument - not used
  * @retval None
  */
static void __attribute__((section("ITCM_RAM"))) Audio_Thread(void const * argument)
{
  (void)argument;
  int ifSelection, outSelection;
  int sampleFreq = 48000;				//will be changed by config
  int SDGUIState = 0;
  int touchDelay = 0;
#define TOUCH_DELAY 5					//approx. 500ms for setting volume after return from GUI pages

  /* after Touch init - get the selection but before DSI_Refresh because it displays text */
  ifSelection = GetInterfaceSelection(&outSelection, &sampleFreq);

  /* set global SYS variable */
  SysInterfaces.ifSelection = ifSelection;
  SysInterfaces.outSelection = outSelection;
  SysInterfaces.sampleFreq = sampleFreq;

#if 1
  //bug: MIC runs just with 44.1 KHz
  if (ifSelection == INIF_MIC)
	  sampleFreq = 44100;
#endif

  if (ifSelection == INIF_LINE)
	  gLineInPT = 1;

  /*
   * make sure that web server task does not intercept the SD card procedures!
   */
  //if no network - we have a huge delay here and GUI seems to be slow
  //it does not work together with SD Card !!
  //and it needs a power cycle of the device !!

  /* start web server or uDANTE listener */
  if (ifSelection == INIF_UDANTE)
  {
	  GNetworkMode = 1;					//no web server, uDANTE listener
	  http_taskCreate();
  }
  else
  {
	  GNetworkMode = 0;					//with web server
	  if (ifSelection != INIF_SD)
	  {
		  if (SysConfig.netConfig & NET_WEB_SERVER)
			  http_taskCreate();
	  }
  }

  /* Refresh the LCD display*/
  HAL_DSI_Refresh(&hdsi_discovery);

  //if not PC USB Audio Out (here as audio input) - open as PC USB Audio In (to PC)
  /* USB MSC for SD Card as file system is exclusive with USB Audio from or to PC, also
   * exclusive with MIC !
   */
  if (USBConfig == 1)
  {
	  /* Audio to host, as sound card device */
	  USB_Interface(2);
  }
  else
  if (ifSelection != INIF_USB)
  {
	  /* no USB Audio, USB is for MSC to get SD Card on Host */
	  USB_Interface(1);
  }

  /* clear buffer to make sure not to have garbage */
  AUDIO_PLAYER_ClrBuf();

  /* Init Audio interface - important to do in this order */
  AUDIO_PLAYER_Init(outSelection, sampleFreq);

#ifndef DIGI_FP
#ifdef STM_SPDIF_OUT
  /* SPDIF Tx Out - always active */
  SPDIF_TX_Init();
#endif
#else
#endif

  /* Network Audio selection: 48KHz, 16bit */
  if (ifSelection == INIF_UDANTE)
  {
	  //similar to USB but nothing else to do here
  }

  /* select here between USB, SPDIF_Rx, MIC, ... as input */
  if (ifSelection == INIF_LINE)
  {
	  /* Line In Init */
	  int bufSize;
	  uint8_t *buffer;

	  /* it does not work right:
	   * there is nothing out, instead we get all the time the SAI Tx DMA Error, with FIFO_ERROR (FE) set,
	   * but the VU Meter and FFT works
	   */
      BSP_AUDIO_IN_InitEx(INPUT_DEVICE_ANALOG_MIC, sampleFreq, DEFAULT_AUDIO_IN_BIT_RESOLUTION, DEFAULT_AUDIO_IN_CHANNEL_NBR);
      bufSize = AUDIO_PLAYER_GetBuffer(&buffer);
#if 1
      BSP_AUDIO_IN_Record((uint16_t*)buffer, bufSize / (sizeof(uint16_t)));
#else
      BSP_AUDIO_IN_Record((uint16_t*)inBuffer, bufSize / (sizeof(uint16_t)));
#endif
  }

  if (ifSelection == INIF_MIC)
  {
	  //use onboard MICs
	  int bufSize;
	  uint8_t *buffer;

      BSP_AUDIO_IN_Init(sampleFreq, DEFAULT_AUDIO_IN_BIT_RESOLUTION, DEFAULT_AUDIO_IN_CHANNEL_NBR);
      BSP_AUDIO_IN_AllocScratch(Scratch, SCRATCH_BUFF_SIZE); //it is needed but what is the size of the buffer?
      bufSize = AUDIO_PLAYER_GetBuffer(&buffer);
      BSP_AUDIO_IN_Record((uint16_t*)buffer, bufSize / (sizeof(uint16_t)));
  }

  if (ifSelection == INIF_USB)
  {
	  /* USB Audio Init for audio from PC */
#ifndef TRY_USBH_MIC
	  USB_Interface(0);
#else
	  /* we hijack the USB (audio from PC) button on LCD in order
	   * to select the USB MIC (STM32 board as host)
	   * --> it works just a bit (USB Enumeration), but the audio is distorted,
	   * and we would need a very generic implementation in order to realize the
	   * audio format (44.1 vs. 48KHz, 16bit vs. 24bit and Mono/Stereo!
	   */
	  USB_Interface(3);
#endif
  }

  if (ifSelection == INIF_SPDIF_STM)
  {
	  /* Init SPDIF Rx on STM board */
	  SPDIF_RX_Init();
	  SPDIF_RX_DMA_Init();
	  SPDIF_RX_Msp_Init();

	  /* start SPDIF Rx */
	  SPDIF_RX_Start();
  }

#ifdef DIGI_FP
  if (ifSelection == INIF_SPDIF_D)
  {
	  /* SPDIF or ... input from SRC */
	  SRC_Rx_Init();
	  SRC_AudioSource(1);
	  SRC_Rx_Start();
  }

  if (ifSelection == INIF_TOS_D)
  {
	  /* SPDIF or ... input from SRC */
	  SRC_Rx_Init();
	  SRC_AudioSource(0);
	  SRC_Rx_Start();
  }

  if (ifSelection == INIF_AES3_D)
  {
	  /* SPDIF or ... input from SRC */
	  SRC_Rx_Init();
	  SRC_AudioSource(2);
	  SRC_Rx_Start();
  }

  if (ifSelection == INIF_BT_D)
  {
	  /* SPDIF or ... input from SRC */
	  SRC_Rx_Init();
	  SRC_AudioSource(3);
	  SRC_Rx_Start();
  }
#endif

  if (ifSelection == INIF_SD)
  {
	  /* SDCard WAV Player */
	  SDPlay_Init();

	  SDPlay_FileSelection(0);
  }

  if (ifSelection == INIF_GEN)		//debug: enable for LineIn test of output player
  {
	  TONEGEN_CpySine();
  }

  //now start the audio output functions
  AUDIO_PLAYER_Start();

#ifndef DIGI_FP
#ifdef STM_SPDIF_OUT
  /* SPDIF Tx Out - always active */
  SPDIF_TX_Start();
#endif
#endif

  Display_FFTPage();

  /* init FFT */
  FFT_Init();

  /* TIM for CPU load measurement */
  TIM_Start();

  /* Infinite loop */
  while (1)
  {
	TIM_MEASURE_BEGIN;

    if (AUDIO_PLAYER_GetClock())
    {
    	TIM_MEASURE_START; 	// start stop watch

#if 0
    	//debug - if we have Out when LineIn is selected - yes
    	TONEGEN_CpySine();
#endif

#if 0
    	if (ifSelection == INIF_LINE)
    		memcpy(BufferCtl.buff, inBuffer, sizeof(inBuffer));
#endif

    	//check if we have audio in received - if not - clear the buffers
    	AUDIO_PLAYER_GetHeartbeat();

#ifndef DIGI_FP
#ifdef STM_SPDIF_OUT
    	//SPDIF Tx on STM to output
    	//taskYIELD();	/* give higher thread a chance */
    	SPDIF_TX_OutBuf();
#endif
#endif

    	if (ifSelection == INIF_SD)
    	{
    		AUDIO_PLAYER_CpyBuf(sSDBuffer);

    		if (SysConfig.activeFilter & FILT_BASS_BOOST)
    			//process the output buffer as fast as possible
    			//ATT: works only with SD-Card !!!
    			BASS_BOOST_filter();
    	}

    	if (ifSelection == INIF_UDANTE)
    	{
    		//if with uDANTE network audio
    		extern ip_addr_t UDANTE_IPAddr;
    		extern u16_t UDANTE_UDPPort;
    		extern void Netif_AddUDANTE(ip_addr_t ipAddr);
    		extern void main_UDanteUDP(void);

    		static int done = 0;
    		if ((UDANTE_IPAddr.addr != 0) && ( ! done))
    		{
    			Netif_AddUDANTE(UDANTE_IPAddr);
    			main_UDanteUDP();
    			done = 1;

    			{
    				//print where found DANTE audio stream
    				uint8_t iptxt[35];
    				sprintf((char*)iptxt, "ETH audio: %ld.%ld.%ld.%ld | %d", UDANTE_IPAddr.addr & 0xFF, UDANTE_IPAddr.addr >> 8 & 0xFF, UDANTE_IPAddr.addr >> 16 & 0xFF, UDANTE_IPAddr.addr >> 24 & 0xFF,
    						UDANTE_UDPPort);

    				BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    				//taskYIELD();	/* give higher thread a chance */
    				BSP_LCD_DisplayStringAt(370, 100, (uint8_t *)iptxt, LEFT_MODE);
    			}
    		}
    	}
    	else
    	{
    		if (IP_ADDR[0] != 0)
    		{
				uint8_t iptxt[22];
				sprintf((char*)iptxt, "HTTP: %d.%d.%d.%d", IP_ADDR[0], IP_ADDR[1], IP_ADDR[2], IP_ADDR[3]);

				BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
				//taskYIELD();	/* give higher thread a chance */
				BSP_LCD_DisplayStringAt(370, 100, (uint8_t *)iptxt, LEFT_MODE);
    		}
    	}

    	if (ifSelection == INIF_SD)
    	{
    		/*
    		 * state machine to handle SD Player GUI if audio is running
    		 */
    		int button;

    		if ( ! SDPlay_Run())
    		{
    			//not playing or end of file, played - open SD Player again
    			//and start a new file to play (these steps are needed)
    			//we come to here if we stop the played file
    			button = SDPlay_FileSelection(0);				//blocking - return if play selected
    			if (button == SD_B_PLAY)
    			{
    				//taskYIELD();	/* give higher thread a chance */
    				Display_FFTPage();
    				AUDIO_PLAYER_Start();
    				SDGUIState = 0;
    				touchDelay = TOUCH_DELAY;
    			}
    			else
    				SDGUIState = 1;
    		}

    		//check if the user button is pressed
    		if (SDGUIState == 0)
    		{
    			//never do again if already open
    			if (BSP_PB_GetState(BUTTON_USER))
    			{
    				SDGUIState = 1;
    				/* clear display and show GUI page - display all again, but non-blocking*/
    				//taskYIELD();	/* give higher thread a chance */
    				SDPlay_FileSelection(1);				//non-blocking
    			}
    		}

    		if (SDGUIState == 1)
    		{
    			button = SDPlay_FileSelectionNB();			//non-blocking
    			if (button == SD_B_PLAY)
    			{
    				//a new or the same file to play
    				SDGUIState = 0;
    				//taskYIELD();	/* give higher thread a chance */
        			Display_FFTPage();
        			AUDIO_PLAYER_Start();
        			touchDelay = TOUCH_DELAY;
    			}
    			if (button == SD_B_EXIT)
    			{
    				//closed without to change anything
    				SDGUIState = 0;
    				//taskYIELD();	/* give higher thread a chance */
    				Display_FFTPage();
    				touchDelay = TOUCH_DELAY;
    			}
    		}
    	}

    	if ((SDGUIState == 0) && (touchDelay == 0))
    	{
    		//if we touch Peak Meter - show volume control
    		TS_StateTypeDef tsState;

    		BSP_TS_GetState(&tsState);
    		if (tsState.touchDetected)
    		{
    			//ATT: make sure to avoid conflict with Exit from SD Card menu (full volume if still pressed)
    			if ((tsState.touchY[0] < FFT_Y_BORDER_TOP) && (tsState.touchX[0] < 760))
    			{
    				processVolume = 1;
    				taskYIELD();	/* give higher thread a chance */
    				VUMETER_ProcessVolume(tsState.touchX[0]);
    			}
    			else
    				processVolume = 0;
    		}
    		else
    			processVolume = 0;
    	}

#ifdef LCD_UPDATE
    	if (SDGUIState == 0)
    	{
    		if ( ! processVolume)
    		{
    			//in combination with SD Player GUI - just if SD Player GUI not visible
    			VUMETER_Analyse();
    			taskYIELD();	/* give higher thread a chance */
    			VUMETER_Input(0);
    			taskYIELD();	/* give higher thread a chance */
    			VUMETER_Input(1);
    			taskYIELD();	/* give higher thread a chance */
    			VUMETER_Display(0);
    			taskYIELD();	/* give higher thread a chance */
    			VUMETER_Display(1);
    			taskYIELD();	/* give higher thread a chance */
    		}

#ifdef LCD_FFT
    		FFT_Filter(0);
    		taskYIELD();	/* give higher thread a chance */
    		FFT_DisplayGraph(0);
    		taskYIELD();	/* give higher thread a chance */
#if 0
    		//with both channels to display
    		FFT_Filter(1);
    		//taskYIELD();	/* give higher thread a chance */
    		FFT_DisplayGraph(1);
    		//taskYIELD();	/* give higher thread a chance */
#endif
#endif
    	}

    	if (SDGUIState == 0)
    	{
    		//in combination with SD Player GUI - just if SD Player GUI not visible
    		//display CPU load in percentage
    	    static float sMaxLoad = 0.0f;
    	    int sMaxLoadInt;
    	    float load;
    	    char text[15];

    	    if (time_total)
    	    	load = (float)((100.0 * (float)time_diff) / (float)time_total);
    	    else
    	    	load = 100.0;
    	    if (sMaxLoad < load)
    	    	sMaxLoad = load;
    	    sMaxLoadInt = (int)(sMaxLoad * 10.0);
    	    sprintf(text, "%3d.%1d%%", sMaxLoadInt / 10, sMaxLoadInt % 10);			//keep text shortest as possible
    		//BSP_LCD_SetFont(&Font16);					//use smaller font here
    	    if (sMaxLoad > 30.0f)
    	    {
    	    	BSP_LCD_SetTextColor(LCD_COLOR_LIGHTRED);
    	    }
    	    else
    	    {
    	    	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    	    }
    	    taskYIELD();	/* give higher thread a chance */
    		BSP_LCD_DisplayStringAt(715, FFT_Y_BORDER_TOP + 130, (uint8_t *)text, LEFT_MODE);
    		if (sMaxLoad > 0.05f)
    			sMaxLoad -= 0.05f;

#if 0
    		//display sample rate
    		sprintf(text, "%5d", SysInterfaces.sampleFreq);
    		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    		//taskYIELD();	/* give higher thread a chance */
    		BSP_LCD_DisplayStringAt(726, FFT_Y_BORDER_TOP + 170, (uint8_t *)text, LEFT_MODE);
#endif

    		/*Refresh the LCD display*/
    		HAL_DSI_Refresh(&hdsi_discovery);
    	}
#endif

    	if (touchDelay)
    		touchDelay--;

		TIM_MEASURE_END; 						//stop stop watch for audio processing
		TIM_MEASURE_TOTAL;						//total time for audio sample loops

		////taskYIELD();
    }
    taskYIELD();
  }
}
/**
  * @brief  Configure LDC
  * @param  None
  * @retval None
  */
void LCD_Config(void)
{
  uint8_t  lcd_status = LCD_OK;

  /* LCD Initialization Layer 0 */
  lcd_status = LCD_Init();
  OnError_Handler(lcd_status != LCD_OK);

  BSP_LCD_LayerDefaultInit(0, LAYER0_ADDRESS);
  BSP_LCD_SelectLayer(0);

  /* Enable the LCD */
  BSP_LCD_DisplayOn();
}

/**
  * @brief  Select input, output interfaces and sample rate
  * @param  outSelection - pointer to the enumeration for the output
  * @param  sampleRate - pointer to the sampleRate
  * @retval enumeration for the input interface
  */
int GetInterfaceSelection(int *outSelection, int *sampleRate)
{
	TS_StateTypeDef tsState;
	int ret = -1;

	*outSelection = 1;					//default, in case we do not select
	*sampleRate = I2S_AUDIOFREQ_48K;

	/* set the background layer */
	//BSP_LCD_SelectLayer(0);
	//BSP_LCD_SelectLayer(1);
	BSP_LCD_DrawBitmap(0, 0, (uint8_t *)PAGE_SPLASH);

    /*Refresh the LCD display*/
    HAL_DSI_Refresh(&hdsi_discovery);

	//HAL_Delay(100);	/* let us see the splash screen */

	//with BMP as background
	while (1)
	{
		BSP_TS_GetState(&tsState);
		if (tsState.touchDetected)
		{
			//config activated?
			if ((tsState.touchY[0] > 0) && (tsState.touchY[0] < 100))
			{
				if ((tsState.touchX[0] > 0) && (tsState.touchX[0] < 100))
				{
					ProcessConfig();
					continue;
				}
			}

			if ((tsState.touchY[0] > 44) && (tsState.touchY[0] < 144))
			{
				//Sample Rate
				if ((tsState.touchX[0] > 240) && (tsState.touchX[0] < 340))
				{
					*sampleRate = I2S_AUDIOFREQ_48K;
				}
				if ((tsState.touchX[0] > 350) && (tsState.touchX[0] < 450))
				{
					*sampleRate = I2S_AUDIOFREQ_44K;
				}
			}

			if ((tsState.touchY[0] > 200) && (tsState.touchY[0] < 300))
			{
				//HP or SPKR output
				if ((tsState.touchX[0] > 240) && (tsState.touchX[0] < 340))
				{
					*outSelection = 0;
				}
				if ((tsState.touchX[0] > 350) && (tsState.touchX[0] < 450))
				{
					*outSelection = 1;
				}
			}

			if ((tsState.touchY[0] > 44) && (tsState.touchY[0] < 144))
			{
				if ((tsState.touchX[0] > 460) && (tsState.touchX[0] < 780))
				{
					//Network Audio selection
					*sampleRate = I2S_AUDIOFREQ_48K;
					ret = INIF_UDANTE;
					break;
				}
			}

			if ((tsState.touchY[0] > 200) && (tsState.touchY[0] < 300))
			{
				if ((tsState.touchX[0] > 460) && (tsState.touchX[0] < 560))
				{
					//Sine Wave Generator
					ret = INIF_GEN;
					break;
				}
				if ((tsState.touchX[0] > 570) && (tsState.touchX[0] < 670) && (USBConfig == 0))
				{
					//SDCard - exclusive with USB MSC for SD Card
					ret = INIF_SD;
					break;
				}
#ifdef DIGI_FP
				if ((tsState.touchX[0] > 680) && (tsState.touchX[0] < 780))
				{
					//BT
					ret = INIF_BT_D;
					break;
				}
#endif
			}

			if ((tsState.touchY[0] > 355) && (tsState.touchY[0] < 455))
			{
				//all other input interfaces
				if ((tsState.touchX[0] >  20) && (tsState.touchX[0] < 120))
				{
					ret = INIF_SPDIF_STM;
					break;
				}
				if ((tsState.touchX[0] > 130) && (tsState.touchX[0] < 230) && (USBConfig == 0))
				{
					//exclusive with USB MSC for SD Card
					ret = INIF_MIC;
					break;
				}
				if ((tsState.touchX[0] > 240) && (tsState.touchX[0] < 340) && (USBConfig == 0))
				{
					//exclusive with USB MSC for SD Card
					ret = INIF_USB;
					break;
				}
				if ((tsState.touchX[0] > 350) && (tsState.touchX[0] < 450))
				{
					ret = INIF_LINE;
					break;
				}
#ifdef DIGI_FP
				if ((tsState.touchX[0] > 460) && (tsState.touchX[0] < 560))
				{
					ret = INIF_SPDIF_D;
					break;
				}
				if ((tsState.touchX[0] > 570) && (tsState.touchX[0] < 670))
				{
					ret = INIF_TOS_D;
					break;
				}
				if ((tsState.touchX[0] > 680) && (tsState.touchX[0] < 780))
				{
					ret = INIF_AES3_D;
					break;
				}
#endif
			}
		}
		else
			taskYIELD();
	}

#if 0
	/* Copy Buffer 0 into buffer 1, so only image area to be redrawn later */
	CopyBuffer((uint32_t *)Buffers[0], (uint32_t *)Buffers[1], 0, 0, 800, 480);
#endif

	//XXXX: actually just needed if network will be used

	//display temporary message - we are waiting for network
	BSP_LCD_SetFont(&Font16);
	//BSP_LCD_SetBackColor(LCD_COLOR_TRANSPARENT);
	BSP_LCD_SetBackColor(LCD_COLOR_BLACK);

	//clear all
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_FillRect(0, 0, 800, 480);

	//display left and right
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_DisplayStringAt(  300,  220, (uint8_t *)"waiting for network ...", LEFT_MODE);

	return ret;
}

void ProcessConfig(void)
{
	TS_StateTypeDef tsState;

	/* set the background layer */
	//BSP_LCD_SelectLayer(0);
	//BSP_LCD_SelectLayer(1);
	BSP_LCD_DrawBitmap(0, 0, (uint8_t *)PAGE_CONFIG);

	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_SetBackColor(LCD_COLOR_GREEN);

	//display version info in header
	BSP_LCD_DisplayStringAt(348, 7, (uint8_t *)VERSION_STR, LEFT_MODE);

	/* display current config - via X in check box */
	BSP_LCD_SetTextColor(LCD_COLOR_RED);
	BSP_LCD_SetBackColor(LCD_COLOR_YELLOW);

	//draw current config

	//auto config
	BSP_LCD_DisplayStringAt(426, 51, (uint8_t *)"X", LEFT_MODE);
	/** TODO auto config not yet used */

	//FFT display type
	if (SysConfig.fftType == FFT_DISPL_DOTS)
	{
		BSP_LCD_DisplayStringAt(25, 148, (uint8_t *)"X", LEFT_MODE);
		//next should be already done on reading config, do it again does not harm
		FFT_SetBarType(1);
	}
	if (SysConfig.fftType == FFT_DISPL_BARS)
	{
		BSP_LCD_DisplayStringAt(25, 198, (uint8_t *)"X", LEFT_MODE);
		FFT_SetBarType(0);
	}
	if (SysConfig.fftType == FFT_DISPL_LINE)
	{
		BSP_LCD_DisplayStringAt(25, 245, (uint8_t *)"X", LEFT_MODE);
		FFT_SetBarType(2);
	}

	//USB for audio (#1)
	BSP_LCD_DisplayStringAt(25, 346, (uint8_t *)"X", LEFT_MODE);

	//DCHP or static IP
	if (SysConfig.netConfig & NET_DHCP)
	{
		BSP_LCD_DisplayStringAt(425, 148, (uint8_t *)"X", LEFT_MODE);
	}
	else
	{
		BSP_LCD_DisplayStringAt(425, 198, (uint8_t *)"X", LEFT_MODE);
	}

	if (SysConfig.netConfig & NET_WEB_SERVER)
	{
		BSP_LCD_DisplayStringAt(425, 245, (uint8_t *)"X", LEFT_MODE);
	}
	else
	{
		//no web server
		BSP_LCD_DisplayStringAt(635, 245, (uint8_t *)"X", LEFT_MODE);
	}

	if (SysConfig.activeFilter & FILT_BASS_BOOST)
	{
		BSP_LCD_DisplayStringAt(425, 346, (uint8_t *)"X", LEFT_MODE);
	}
	else
	{
		//no bass boost
		BSP_LCD_DisplayStringAt(635, 346, (uint8_t *)"X", LEFT_MODE);
	}

    /*Refresh the LCD display*/
    HAL_DSI_Refresh(&hdsi_discovery);

	//HAL_Delay(100);	/* let us see the splash screen */

	//with BMP as background
	while (1)
	{
		BSP_TS_GetState(&tsState);
		if (tsState.touchDetected)
		{
			//exit activated? - no save of config
			if (tsState.touchY[0] < 50)
			{
				if (tsState.touchX[0] > 700)
				{
					//exit - go back to splash screen
					/* set the background layer */
					//BSP_LCD_SelectLayer(0);
					//BSP_LCD_SelectLayer(1);
					BSP_LCD_DrawBitmap(0, 0, (uint8_t *)PAGE_SPLASH);

				    /*Refresh the LCD display*/
				    HAL_DSI_Refresh(&hdsi_discovery);
					break;
				}
			}
			if (tsState.touchY[0] > 435)
			{
				//save - go back to splash screen
				/*
				 * save current config
				 */
				WriteSysConfig();

				/* set the background layer */
				//BSP_LCD_SelectLayer(0);
				//BSP_LCD_SelectLayer(1);
				BSP_LCD_DrawBitmap(0, 0, (uint8_t *)PAGE_SPLASH);

			    /*Refresh the LCD display*/
			    HAL_DSI_Refresh(&hdsi_discovery);
				break;
			}

			//auto config (top)
			if ((tsState.touchY[0] > 37) && (tsState.touchY[0] < 90))
			{
				//left
				if ((tsState.touchX[0] > 15) && (tsState.touchX[0] < 300))
				{
					BSP_LCD_DisplayStringAt(26,  51, (uint8_t *)"X", LEFT_MODE);
					BSP_LCD_DisplayStringAt(426, 51, (uint8_t *)" ", LEFT_MODE);
					/** TODO not yet used auto config */
				}
				//right
				if ((tsState.touchX[0] > 410) && (tsState.touchX[0] < 600))
				{
					BSP_LCD_DisplayStringAt(26,  51, (uint8_t *)" ", LEFT_MODE);
					BSP_LCD_DisplayStringAt(426, 51, (uint8_t *)"X", LEFT_MODE);
					/** TODO not yet used auto config */
				}
			}

			//FFT Display or Network - #1 row
			if ((tsState.touchY[0] > 142) && (tsState.touchY[0] < 174))
			{
				//left
				if ((tsState.touchX[0] > 15) && (tsState.touchX[0] < 300))
				{
					//FFT points
					BSP_LCD_DisplayStringAt(25, 148, (uint8_t *)"X", LEFT_MODE);
					BSP_LCD_DisplayStringAt(25, 198, (uint8_t *)" ", LEFT_MODE);
					BSP_LCD_DisplayStringAt(25, 245, (uint8_t *)" ", LEFT_MODE);

					SysConfig.fftType = FFT_DISPL_DOTS;
					FFT_SetBarType(1);
				}
				//right
				if ((tsState.touchX[0] > 410) && (tsState.touchX[0] < 600))
				{
					BSP_LCD_DisplayStringAt(425, 148, (uint8_t *)"X", LEFT_MODE);
					BSP_LCD_DisplayStringAt(425, 198, (uint8_t *)" ", LEFT_MODE);

					SysConfig.netConfig |= NET_DHCP;	//with DCHP
				}
			}
			//FFT Display or Network - #2 row
			if ((tsState.touchY[0] > 192) && (tsState.touchY[0] < 222))
			{
				if ((tsState.touchX[0] > 15) && (tsState.touchX[0] < 300))
				{
					//FFT bars
					BSP_LCD_DisplayStringAt(25, 148, (uint8_t *)" ", LEFT_MODE);
					BSP_LCD_DisplayStringAt(25, 198, (uint8_t *)"X", LEFT_MODE);
					BSP_LCD_DisplayStringAt(25, 245, (uint8_t *)" ", LEFT_MODE);

					SysConfig.fftType = FFT_DISPL_BARS;
					FFT_SetBarType(0);
				}
				if ((tsState.touchX[0] > 410) && (tsState.touchX[0] < 600))
				{
					BSP_LCD_DisplayStringAt(425, 148, (uint8_t *)" ", LEFT_MODE);
					BSP_LCD_DisplayStringAt(425, 198, (uint8_t *)"X", LEFT_MODE);

					SysConfig.netConfig &= ~NET_DHCP;	//no DHCP, static IP address
				}
			}
			//FFT Display or Network - #3 row
			if ((tsState.touchY[0] > 239) && (tsState.touchY[0] < 271))
			{
				if ((tsState.touchX[0] > 15) && (tsState.touchX[0] < 300))
				{
					//FFT lines
					BSP_LCD_DisplayStringAt(25, 148, (uint8_t *)" ", LEFT_MODE);
					BSP_LCD_DisplayStringAt(25, 198, (uint8_t *)" ", LEFT_MODE);
					BSP_LCD_DisplayStringAt(25, 245, (uint8_t *)"X", LEFT_MODE);

					SysConfig.fftType = FFT_DISPL_LINE;
					FFT_SetBarType(2);
				}
				if ((tsState.touchX[0] > 410) && (tsState.touchX[0] < 600))
				{
					//with Web Server
					BSP_LCD_DisplayStringAt(425, 245, (uint8_t *)"X", LEFT_MODE);
					BSP_LCD_DisplayStringAt(635, 245, (uint8_t *)" ", LEFT_MODE);

					SysConfig.netConfig |= NET_WEB_SERVER;	//with web server
				}
				if ((tsState.touchX[0] > 625) && (tsState.touchX[0] < 760))
				{
					//without Web Server
					BSP_LCD_DisplayStringAt(425, 245, (uint8_t *)" ", LEFT_MODE);
					BSP_LCD_DisplayStringAt(635, 245, (uint8_t *)"X", LEFT_MODE);

					SysConfig.netConfig &= ~NET_WEB_SERVER;
				}
			}

			//USB or Other - #1 row
			if ((tsState.touchY[0] > 335) && (tsState.touchY[0] < 380))
			{
				//left
				if ((tsState.touchX[0] > 15) && (tsState.touchX[0] < 300))
				{
					//left - USB
					BSP_LCD_DisplayStringAt(25, 346, (uint8_t *)"X", LEFT_MODE);
					BSP_LCD_DisplayStringAt(25, 396, (uint8_t *)" ", LEFT_MODE);

					USBConfig = 0;
				}
				//right
				if ((tsState.touchX[0] > 410) && (tsState.touchX[0] < 760))
				{
					//right - Bass Boost
					if ((tsState.touchX[0] > 410) && (tsState.touchX[0] < 600))
					{
						//with Bass Boost
						BSP_LCD_DisplayStringAt(425, 346, (uint8_t *)"X", LEFT_MODE);
						BSP_LCD_DisplayStringAt(635, 346, (uint8_t *)" ", LEFT_MODE);

						SysConfig.activeFilter |= FILT_BASS_BOOST;
					}
					if ((tsState.touchX[0] > 625) && (tsState.touchX[0] < 760))
					{
						//without BassBoost
						BSP_LCD_DisplayStringAt(425, 346, (uint8_t *)" ", LEFT_MODE);
						BSP_LCD_DisplayStringAt(635, 346, (uint8_t *)"X", LEFT_MODE);

						SysConfig.activeFilter &= ~FILT_BASS_BOOST;
					}
				}
			}
			//USB or Other - #2 row
			if ((tsState.touchY[0] > 385) && (tsState.touchY[0] < 430))
			{
				if ((tsState.touchX[0] > 15) && (tsState.touchX[0] < 300))
				{
					//left - USB
					BSP_LCD_DisplayStringAt(25, 346, (uint8_t *)" ", LEFT_MODE);
					BSP_LCD_DisplayStringAt(25, 396, (uint8_t *)"X", LEFT_MODE);

					//we do not store permanently!
					USBConfig = 1;
				}
				if ((tsState.touchX[0] > 410) && (tsState.touchX[0] < 600))
				{
					//right - Other (empty for now)
				}
			}

		    /*Refresh the LCD display*/
		    HAL_DSI_Refresh(&hdsi_discovery);

		}
		else
			taskYIELD();
	}

	//wait until touch released
	do
	{
		BSP_TS_GetState(&tsState);
		if (tsState.touchDetected)
		{
			osDelay(100);
			continue;
		}
		else
			break;
	} while(1);
}

#ifdef UART_THREAD
static void UARTCmd_Thread(void const * argument)
{
	(void)argument;
	//initialize USART1, for VCP on USB-Debug-Port, for UART Shell commands
	//UART_cmd_init();

	while(1)
	{
		//receive UART - non-blocking!
		//Remark: with FreeRTOS threads we block here if nothing available
		//it is a single character reception until entire line is complete
		UART_task();
	}
}
#endif

void Display_FFTPage(void)
{
	//BSP_LCD_SelectLayer(1);

	BSP_LCD_SetFont(&Font12);
	//BSP_LCD_SetBackColor(LCD_COLOR_TRANSPARENT);
	BSP_LCD_SetBackColor(LCD_COLOR_BLACK);

	//clear all
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_FillRect(0, 0, 800, 480);

	//display left and right
	BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
	BSP_LCD_DisplayStringAt(  2,  0, (uint8_t *)"LEFT", LEFT_MODE);
	BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
	BSP_LCD_DisplayStringAt(  2, 60, (uint8_t *)"RIGHT", LEFT_MODE);

	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);

	//create labels for the Peak Meter
	BSP_LCD_DisplayStringAt(  45, 0, (uint8_t *)"-30", LEFT_MODE);
	BSP_LCD_DisplayStringAt( 168, 0, (uint8_t *)"-20", LEFT_MODE);
	BSP_LCD_DisplayStringAt( 478, 0, (uint8_t *)"-10", LEFT_MODE);
	BSP_LCD_DisplayStringAt( 697, 0, (uint8_t *)"-7", LEFT_MODE);
	BSP_LCD_DisplayStringAt( 760, 0, (uint8_t *)"-6dB", LEFT_MODE);

	//create X, Y axis labels for FFT
	BSP_LCD_DisplayStringAt( 46, FFT_Y_BORDER_TOP - 10, (uint8_t *)"1K", LEFT_MODE);
	BSP_LCD_DisplayStringAt( 88, FFT_Y_BORDER_TOP - 10, (uint8_t *)"|", LEFT_MODE);
	BSP_LCD_DisplayStringAt(124, FFT_Y_BORDER_TOP - 10, (uint8_t *)"|", LEFT_MODE);
	BSP_LCD_DisplayStringAt(160, FFT_Y_BORDER_TOP - 10, (uint8_t *)"|", LEFT_MODE);
	BSP_LCD_DisplayStringAt(197, FFT_Y_BORDER_TOP - 10, (uint8_t *)"5K", LEFT_MODE);
	BSP_LCD_DisplayStringAt(236, FFT_Y_BORDER_TOP - 10, (uint8_t *)"|", LEFT_MODE);
	BSP_LCD_DisplayStringAt(272, FFT_Y_BORDER_TOP - 10, (uint8_t *)"|", LEFT_MODE);
	BSP_LCD_DisplayStringAt(313, FFT_Y_BORDER_TOP - 10, (uint8_t *)"|", LEFT_MODE);
	BSP_LCD_DisplayStringAt(349, FFT_Y_BORDER_TOP - 10, (uint8_t *)"|", LEFT_MODE);
	BSP_LCD_DisplayStringAt(380, FFT_Y_BORDER_TOP - 10, (uint8_t *)"10K", LEFT_MODE);
	BSP_LCD_DisplayStringAt(424, FFT_Y_BORDER_TOP - 10, (uint8_t *)"|", LEFT_MODE);
	BSP_LCD_DisplayStringAt(460, FFT_Y_BORDER_TOP - 10, (uint8_t *)"|", LEFT_MODE);
	BSP_LCD_DisplayStringAt(495, FFT_Y_BORDER_TOP - 10, (uint8_t *)"|", LEFT_MODE);
	BSP_LCD_DisplayStringAt(535, FFT_Y_BORDER_TOP - 10, (uint8_t *)"|", LEFT_MODE);
	BSP_LCD_DisplayStringAt(569, FFT_Y_BORDER_TOP - 10, (uint8_t *)"15K", LEFT_MODE);
	BSP_LCD_DisplayStringAt(608, FFT_Y_BORDER_TOP - 10, (uint8_t *)"|", LEFT_MODE);
	BSP_LCD_DisplayStringAt(644, FFT_Y_BORDER_TOP - 10, (uint8_t *)"|", LEFT_MODE);
	BSP_LCD_DisplayStringAt(684, FFT_Y_BORDER_TOP - 10, (uint8_t *)"|", LEFT_MODE);
	BSP_LCD_DisplayStringAt(721, FFT_Y_BORDER_TOP - 10, (uint8_t *)"|", LEFT_MODE);
	BSP_LCD_DisplayStringAt(758, FFT_Y_BORDER_TOP - 10, (uint8_t *)"20KHz", LEFT_MODE);
}

/**
  * @brief  End of Refresh DSI callback.
  * @param  hdsi: pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @retval None
  */
void HAL_DSI_EndOfRefreshCallback(DSI_HandleTypeDef *hdsi)
{
  (void)hdsi;
  if(pend_buffer >= 0)
  { 
    /* Disable DSI Wrapper */
    __HAL_DSI_WRAPPER_DISABLE(&hdsi_discovery);
    /* Update LTDC configuration */
    LTDC_LAYER(&hltdc_discovery, 0)->CFBAR = ((uint32_t)Buffers[pend_buffer]);
    __HAL_LTDC_RELOAD_CONFIG(&hltdc_discovery);
    /* Enable DSI Wrapper */
    __HAL_DSI_WRAPPER_ENABLE(&hdsi_discovery);
    
    front_buffer = pend_buffer;  
    pend_buffer = -1;
  }
}

/**
  * @brief  Initializes the DSI LCD. 
  * The initialization is done as below:
  *     - DSI PLL initialization
  *     - DSI initialization
  *     - LTDC initialization
  *     - OTM8009A LCD Display IC Driver initialization
  * @param  None
  * @retval LCD state
  */
static uint8_t LCD_Init(void)
{
  /* Toggle Hardware Reset of the DSI LCD using
  * its XRES signal (active low) */
  BSP_LCD_Reset();
  
  /* Call first MSP Initialize only in case of first initialization
  * This will set IP blocks LTDC, DSI and DMA2D
  * - out of reset
  * - clocked
  * - NVIC IRQ related to IP blocks enabled
  */
  BSP_LCD_MspInit();
  
  /* LCD clock configuration */
  /* PLLSAI_VCO Input = HSE_VALUE/PLL_M = 1 Mhz */
  /* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN = 417 Mhz */
  /* PLLLCDCLK = PLLSAI_VCO Output/PLLSAIR = 417 MHz / 5 = 83.4 MHz */
  /* LTDC clock frequency = PLLLCDCLK / LTDC_PLLSAI_DIVR_2 = 83.4 / 2 = 41.7 MHz */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
  PeriphClkInitStruct.PLLSAI.PLLSAIN = 417;
  PeriphClkInitStruct.PLLSAI.PLLSAIR = 5;
  PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_2;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
  
  /* Base address of DSI Host/Wrapper registers to be set before calling De-Init */
  hdsi_discovery.Instance = DSI;
  
  HAL_DSI_DeInit(&(hdsi_discovery));
  
  dsiPllInit.PLLNDIV  = 100;
  dsiPllInit.PLLIDF   = DSI_PLL_IN_DIV5;
  dsiPllInit.PLLODF   = DSI_PLL_OUT_DIV1;  

  hdsi_discovery.Init.NumberOfLanes = DSI_TWO_DATA_LANES;
  hdsi_discovery.Init.TXEscapeCkdiv = 0x4;
  HAL_DSI_Init(&(hdsi_discovery), &(dsiPllInit));
    
  /* Configure the DSI for Command mode */
  CmdCfg.VirtualChannelID      = 0;
  CmdCfg.HSPolarity            = DSI_HSYNC_ACTIVE_HIGH;
  CmdCfg.VSPolarity            = DSI_VSYNC_ACTIVE_HIGH;
  CmdCfg.DEPolarity            = DSI_DATA_ENABLE_ACTIVE_HIGH;
  CmdCfg.ColorCoding           = DSI_RGB888;
  CmdCfg.CommandSize           = HACT;
  CmdCfg.TearingEffectSource   = DSI_TE_DSILINK;
  CmdCfg.TearingEffectPolarity = DSI_TE_RISING_EDGE;
  CmdCfg.VSyncPol              = DSI_VSYNC_FALLING;
  CmdCfg.AutomaticRefresh      = DSI_AR_DISABLE;
  CmdCfg.TEAcknowledgeRequest  = DSI_TE_ACKNOWLEDGE_ENABLE;
  HAL_DSI_ConfigAdaptedCommandMode(&hdsi_discovery, &CmdCfg);
  
  LPCmd.LPGenShortWriteNoP    = DSI_LP_GSW0P_ENABLE;
  LPCmd.LPGenShortWriteOneP   = DSI_LP_GSW1P_ENABLE;
  LPCmd.LPGenShortWriteTwoP   = DSI_LP_GSW2P_ENABLE;
  LPCmd.LPGenShortReadNoP     = DSI_LP_GSR0P_ENABLE;
  LPCmd.LPGenShortReadOneP    = DSI_LP_GSR1P_ENABLE;
  LPCmd.LPGenShortReadTwoP    = DSI_LP_GSR2P_ENABLE;
  LPCmd.LPGenLongWrite        = DSI_LP_GLW_ENABLE;
  LPCmd.LPDcsShortWriteNoP    = DSI_LP_DSW0P_ENABLE;
  LPCmd.LPDcsShortWriteOneP   = DSI_LP_DSW1P_ENABLE;
  LPCmd.LPDcsShortReadNoP     = DSI_LP_DSR0P_ENABLE;
  LPCmd.LPDcsLongWrite        = DSI_LP_DLW_ENABLE;
  HAL_DSI_ConfigCommand(&hdsi_discovery, &LPCmd);

  /* Initialize LTDC */
  LTDC_Init();
  
  /* Start DSI */
  HAL_DSI_Start(&(hdsi_discovery));
    
  /* Initialize the OTM8009A LCD Display IC Driver (KoD LCD IC Driver)
  *  depending on configuration set in 'hdsivideo_handle'.
  */
  OTM8009A_Init(OTM8009A_COLMOD_RGB888, LCD_ORIENTATION_LANDSCAPE);
  
  LPCmd.LPGenShortWriteNoP    = DSI_LP_GSW0P_DISABLE;
  LPCmd.LPGenShortWriteOneP   = DSI_LP_GSW1P_DISABLE;
  LPCmd.LPGenShortWriteTwoP   = DSI_LP_GSW2P_DISABLE;
  LPCmd.LPGenShortReadNoP     = DSI_LP_GSR0P_DISABLE;
  LPCmd.LPGenShortReadOneP    = DSI_LP_GSR1P_DISABLE;
  LPCmd.LPGenShortReadTwoP    = DSI_LP_GSR2P_DISABLE;
  LPCmd.LPGenLongWrite        = DSI_LP_GLW_DISABLE;
  LPCmd.LPDcsShortWriteNoP    = DSI_LP_DSW0P_DISABLE;
  LPCmd.LPDcsShortWriteOneP   = DSI_LP_DSW1P_DISABLE;
  LPCmd.LPDcsShortReadNoP     = DSI_LP_DSR0P_DISABLE;
  LPCmd.LPDcsLongWrite        = DSI_LP_DLW_DISABLE;
  HAL_DSI_ConfigCommand(&hdsi_discovery, &LPCmd);
  
   HAL_DSI_ConfigFlowControl(&hdsi_discovery, DSI_FLOW_CONTROL_BTA);
  /* Refresh the display */
  HAL_DSI_Refresh(&hdsi_discovery);
  
  return LCD_OK; 
}

/**
  * @brief   LTDC initialization
  * @param  None
  * @retval None
  */
void LTDC_Init(void)
{
  /* DeInit */
  HAL_LTDC_DeInit(&hltdc_discovery);
  
  /* LTDC Config */
  /* Timing and polarity */
  hltdc_discovery.Init.HorizontalSync = HSYNC;
  hltdc_discovery.Init.VerticalSync = VSYNC;
  hltdc_discovery.Init.AccumulatedHBP = HSYNC+HBP;
  hltdc_discovery.Init.AccumulatedVBP = VSYNC+VBP;
  hltdc_discovery.Init.AccumulatedActiveH = VSYNC+VBP+VACT;
  hltdc_discovery.Init.AccumulatedActiveW = HSYNC+HBP+HACT;
  hltdc_discovery.Init.TotalHeigh = VSYNC+VBP+VACT+VFP;
  hltdc_discovery.Init.TotalWidth = HSYNC+HBP+HACT+HFP;
  
  /* background value */
  hltdc_discovery.Init.Backcolor.Blue = 0;
  hltdc_discovery.Init.Backcolor.Green = 0;
  hltdc_discovery.Init.Backcolor.Red = 0;
  
  /* Polarity */
  hltdc_discovery.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  hltdc_discovery.Init.VSPolarity = LTDC_VSPOLARITY_AL;
  hltdc_discovery.Init.DEPolarity = LTDC_DEPOLARITY_AL;
  hltdc_discovery.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
  hltdc_discovery.Instance = LTDC;

  HAL_LTDC_Init(&hltdc_discovery);
}

#if 0
/**
  * @brief  Converts a line to an ARGB8888 pixel format.
  * @param  pSrc: Pointer to source buffer
  * @param  pDst: Output color
  * @param  xSize: Buffer width
  * @param  ColorMode: Input color mode   
  * @retval None
  */
static void CopyBuffer(uint32_t *pSrc, uint32_t *pDst, uint16_t x, uint16_t y, uint16_t xsize, uint16_t ysize)
{   
  uint32_t destination = (uint32_t)pDst + (y * 800 + x) * 4;
  uint32_t source      = (uint32_t)pSrc;
  
  /*##-1- Configure the DMA2D Mode, Color Mode and output offset #############*/ 
  hdma2d.Init.Mode         = DMA2D_M2M;
  hdma2d.Init.ColorMode    = DMA2D_OUTPUT_ARGB8888;
  hdma2d.Init.OutputOffset = 800 - xsize;
  hdma2d.Init.AlphaInverted = DMA2D_REGULAR_ALPHA;  /* No Output Alpha Inversion*/  
  hdma2d.Init.RedBlueSwap   = DMA2D_RB_REGULAR;     /* No Output Red & Blue swap */    
  
  /*##-2- DMA2D Callbacks Configuration ######################################*/
  hdma2d.XferCpltCallback  = NULL;
  
  /*##-3- Foreground Configuration ###########################################*/
  hdma2d.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
  hdma2d.LayerCfg[1].InputAlpha = 0xFF;
  hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB8888;
  hdma2d.LayerCfg[1].InputOffset = 0;
  hdma2d.LayerCfg[1].RedBlueSwap = DMA2D_RB_REGULAR; /* No ForeGround Red/Blue swap */
  hdma2d.LayerCfg[1].AlphaInverted = DMA2D_REGULAR_ALPHA; /* No ForeGround Alpha inversion */  

  hdma2d.Instance          = DMA2D; 
   
  /* DMA2D Initialization */
  if(HAL_DMA2D_Init(&hdma2d) == HAL_OK) 
  {
    if(HAL_DMA2D_ConfigLayer(&hdma2d, 1) == HAL_OK) 
    {
      if (HAL_DMA2D_Start(&hdma2d, source, destination, xsize, ysize) == HAL_OK)
      {
        /* Polling For DMA transfer */  
        HAL_DMA2D_PollForTransfer(&hdma2d, 100);
      }
    }
  }   
}
#endif

/**
  * @brief  Configure the MPU attributes as Write Through for SRAM1/2.
  * @note   The Base Address is 0x20010000 since this memory interface is the AXI.
  *         The Region Size is 256KB, it is related to SRAM1 and SRAM2  memory size.
  * @param  None
  * @retval None
  */
void MPU_Config(void)
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

  /* Configure the MPU attributes as WT for SDRAM */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0xC0000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_8MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER2;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Configure the MPU attributes as WT for QSPI (used 16Mbytes) */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0x90000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_16MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_PRIV_RO;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER4;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

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
  *            PLL_R                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 7
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
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
  RCC_OscInitStruct.PLL.PLLN = 435;					//430: seems to work, 420: try 412: 400 for 200 MHz - needed if DDR RAM!, 432 for 216 MHz
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;	//9;			//9: for 216MHz, 8 for 200 MHz with PLLN 400;
  //RCC_OscInitStruct.PLL.PLLR = 7;					//should be default 2?
  
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
  
  /* Activate the OverDrive to reach the 216/200 MHz Frequency */
  ret = HAL_PWREx_EnableOverDrive();
  if(ret != HAL_OK)
  {
	  while(1) { ; }
  }

  //if with 200 MHz HSE Clock - USB needs other clock setting for 48MHz on USB
  /* Select PLLSAI output as USB clock source */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_CLK48;
  PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48SOURCE_PLLSAIP;	//RCC_CLK48SOURCE_PLL;	//RCC_CLK48SOURCE_PLLSAIP;
  PeriphClkInitStruct.PLLSAI.PLLSAIN = 404;			//384 is nominal
  PeriphClkInitStruct.PLLSAI.PLLSAIQ = 8;			//7;
  PeriphClkInitStruct.PLLSAI.PLLSAIP = RCC_PLLSAIP_DIV8;
  /* we trim with debugging for most accurate timing: USB clock should be 48KHz, play clock also 48 KHz,
   * we have trimmed play clock, but USB clock is independent and drifts!
   * So, we might have periodically a wrong sample from USB MIC (both clocks are not locked, they are
   * free running and independent)
   */
  if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct)  != HAL_OK)
  {
	  while(1) { ; }
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2; 
  
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5); //FLASH_LATENCY
  if(ret != HAL_OK)
  {
	  while(1) { ; }
  }

  //it results in 96KHz SPDIFRX clock - but for 48KHz SPDIF Rx
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SPDIFRX;
  PeriphClkInitStruct.PLLI2S.PLLI2SN = 192;
  PeriphClkInitStruct.PLLI2S.PLLI2SP = RCC_PLLP_DIV2;
  PeriphClkInitStruct.PLLI2S.PLLI2SR = 2;
  PeriphClkInitStruct.PLLI2S.PLLI2SQ = 2;
  PeriphClkInitStruct.PLLI2SDivQ = 1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
      return;
  }
}

/**
* @brief  CPU L1-Cache enable.
* @param  None
* @retval None
*/
void CPU_CACHE_Enable(void)
{
  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();
}

#ifdef USE_DMA2D
/**
  * @brief  the DMA2D initialization, DMA2D used as vector memcpy
  * @param  none:
  * @retval None
  */
static void MX_DMA2D_Init(void)
{
  /**
   * DMA2D as Mem-to-Mem works just with foreground (FiFo)
   */
  hdma2d.Instance = DMA2D;
  hdma2d.Init.Mode = DMA2D_M2M;
  hdma2d.Init.ColorMode = DMA2D_ARGB8888;
  hdma2d.Init.OutputOffset = 1;					//2 pixels per line, jump to next half
  hdma2d.LayerCfg[1].InputOffset = 0;
  hdma2d.LayerCfg[1].InputColorMode = CM_ARGB8888;
  hdma2d.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
  hdma2d.LayerCfg[1].InputAlpha = 0x0;
  HAL_DMA2D_Init(&hdma2d);

  HAL_DMA2D_ConfigLayer(&hdma2d, 1);		//foreground
}
#endif

void BSP_AUDIO_IN_TransferComplete_CallBack(void)
{
}

void BSP_AUDIO_IN_HalfTransfer_CallBack(void)
{
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  BSP_LED_On(LED1);
  //while (1) {}

  /* USER CODE END Error_Handler_Debug */
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
