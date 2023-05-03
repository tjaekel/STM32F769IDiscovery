/**
  ******************************************************************************
  * @file    FFT.c
  * @author  Torsten Jaekel
  * @version V1.1.0
  * @date    04-September-2016
  * @brief   The FFT analysis and display functions
  ******************************************************************************
  */

/* ----------------------------------------------------------------------
* Copyright (C) 2010-2012 ARM Limited. All rights reserved.
*
* $Date:         17. January 2013
* $Revision:     V1.4.0
*
* Project:       CMSIS DSP Library
* Title:	     arm_fft_bin_example_f32.c
*
* Description:   Example code demonstrating calculation of Max energy bin of
*                frequency domain of input signal.
*
* Target Processor: Cortex-M4/Cortex-M3
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*   - Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   - Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in
*     the documentation and/or other materials provided with the
*     distribution.
*   - Neither the name of ARM LIMITED nor the names of its contributors
*     may be used to endorse or promote products derived from this
*     software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
* -------------------------------------------------------------------- */

/**
 * @ingroup groupExamples
 */

/**
 * @defgroup FrequencyBin Frequency Bin Example
 *
 * \par Description
 * \par
 * Demonstrates the calculation of the maximum energy bin in the frequency
 * domain of the input signal with the use of Complex FFT, Complex
 * Magnitude, and Maximum functions.
 *
 * \par Algorithm:
 * \par
 * The input test signal contains a 10 kHz signal with uniformly distributed white noise.
 * Calculating the FFT of the input signal will give us the maximum energy of the
 * bin corresponding to the input frequency of 10 kHz.
 *
 * \par Block Diagram:
 * \image html FFTBin.gif "Block Diagram"
 * \par
 * The figure below shows the time domain signal of 10 kHz signal with
 * uniformly distributed white noise, and the next figure shows the input
 * in the frequency domain. The bin with maximum energy corresponds to 10 kHz signal.
 * \par
 * \image html FFTBinInput.gif "Input signal in Time domain"
 * \image html FFTBinOutput.gif "Input signal in Frequency domain"
 *
 * \par Variables Description:
 * \par
 * \li \c testInput_f32_10khz points to the input data
 * \li \c testOutput points to the output data
 * \li \c fftSize length of FFT
 * \li \c ifftFlag flag for the selection of CFFT/CIFFT
 * \li \c doBitReverse Flag for selection of normal order or bit reversed order
 * \li \c refIndex reference index value at which maximum energy of bin ocuurs
 * \li \c testIndex calculated index value at which maximum energy of bin ocuurs
 *
 * \par CMSIS DSP Software Library Functions Used:
 * \par
 * - arm_cfft_f32()
 * - arm_cmplx_mag_f32()
 * - arm_max_f32()
 *
 * <b> Refer  </b>
 * \link arm_fft_bin_example_f32.c \endlink
 *
 */


/** \example arm_fft_bin_example_f32.c
  */

#include <math.h>

#include "FFT.h"
#include "arm_const_structs.h"
#include "stm32f769i_discovery_lcd.h"

int XXTouch = 0;

/* ------------------------------------------------------------------
* Global variables for FFT Bin Example
* ------------------------------------------------------------------- */
uint32_t ifftFlag = 0;									//specifies if FFT or IFFT
uint32_t doBitReverse = 1;								//in order to get the right sequence of results

/* Reference index at which max energy of bin occurs */
uint32_t refIndex = 213;

static q31_t 	 sFFTBuf[FFT_SAMPLES];
static float32_t input_f32[FFT_SAMPLES];
static float32_t input_c32[2 * FFT_SIZE];				//*2 due to complex R + jX as input
static float32_t fftResult[2 * FFT_SIZE];				//*2 due to complex R + jX as output

static int fftResultReady = 0;
static float32_t sFFTmaxValue;
static float32_t sFFTminValue;

//with Hanning Window we avoid Spectrum Leakage but it makes every bin larger (not so sharp with single frequency anymore)
static char sHanningWindow = 0;							//Config: with Rectangle (0) or Hanning Window (<>0)

static uint16_t prevFftY[2][FFT_SIZE / 2 + 1];			//append the skipFFTBins as 0 values
static uint8_t  skipFFTBins = FFT_BIN_SKIP;				//just 0..6 is valid
static uint8_t  barType = FFT_DISP_TYPE;				//which FFT graph: 0..2

