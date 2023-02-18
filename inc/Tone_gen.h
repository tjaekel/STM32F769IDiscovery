/**
  ******************************************************************************
  * @file    Tone_gen.h
  * @author  Torsten Jaekel
  * @version V1.1.0
  * @date    04-September-2016
  * @brief   The interface for the test tone generator
  ******************************************************************************
  */

#ifndef TONE_GEN_H_
#define TONE_GEN_H_

#include "main.h"

void TONEGEN_CpySine(void);
int  TONEGEN_GetSine(const int16_t **buf);

#endif /* TONE_GEN_H_ */
