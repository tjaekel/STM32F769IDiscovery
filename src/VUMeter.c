/**
  ******************************************************************************
  * @file    VUMeter.c
  * @author  Torsten Jaekel
  * @version V1.1.0
  * @date    04-September-2016
  * @brief   The VU, Peak Volume, analyzer and display
  ******************************************************************************
  */

#include "VUMeter.h"
#include "AudioPlayer.h"

/* the VU meter variables */
static t_VUparams l_params;
static int16_t last_input[NUM_CHANNELS];

/**
  * @brief  Analyse the peak meter value
  * @param  None
  * @retval None
  */
void __attribute__((section("ITCM_RAM"))) VUMETER_Analyse(void)
{
	int16_t *sampleBuf, sample;
	int bufLen;

	bufLen = AUDIO_PLAYER_GetBuffer((uint8_t **)&sampleBuf);
	if (AUDIO_PLAYER_GetState())
	{
		//second buffer was played - now the first is active
		sampleBuf += bufLen / (2 + sizeof(int16_t));
	}
	else
	{
		//first buffer was played - now the second is active
		////sampleBuf += bufLen / (2 + sizeof(int16_t));
	}

	bufLen /= (2 + sizeof(int16_t));			//in words, for half-buffer
	while (bufLen > 0)
	{
		sample = *sampleBuf++;
		if ( ! (sample & 0x8000))
		{
			if (sample > l_params.vuInput[0])
				l_params.vuInput[0] = sample;
		}
		sample = *sampleBuf++;
		if ( ! (sample & 0x8000))
		{
			if (sample > l_params.vuInput[1])
				l_params.vuInput[1] = sample;
		}
		bufLen -= 2;
	}
}

/**
  * @brief  Analyze the peak meter for the audio input
  * @param  channel - which channel to analyze
  * @retval None
  */
inline void VUMETER_Input(int channel)
{
	if (l_params.vuInput[channel] > last_input[channel])  		// update if higher input level
		last_input[channel] = l_params.vuInput[channel];
	if (l_params.vuInput[channel] > l_params.vuPeak[channel])	// set peak hold
		l_params.vuPeak[channel] = l_params.vuInput[channel];
}

static void __attribute__((section("ITCM_RAM"))) VUMETER_Move(int channel)
{
	uint32_t curTick = HAL_GetTick();
	uint32_t curTickDelta;

	if (curTick > l_params.vuTick[channel] )
		curTickDelta = curTick - l_params.vuTick[channel];
	else
		curTickDelta = l_params.vuTick[channel] - curTick;

	if (curTickDelta > LEVELMOVE)
	{
		l_params.vuInput[channel] -= l_params.vuInput[channel] >> 4;
		l_params.vuTick[channel] = curTick;
	}

	if (curTick > l_params.vuTickPeak[channel])
		curTickDelta = curTick - l_params.vuTickPeak[channel];
	else
		curTickDelta = l_params.vuTickPeak[channel] - curTick;
	if (curTickDelta > PEAKHOLD)		// reset peak hold after 2000ms
	{
		l_params.vuPeak[channel] = l_params.vuInput[channel];
		l_params.vuTickPeak[channel] = curTick;
	}
}

/**
  * @brief  Display the peak meter
  * @param  channel - 0 or 1 for left or right
  * @retval None
  */
void __attribute__((section("ITCM_RAM"))) VUMETER_Display(int channel)
{
	// Scale to make it fit to number of segments
	uint8_t Level = (l_params.vuInput[channel] / (MAX_VALUE / SEGMENTS));
	uint8_t Peak  = (l_params.vuPeak [channel] / (MAX_VALUE / SEGMENTS));
	int i;
	int colSet = 0;

	for (i = 0; i < (SEGMENTS); i++)
	{
		if (Level > i)
		{
			if (i == (SEGMENTS - 3))
			{
				BSP_LCD_SetTextColor(LCD_COLOR_RED);
#if 0
				BSP_LED_On(LED1);
				BSP_LED_On(LED2);
#endif
			}
			else
			if (i == (SEGMENTS - 6))
			{
				BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
#if 0
				BSP_LED_On(LED2);
#endif
			}
			else
			{
#if 0
				BSP_LED_Off(LED1);
				BSP_LED_Off(LED2);
#endif
				if ( ! (colSet & 1))
				{
					BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
					colSet |= 1;
				}
			}
		}
		else
		{
			if ( ! (colSet & 2))
			{
				BSP_LCD_SetTextColor(LCD_COLOR_GRAY);
				colSet |= 2;
			}
		}

		BSP_LCD_FillRect(X_START + i * (X_WIDTH + XY_GAP), Y_START + channel * (Y_WIDTH + XY_GAP), X_WIDTH, Y_WIDTH); // draw rectangles from left to right
	}

	//draw the peak marker
	BSP_LCD_SetTextColor(LCD_COLOR_MAGENTA);
	if (Peak >= SEGMENTS)
		Peak--;
	BSP_LCD_FillRect(X_START + Peak * (X_WIDTH + XY_GAP), Y_START + channel * (Y_WIDTH + XY_GAP), X_WIDTH, Y_WIDTH);  // draw peak value

	//move the peak markers
	VUMETER_Move(channel);
}

void __attribute__((section("ITCM_RAM"))) VUMETER_Volume(int channel, int level)
{
	// Scale to make it fit to number of segments
	uint8_t Level = level / (100 / SEGMENTS);
	int i;
	int colSet = 0;

	for (i = 0; i < (SEGMENTS); i++)
	{
		if (Level > i)
		{
			if (i == (SEGMENTS - 3))
			{
				BSP_LCD_SetTextColor(LCD_COLOR_RED);
#if 0
				BSP_LED_On(LED1);
				BSP_LED_On(LED2);
#endif
			}
			else
			if (i == (SEGMENTS - 6))
			{
				BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
#if 0
				BSP_LED_On(LED2);
#endif
			}
			else
			{
#if 0
				BSP_LED_Off(LED1);
				BSP_LED_Off(LED2);
#endif
				if ( ! (colSet & 1))
				{
					BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
					colSet |= 1;
				}
			}
		}
		else
		{
			if ( ! (colSet & 2))
			{
				BSP_LCD_SetTextColor(LCD_COLOR_GRAY);
				colSet |= 2;
			}
		}

		BSP_LCD_FillRect(X_START + i * (X_WIDTH + XY_GAP), Y_START + channel * (Y_WIDTH + XY_GAP), X_WIDTH, Y_WIDTH); // draw rectangles from left to right
	}
}

void __attribute__((section("ITCM_RAM"))) VUMETER_ProcessVolume(int x)
{
	int vol;

	//x is max. < 760
	//convert x as X touch coordinate into percentage
	//vol = (x * 100) / (SEGMENTS * (X_WIDTH + XY_GAP));
	vol  = x * 100;
	vol /= 760;

	//show volume
	VUMETER_Volume(0, vol);
	VUMETER_Volume(1, vol);

	//set volume
	BSP_AUDIO_OUT_SetVolume(vol);

	gCurVolume = vol;
}