void FFT_Display_BarGraph(int ch);						//forward declaration
void FFT_Display_Points(int ch);
void FFT_Display_Lines(int ch);

/*
 * Hanning Window Coefficients, first half only (symmetrical)
 * w(n) = 0.5 - 0.5 * cos (2 * PI * n / N)  | n = 0,1,2 ... N, L = N + 1, for 256-pt FFT 128 values
 */
static const float32_t sHanningWindowCoeff[] = {
		//0
		0.0f,
		0.000151774f,
		0.0006070039f,
		0.0013654133f,
		0.0024265418f,
		0.0037897452f,
		0.0054541958f,
		0.0074188833f,
		0.0096826148f,
		0.012244016f,
		//10
		0.015101532f,
		0.0182534279f,
		0.0216977902f,
		0.025432528f,
		0.0294553737f,
		0.0337638853f,
		0.0383554469f,
		0.0432272712f,
		0.0483764003f,
		//20
		0.0537997084f,
		0.0594939029f,
		0.0654555268f,
		0.0716809611f,
		0.0781664261f,
		0.0849079846f,
		0.0919015438f,
		0.099142858f,
		0.106627531f,
		0.1143510188f,
		//30
		0.1223086326f,
		0.1304955414f,
		0.1389067748f,
		0.1475372265f,
		0.156381657f,
		0.1654346968f,
		0.1746908499f,
		0.1841444969f,
		0.1937898985f,
		0.203621199f,
		//40
		0.21363243f,
		0.2238175135f,
		0.2341702664f,
		0.2446844034f,
		0.2553535415f,
		0.2661712036f,
		0.2771308221f,
		0.2882257437f,
		0.2994492325f,
		0.310794475f,
		//50
		0.3222545833f,
		0.3338226003f,
		0.3454915028f,
		0.3572542069f,
		0.3691035713f,
		0.3810324025f,
		0.3930334584f,
		0.4050994533f,
		0.4172230619f,
		0.4293969241f,
		//60
		0.4416136491f,
		0.4538658203f,
		0.4661459993f,
		0.478446731f,
		0.4907605475f,
		0.5030799733f,
		0.5153975293f,
		0.5277057375f,
		0.5399971256f,
		0.5522642316f,
		//70
		0.5644996083f,
		0.5766958274f,
		0.5888454849f,
		0.6009412046f,
		0.6129756433f,
		0.6249414949f,
		0.636831495f,
		0.6486384253f,
		0.6603551178f,
		0.6719744593f,
		//80
		0.6834893958f,
		0.6948929366f,
		0.7061781587f,
		0.7173382109f,
		0.7283663179f,
		0.7392557846f,
		0.75f,
		0.7605924414f,
		0.7710266782f,
		0.7812963758f,
		//90
		0.7913952995f,
		0.8013173182f,
		0.8110564084f,
		0.8206066574f,
		0.8299622674f,
		0.8391175587f,
		0.848066973f,
		0.8568050772f,
		0.8653265665f,
		0.8736262674f,
		//100
		0.8816991414f,
		0.8895402873f,
		0.8971449448f,
		0.9045084972f,
		0.9116264741f,
		0.9184945542f,
		0.9314644998f,
		0.9375584914f,
		0.943386843f,
		0.9489460161f,
		//110
		0.9542326359f,
		0.9592434929f,
		0.9639755449f,
		0.9684259193f,
		0.9725919141f,
		0.9764710002f,
		0.9800608227f,
		0.9833592021f,
		0.9863641361f,
		0.9890738004f,
		//120
		0.9914865498f,
		0.9936009198f,
		0.9954156265f,
		0.9969295684f,
		0.9981418264f,
		0.9990516644f,
		0.9996585301f,
		0.9999620551f
};
/* ----------------------------------------------------------------------
* Max magnitude FFT Bin test
* ------------------------------------------------------------------- */

int32_t FFT_Init(void)
{
	//just reset the index
	fftResultReady = 0;

	return 0;
}

int32_t FFT_Ready(float32_t **result, int *numResults)
{
	if (fftResultReady)
	{
		*result = fftResult + (FFT_SIZE / 2);
		*numResults = (FFT_SIZE / 2);
		return 1;			//available, ready
	}
	else
		return 0;			//not yet ready, available
}

