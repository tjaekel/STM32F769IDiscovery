/**
  ******************************************************************************
  * @file    VUMeter.h
  * @author  Torsten Jaekel
  * @version V1.1.0
  * @date    04-September-2016
  * @brief   The interface for the VU, Peak Meter analysis and display
  ******************************************************************************
  */

#ifndef VUMETER_H_
#define VUMETER_H_

#include "main.h"

#define NUM_CHANNELS	2
#define MAX_VALUE 		0x7FFF				// for int16 (16bit samples)
#define SEGMENTS		25
#define X_START			10
#define	Y_START			13
#define X_WIDTH			30
#define Y_WIDTH			20
#define XY_GAP   		 1
#define MAX_X			800

#define PEAKHOLD		2000				// Time of peak hold in ms
#define LEVELMOVE		 100				// Time to lower level in ms, if too slow (in relation to buffer size) - it looks strange on highest segment

typedef struct								// VU Meter Parameters
{
	uint16_t channel;						// defines active channel for processing
	int16_t  vuInput[NUM_CHANNELS];			// value for VU-Input (0 = left, 1 = right)
	int16_t  vuPeak[NUM_CHANNELS];			// Peak value for VU-Meter peak hold
	uint32_t vuTick[NUM_CHANNELS];			// Time constant for release VU-Input
	uint32_t vuTickPeak[NUM_CHANNELS];		// Time constant for release VU-Input Peak
} t_VUparams;

extern void VUMETER_Analyse(void);
extern void VUMETER_Input(int channel);
extern void VUMETER_Display(int channel);
extern void VUMETER_Volume(int channel, int level);
extern void VUMETER_ProcessVolume(int x);

#endif /* VUMETER_H_ */
