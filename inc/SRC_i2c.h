/**
  ******************************************************************************
  * @file    SRC_i2c.h
  * @author  Torsten Jaekel
  * @version V1.1.0
  * @date    04-September-2016
  * @brief   The I2C interface to the DIGI-FP board (for Lyrebird APP Ultra)
  ******************************************************************************
  */

#ifndef SRC_I2C_H_
#define SRC_I2C_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f769i_discovery.h"

/* Declarations and Definitions ----------------------------------------------*/

#define SRC_I2C_ADDR		0xE2					//DIGI-FP I2C SRC address, on DIGI-FP board

/* Enumerations --------------------------------------------------------------*/

typedef enum {
	CLOCK_SRC_INTERNAL,								//internal XTAL
	CLOCK_SRC_EXTERNAL								//Word Clock
} EClockSrc;

typedef enum {
	CLOCK_XTAL_1,									//the 24.576 MHz XTAL  - for 48 KHz based sound
	CLOCK_XTAL_2									//the 22.5792 MHz XTAL - for 44.1 KHz based sound
} ERefClk;

typedef enum {
	SRC_SOURCE_TOSLINK,								//ATT: we use the value also as SRC register value
	SRC_SOURCE_SPDIF,								//the value must match here with SRC register values
	SRC_SOURCE_AES3,
	SRC_UNUSED										//there is nothing connected on DIGI-FP board
} EAudioSource;

typedef enum {
	SRC_RATE_1,										//x1
	SRC_RATE_2,										//x2
	SRC_RATE_4										//x4
} ESampleRate;

/* Function Prototypes -------------------------------------------------------*/

//global init and deinit
int SRC_I2C_Configure(void);
int SRC_I2C_Release(void);

//SRC chip related config, here to select input (source) and upsample rate (x1, x2, x4)
int SRC_AudioSource(EAudioSource src);
int SRC_SetSampleRate(ESampleRate rate);

//configure SRC as DIR Rx clock source (internal) or to use MCLK (external)
int SRC_SetRefClock(EClockSrc ref);

#endif /* SRC_I2C_H_ */