/**
  * @fn void DC_BlockerSample(int ch, int32_t *inOutPtr)
  * @brief  remove DC from one sample on one channel
  * @param  ch: the channel mumber
  * @param  inOutPtr: the pointer to the (32bit) sample
  * @retval None
  */
static void DC_BlockerSample(int32_t *inOutPtr, int ch)
{
//define float -> uint32_t conversion with rounding handling,
//make sure not to use LIB function round which is different
//ATT: use suffix f for 0.5, otherwise converted to double internally
#define ROUND(x) ((x)>=0?(q31_t)((x)+0.5):(q31_t)((x)-0.5))
#define ALPHA 0.9999			//the factor - the larger the faster to reach DC free signal: 0.999 results in -3dB @ 10Hz

	static double lastIn[2]  = {0.0, 0.0};
	static double lastOut[2] = {0.0, 0.0};
	register double tmp, res;

	tmp = (double)*inOutPtr;

	res = tmp - lastIn[ch] + ALPHA * lastOut[ch];
	lastIn[ch]  = tmp;
	lastOut[ch] = res;

	*inOutPtr = ROUND(res);
}

int32_t FFT_Filter(int ch)
{
  int i, j;
  q31_t *inBuf = sFFTBuf;
  int16_t *sampleBuf;
  int bufLen;
  uint32_t testIndex;

  //get the buffer for the audio to analyze
  bufLen = AUDIO_PLAYER_GetBuffer((uint8_t **)&sampleBuf);
  if (AUDIO_PLAYER_GetState())
  {
		//second buffer was played - now the first is active
		sampleBuf += bufLen / (2 + sizeof(int16_t));
  }
  else
  {
		//first buffer was played - now the second is active
		//sampleBuf += bufLen / (2 + sizeof(int16_t));
  }

  if (ch)
	  sampleBuf++;

  bufLen = FFT_SAMPLES;											//in 16bit words, for half-buffer mono

  //prepare buffer for FFT: as 32bit signed, single channel
  while (bufLen--)
  {
	  *inBuf = (q31_t)(*sampleBuf++) << 16;						//scale up into highest value range for q31
	  sampleBuf++;												//skip the other stereo channel
	  DC_BlockerSample(inBuf++, ch);							//remove DC from samples
  }
  inBuf = sFFTBuf;

  //convert as Q31 (as int32_t) to float for FFT
  arm_q31_to_float(inBuf, input_f32, FFT_SAMPLES);

  {
	  //convert the float buffer into complex value buffer (store just real part, the other imaginary part remains 0)
	  for (i = 0, j = 0; i < FFT_SIZE; i++)
	  {
		  if (sHanningWindow)
		  {
			  input_c32[2 * i] = input_f32[i] * sHanningWindowCoeff[j];
			  if (j != 127)
			  {
				  if (j < 127)
					  j++;
				  else
					  j--;
			  }
		  }
		  else
			  input_c32[2 * i] = input_f32[i];

		  input_c32[2 * i + 1] = 0.0f;
	  }

	  /* Process the data through the CFFT/CIFFT module - depends on FFT size */
#if FFT_SIZE == 1024
	  arm_cfft_f32(&arm_cfft_sR_f32_len1024, input_c32, ifftFlag, doBitReverse);
#elif FFT_SIZE == 512
	  arm_cfft_f32(&arm_cfft_sR_f32_len512, input_c32, ifftFlag, doBitReverse);
#elif FFT_SIZE == 256
	  arm_cfft_f32(&arm_cfft_sR_f32_len256, input_c32, ifftFlag, doBitReverse);
#else
#warning "Error: NOT supported FFT size"
#endif

	  /* Process the data through the Complex Magnitude Module for calculating the magnitude at each bin */
	  arm_cmplx_mag_f32(input_c32, fftResult, FFT_SIZE);

	  {
		  //correct low frequency bins
		  int i;
		  for (i = 0; i < 8; i++)
		  {
			  fftResult[i] = fftResult[i] * sHanningWindowCoeff[i + 120-1];
		  }
	  }
	  fftResultReady = 0;
	  /* Calculates maxValue and returns corresponding BIN value */
	  arm_max_f32(&fftResult[skipFFTBins], FFT_SIZE / 2 - skipFFTBins, &sFFTmaxValue, &testIndex);
	  arm_min_f32(&fftResult[skipFFTBins], FFT_SIZE / 2 - skipFFTBins, &sFFTminValue, &testIndex);

	  fftResultReady = 1;

	  if(testIndex != refIndex)
		  return 1;			//error

	  return 0;				//no error
  }
}

