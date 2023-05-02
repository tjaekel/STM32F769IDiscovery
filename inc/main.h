/**
  ******************************************************************************
  * @file    main.h
  * @author  Torsten Jaekel
  * @version V1.1.0
  * @date    04-September-2016
  * @brief   The main definitions, includes, interfaces for the system
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_audio.h"
#include "usbd_audio_if.h"
#include "usbd_main.h"
#include "stm32f769i_discovery.h"
#include "stm32f769i_discovery_lcd.h"
#include "stm32f769i_discovery_sdram.h"
#include "stm32f769i_discovery_audio.h"
#include "stm32f769i_discovery_ts.h"
#include "stm32f769i_discovery_qspi.h"
#include "stm32f769i_discovery_sd.h"

#include "cmsis_os.h"
#include "semphr.h"
#include "arm_math.h"

#include "AudioPlayer.h"
#include "SPDIF_Rx.h"
#include "SPDIF_Tx.h"
#include "VUMeter.h"

#include "SRC_i2c.h"
#include "SRC_i2s_in.h"

#include "ffconf.h"
#include "SDCard.h"
#include "SDPlay.h"

#include "Tone_gen.h"

#include "SPDIF_Rx.h"

#include "usbd_desc_msc.h"
#include "usbd_storage.h"

#include "uart_cmd.h"
#include "VCP_UART.h"

//what is our main function:
//just web server
//#define	main_http	main
//audio
#define main_audio	main

#define AUDIO_FREQ_DEFAULT	48000

// stop watch to measure performance
volatile uint32_t time_begin, time_start, time_end, time_diff, time_total;
TIM_HandleTypeDef hTim2;

#define TIM_MEASURE_BEGIN 	do { \
								if ( ! time_begin) \
									time_begin = hTim2.Instance->CNT; \
						  	} \
							while(0)
#define TIM_MEASURE_START 	time_start = hTim2.Instance->CNT
#define TIM_MEASURE_END 	time_end = hTim2.Instance->CNT; \
                        	time_diff = time_end - time_start
#define TIM_MEASURE_TOTAL 	time_total = time_end - time_begin; time_begin = 0

extern SAI_HandleTypeDef               haudio_out_sai;
extern SAI_HandleTypeDef			   haudio_in_sai;
extern DFSDM_Filter_HandleTypeDef      hAudioInTopLeftFilter;
extern DFSDM_Filter_HandleTypeDef      hAudioInTopRightFilter;

extern LTDC_HandleTypeDef 			   hltdc_discovery;
extern DSI_HandleTypeDef 			   hdsi_discovery;

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/*Static IP ADDRESS*/
/* with Network Audio - we do not need really IP address, running in promiscuous mode and never sending */
#define IP_ADDR0   		192
#define IP_ADDR1   		168
#define IP_ADDR2   		1
#define IP_ADDR3   		200

/*NETMASK*/
#define NETMASK_ADDR0   255
#define NETMASK_ADDR1   255
#define NETMASK_ADDR2   255
#define NETMASK_ADDR3   0

/*Gateway Address*/
#define GW_ADDR0   		IP_ADDR0
#define GW_ADDR1   		IP_ADDR1
#define GW_ADDR2   		IP_ADDR2
#define GW_ADDR3   		1

/*Input interface selection*/
#define	INIF_SPDIF_STM	0
#define	INIF_USB		1
#define	INIF_MIC		2
#define	INIF_LINE		3
#define	INIF_UDANTE		4
#define	INIF_SPDIF_D	5		//rear, on DIGI-FP
#define	INIF_TOS_D		6		//rear, on DIGI-FP
#define	INIF_AES3_D		7		//rear, on DIGI-FP
#define	INIF_BT_D		8		//free input on SRC, DIGI-FP, for BT
#define	INIF_SD			9		//SD Card File menu and player
#define	INIF_GEN		10		//Tone Generator

typedef struct {
	int		ifSelection;
	int		outSelection;
	int		sampleFreq;
} TSYSInterfaces;

extern const char VERSION_STR[7];
extern TSYSInterfaces SysInterfaces;

#ifdef USE_DMA2D

/** @addtogroup BSP
  * @{
  */

/* External References -------------------------------------------------------*/

/**
 * reference to the peripheral handler
 */
extern DMA2D_HandleTypeDef hdma2d;

/* Function Prototypes -------------------------------------------------------*/

/**
  * @}
  */
#endif
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern void fast_memcpy(void *dst, void *src, size_t len);

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
