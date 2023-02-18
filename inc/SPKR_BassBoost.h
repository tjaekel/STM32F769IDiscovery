/**
  ******************************************************************************
  * @file    SPKR_BassBoost.h
  * @author  Torsten Jaekel
  * @version V1.1.0
  * @date    04-September-2016
  * @brief   The aBass Boost filter interface (for small speakers)
  ******************************************************************************
  */

#ifndef SPKR_BASSBOOST_H_
#define SPKR_BASSBOOST_H_

#include "main.h"

void BASS_BOOST_filter(void);								//used for SD Card
void BASS_BOOST_filterBuf(int16_t *outBuf, int bytes);		//used for USB

#endif /* SPKR_BASSBOOST_H_ */