/**
  * @brief  Display the FFT, depending on the barType - which type we want to display
  * @param  None
  * @retval None
  */
void FFT_DisplayGraph(int ch)
{
	switch (barType)
	{
		case 0 	: FFT_Display_BarGraph(ch); break;
		case 1 	: FFT_Display_Points(ch); break;
		default : FFT_Display_Lines(ch);
	}
}

void FFT_SetBarType(int type)
{
	barType = type;
}

static void FFT_DisplayMagnitude(int ch, float32_t FFTmaxValue, float32_t FFTminValue)
{
	//display magnitude in dB
	BSP_LCD_SetFont(&Font16);
	BSP_LCD_SetBackColor(LCD_COLOR_BLACK);

	if (ch)
		BSP_LCD_SetTextColor(COLOR_RIGHT);
	else
		BSP_LCD_SetTextColor(COLOR_LEFT);
	{
		float32_t dB;
		int dBInt;
		char s[20];

		//display magnitude in dB
		if (FFTmaxValue > 0.00005f)
		{
			dB = sFFTmaxValue / (float32_t)(256.0f);
			dB = log(dB) * 10;
			dBInt = (int)(dB * 100.0);
			sprintf(s, "%4d.%2ddB", dBInt / 100, dBInt % 100);
			if (ch)
				BSP_LCD_DisplayStringAt(FFT_X_DISP, FFT_Y_BORDER_TOP + 50, (uint8_t *)s, LEFT_MODE);
			else
				BSP_LCD_DisplayStringAt(FFT_X_DISP, FFT_Y_BORDER_TOP + 30, (uint8_t *)s, LEFT_MODE);
		}

		//display DNR in dB
		if (FFTminValue  > 0.00001f)
		{
			dB = sFFTmaxValue / sFFTminValue;
			dB = log(dB) * 10;
			dBInt = (int)(dB * 100.0);
			sprintf(s, "%4d.%2ddB", dBInt / 100, dBInt % 100);
			if (ch)
				BSP_LCD_DisplayStringAt(FFT_X_DISP, FFT_Y_BORDER_TOP + 90, (uint8_t *)s, LEFT_MODE);
			else
				BSP_LCD_DisplayStringAt(FFT_X_DISP, FFT_Y_BORDER_TOP + 70, (uint8_t *)s, LEFT_MODE);
		}
	}
}

/**
 * @attention This is just for 256 point FFT (128 bins)
 */
const uint8_t lookupInd[] = {25, 27, 30, 34, 39, 45, 52, 60, 69, 79, 90, 102, 115, 127};

/**
  * @brief  Draw FFT as bar graph
  * @param  ch - the channel 0 or 1
  * @retval None
  */
