/**
  ******************************************************************************
  * @file    SPKR_BassBoost.c
  * @author  Torsten Jaekel
  * @version V1.1.0
  * @date    04-September-2016
  * @brief   The internal speaker bass boost filter (for small speakers)
  ******************************************************************************
  */

#include "SPKR_BassBoost.h"

float filterZchR[20] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0, 0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
float filterZchL[20] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0, 0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
float filterCoeff[10] = {
			 //based on: 0..300Hz, 80dB, 48KHz, 19pts but modified coefficients!
		0.000540,	//0.000540,
		0.001364,	//0.001364,
		0.003951,	//0.003951,
		0.007153,	//0.007153,
		0.011637,	//0.011637,
		0.015813,	//0.015813,
		0.019204,	//0.019204,
		0.025396,	//0.025396,
		0.038942,	//0.038942,
		0.048500	//0.048500
};

static void BassBoost_SingleSample(int16_t *chR, int16_t *chL)
{
	float dChR, dChL;
	int32_t lChR, lChL;
	int i;

	for (i = 0; i < 19; i++)
	{
		filterZchR[i] = filterZchR[i + 1];
		filterZchL[i] = filterZchL[i + 1];
	}

	filterZchR[19] = (float)(*chR / 1);
	filterZchL[19] = (float)(*chL / 1);

	dChR = 	filterCoeff[0]*(filterZchR[9]+filterZchR[10]) +
			filterCoeff[1]*(filterZchR[8]+filterZchR[11]) +
			filterCoeff[2]*(filterZchR[7]+filterZchR[12]) +
			filterCoeff[3]*(filterZchR[6]+filterZchR[13]) +
			filterCoeff[4]*(filterZchR[5]+filterZchR[14]) +
			filterCoeff[5]*(filterZchR[4]+filterZchR[15]) +
			filterCoeff[6]*(filterZchR[3]+filterZchR[16]) +
			filterCoeff[7]*(filterZchR[2]+filterZchR[17]) +
			filterCoeff[8]*(filterZchR[1]+filterZchR[18]) +
			filterCoeff[9]*(filterZchR[0]+filterZchR[19]);

	dChL = 	filterCoeff[0]*(filterZchL[9]+filterZchL[10]) +
			filterCoeff[1]*(filterZchL[8]+filterZchL[11]) +
			filterCoeff[2]*(filterZchL[7]+filterZchL[12]) +
			filterCoeff[3]*(filterZchL[6]+filterZchL[13]) +
			filterCoeff[4]*(filterZchL[5]+filterZchL[14]) +
			filterCoeff[5]*(filterZchL[4]+filterZchL[15]) +
			filterCoeff[6]*(filterZchL[3]+filterZchL[16]) +
			filterCoeff[7]*(filterZchL[2]+filterZchL[17]) +
			filterCoeff[8]*(filterZchL[1]+filterZchL[18]) +
			filterCoeff[9]*(filterZchL[0]+filterZchL[19]);

	if (dChR > 0.0)
		lChR = (int32_t)(dChR + 0.5);
	else
		lChR = (int32_t)(dChR - 0.5);
	if (dChL > 0.0)
		lChL = (int32_t)(dChL + 0.5);
	else
		lChL = (int32_t)(dChL - 0.5);

	lChR = 20*(lChR) + filterZchR[0];		//XXXX: 0 or 19 delay tap?
	lChL = 20*(lChL) + filterZchL[0];

	*chR = lChR / 8;
	*chL = lChL / 8;
}

void BASS_BOOST_filterBuf(int16_t *outBuf, int bytes)
{
	while (bytes)
	{
		BassBoost_SingleSample(outBuf, outBuf + 1);
		bytes -= 2*2;
	}
}

void BASS_BOOST_filter(void)
{
	int bufSize;					//bufSize is in bytes
	int16_t *outBuf;				//we use 16bit samples
	int i;

	//get which buffer is active as output
	bufSize = AUDIO_PLAYER_GetActiveBuffer((uint8_t **)&outBuf);

	//now iterate over entire (half) buffer and filter:
	//we can use a state variable filter, or a simple FIR low pass filter, we convert stereo into mono
	//and boost the bass as mono to both channels

#if 0
	//convert into mono
	{
		int32_t sample;
		for (i = 0; i < (bufSize / (2 + 2)); i++)
		{
			sample  = (int32_t)*outBuf;
			sample += (int32_t)*(outBuf + 1);
			sample /= 2;

			//for testing - we convert into mono
			*outBuf = sample;
			*(outBuf + 1) = sample;

			outBuf += 2;
		}
	}
#endif

	extern int XXTouch;

	if ( ! XXTouch)
	for (i = 0; i < (bufSize / (2 + 2)); i++)
	{
		BassBoost_SingleSample(outBuf, outBuf + 1);
		outBuf += 2;
	}
}
