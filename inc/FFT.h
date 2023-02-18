/**
  ******************************************************************************
  * @file    FFT.h
  * @author  Torsten Jaekel
  * @version V1.1.0
  * @date    04-September-2016
  * @brief   The FFT anaylyzer and display functions
  ******************************************************************************
  */

#ifndef FFT_H_
#define FFT_H_

#include "AudioPlayer.h"

#define	FFT_SIZE			256						//256, 512 or 1024, nothing else, just 256 supported
#define FFT_SAMPLES 		(2*FFT_SIZE)

#define	FFT_Y_BORDER		0
#define FFT_Y_BORDER_TOP	70
#define FFT_X_WIDTH			20
#define FFT_X_W_RECT		(FFT_X_WIDTH-3)
#define FFT_X_START			20

#define X_STEPS				7						//for line and point graph

#define FFT_BIN_SKIP		0						//0..10 : how many low freq. bins to skip for higher resolution on higher freq.
#define FFT_DISP_TYPE		2						//0..2 : 0 = bars, 1 = points, 2 = lines - 1: points, it the fastest way

#define FFT_X_DISP			690						//X position where to start with text info, for magnitude values

#define	COLOR_INACTIVE		LCD_COLOR_BLACK
#define	COLOR_LEFT			LCD_COLOR_YELLOW
#define	COLOR_RIGHT			LCD_COLOR_GREEN

extern int32_t 	FFT_Init(void);
extern int32_t 	FFT_Filter(int ch);
extern int32_t 	FFT_Ready(float32_t **result, int *numResults);
extern void 	FFT_DisplayGraph(int ch);
extern void 	FFT_SetBarType(int type);

#endif /* FFT_H_ */