void FFT_Display_BarGraph(int ch)
{
	float32_t *fft= fftResult;
	int valInd, x;
	int fftVal;
	float fFftVal;
	TS_StateTypeDef tsState;

	//if we push the center - we stop updating FFT
	XXTouch = 0;

	BSP_TS_GetState(&tsState);
	if (tsState.touchDetected)
	{
		if (tsState.touchY[0] > FFT_Y_BORDER_TOP)
		{
			XXTouch = 1;
			if ((tsState.touchX[0] > 100) && (tsState.touchX[0] < (800 - 100)))
			{
				AUDIO_PLAYER_UDANTERestart();
				return;
			}
		}
	}

	x = 16;
	valInd = 0;

	fft 	+= skipFFTBins;
	valInd   = skipFFTBins;

	while (x < 800)
	{
		if (valInd >= (FFT_SIZE / 2 - 1))
			break;

		if (valInd < (lookupInd[0] - 1))
		{
			//the first bins without decimation
			fFftVal = *fft++;
			valInd++;

			//auto-scale to maximal value
			fFftVal *= (480.0f - FFT_Y_BORDER_TOP - FFT_Y_BORDER);
			fFftVal /= sFFTmaxValue;
			fftVal = (int)fFftVal;

			//clear the upper part
			BSP_LCD_SetTextColor(COLOR_INACTIVE);
			BSP_LCD_FillRect(x, FFT_Y_BORDER_TOP, FFT_X_W_RECT, 480 - FFT_Y_BORDER - fftVal);
			//draw the active, lower part
			if (ch)
				BSP_LCD_SetTextColor(COLOR_RIGHT);
			else
				BSP_LCD_SetTextColor(COLOR_LEFT);
			BSP_LCD_FillRect(x, 480 - FFT_Y_BORDER - fftVal, FFT_X_W_RECT, fftVal);

			x += FFT_X_WIDTH;
		}
		else
		{
			//now we iterate with decimation
			uint8_t nextInd;
			size_t i;
			float maxFftVal;

			i = 0;
			while (i < sizeof(lookupInd))
			{
				fFftVal = 0.0f;				//just to avoid warning
				maxFftVal = 0.0f;
				nextInd = lookupInd[i];
				while (valInd < nextInd)
				{
					//decimation
					fFftVal = *fft++;
					if (maxFftVal < fFftVal)
						maxFftVal = fFftVal;
					valInd++;
				}

				//auto-scale to maximal value
				maxFftVal *= (480.0f - FFT_Y_BORDER_TOP - FFT_Y_BORDER);
				maxFftVal /= sFFTmaxValue;
				fftVal = (int)maxFftVal;

				//clear the upper part
				BSP_LCD_SetTextColor(COLOR_INACTIVE);
				BSP_LCD_FillRect(x, FFT_Y_BORDER_TOP, FFT_X_W_RECT, 480 - FFT_Y_BORDER - fftVal);
				//draw the active, lower part
				if (ch)
					BSP_LCD_SetTextColor(COLOR_RIGHT);
				else
					BSP_LCD_SetTextColor(COLOR_LEFT);
				BSP_LCD_FillRect(x, 480 - FFT_Y_BORDER - fftVal, FFT_X_W_RECT, fftVal);

				x += FFT_X_WIDTH;
				i++;
			}
		}
	}

	//check if we have a touch - change the window function for Rectangle (left touch) or Hanning (right touch)
	BSP_TS_GetState(&tsState);
	if (tsState.touchDetected)
	{
		if (tsState.touchY[0] > FFT_Y_BORDER_TOP)
		{
			if (tsState.touchX[0] < 100)
			{
				sHanningWindow = 0;
			}
			if (tsState.touchX[0] > (800 - 100))
			{
				sHanningWindow = 1;
			}
		}
	}

	//display magnitude in dB
	if (ch)
		FFT_DisplayMagnitude(ch, sFFTmaxValue, sFFTminValue);
}

/**
  * @brief  Draw FFT as points
  * @param  ch - the channel 0 or 1
  * @retval None
  */
void FFT_Display_Points(int ch)
{
	float32_t *fft= fftResult;
	int valInd, x;
	int fftVal;
	float fFftVal;
	TS_StateTypeDef tsState;

	//if we push the center - we stop updating FFT
	XXTouch = 0;

	BSP_TS_GetState(&tsState);
	if (tsState.touchDetected)
	{
		if (tsState.touchY[0] > FFT_Y_BORDER_TOP)
		{
			XXTouch = 1;
			if ((tsState.touchX[0] > 100) && (tsState.touchX[0] < (800 - 100)))
			{
				AUDIO_PLAYER_UDANTERestart();
				return;
			}
		}
	}

	x = 16;
	valInd = 0;

	fft 	+= skipFFTBins;
	valInd   = skipFFTBins;

	while (x < 800)
	{
		if (valInd >= (FFT_SIZE / 2 - 1))
			break;

		fFftVal = *fft++;

		//auto-scale to maximal value
		fFftVal *= (480.0f - FFT_Y_BORDER_TOP - FFT_Y_BORDER);
		fFftVal /= sFFTmaxValue;
		fftVal = (int)fFftVal;

		//clear the old pixels
		BSP_LCD_SetTextColor(COLOR_INACTIVE);
		BSP_LCD_FillRect(x, 480 - FFT_Y_BORDER - prevFftY[ch][valInd], X_STEPS-1, X_STEPS-1);

		//draw a rectangle for the FFT value
		if (ch)
			BSP_LCD_SetTextColor(COLOR_RIGHT);
		else
			BSP_LCD_SetTextColor(COLOR_LEFT);
		BSP_LCD_FillRect(x, 480 - FFT_Y_BORDER - fftVal, X_STEPS-1, X_STEPS-1);

		//store the Y coordinate to clear later
		prevFftY[ch][valInd] = fftVal;

		x += X_STEPS;
		valInd++;
	}

	//clear the last
	//BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	//BSP_LCD_FillRect(x, 480 - FFT_Y_BORDER - prevFftY[ch][valInd], X_STEPS-1, X_STEPS-1);

	//check if we have a touch - change the window function for Rectangle (left touch) or Hanning (right touch)
	BSP_TS_GetState(&tsState);
	if (tsState.touchDetected)
	{
		if (tsState.touchY[0] > FFT_Y_BORDER_TOP)
		{
			if (tsState.touchX[0] < 100)
			{
				sHanningWindow = 0;
			}
			if (tsState.touchX[0] > (800 - 100))
			{
				sHanningWindow = 1;
			}
		}
	}

	//display magnitude in dB
	FFT_DisplayMagnitude(ch, sFFTmaxValue, sFFTminValue);
}

/**
  * @brief  Draw FFT as (connected) lines (default)
  * @param  ch - the channel 0 or 1
  * @retval None
  */
void FFT_Display_Lines(int ch)
{
	float32_t *fft = fftResult;
	int valInd, x;
	int fftVal, fftValNext;
	float fFftVal, fFftValNext;
	TS_StateTypeDef tsState;

	//if we push the center - we stop updating FFT
	XXTouch = 0;

	BSP_TS_GetState(&tsState);
	if (tsState.touchDetected)
	{
		if (tsState.touchY[0] > FFT_Y_BORDER_TOP)
		{
			XXTouch = 1;
			if ((tsState.touchX[0] > 100) && (tsState.touchX[0] < (800 - 100)))
			{
				AUDIO_PLAYER_UDANTERestart();
				return;
			}
		}
	}

	x = 16;
	valInd = 0;

	fft 	+= skipFFTBins;
	valInd   = skipFFTBins;

	while (x < (800 - X_STEPS))
	{
		fFftVal 	= *fft++;
		fFftValNext = *fft;

		//auto-scale to maximal value
		fFftVal *= (480.0f - FFT_Y_BORDER_TOP - FFT_Y_BORDER);
		fFftVal /= sFFTmaxValue;
		fftVal = (int)fFftVal;

		fFftValNext *= (480.0f - FFT_Y_BORDER_TOP - FFT_Y_BORDER);
		fFftValNext /= sFFTmaxValue;
		fftValNext = (int)fFftValNext;

		//clear the old line
		BSP_LCD_SetTextColor(COLOR_INACTIVE);
		BSP_LCD_DrawLine(x, 480 - FFT_Y_BORDER - prevFftY[ch][valInd], x + X_STEPS, 480 - FFT_Y_BORDER - prevFftY[ch][valInd + 1]);

		//draw a line for the FFT value
		if (ch)
			BSP_LCD_SetTextColor(COLOR_RIGHT);
		else
			BSP_LCD_SetTextColor(COLOR_LEFT);
		BSP_LCD_DrawLine(x, 480 - FFT_Y_BORDER - fftVal, x + X_STEPS, 480 - FFT_Y_BORDER - fftValNext);

		//store the Y coordinate to clear later
		prevFftY[ch][valInd] = fftVal;

		valInd++;

		if (valInd >= (FFT_SIZE / 2))
			break;

		x += X_STEPS;
	}

	prevFftY[ch][valInd] = fftValNext;

	//check if we have a touch - change the window function for Rectangle (left touch) or Hanning (right touch)
	BSP_TS_GetState(&tsState);
	if (tsState.touchDetected)
	{
		if (tsState.touchY[0] > FFT_Y_BORDER_TOP)
		{
			if (tsState.touchX[0] < 100)
			{
				sHanningWindow = 0;
			}
			if (tsState.touchX[0] > (800 - 100))
			{
				sHanningWindow = 1;
			}
		}
	}

	//display magnitude in dB
	FFT_DisplayMagnitude(ch, sFFTmaxValue, sFFTminValue);
}

 /** \endlink */

