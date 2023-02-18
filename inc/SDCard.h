/**
  ******************************************************************************
  * @file    SDCard.h
  * @author  Torsten Jaekel
  * @version V1.1.0
  * @date    04-September-2016
  * @brief   The SD Card WAV file interfaces
  ******************************************************************************
  */

#ifndef SDCARD_H_
#define SDCARD_H_

#include "ff.h"
#include "ff_gen_drv.h"
#include "sd_diskio.h"
#include "main.h"

extern int  SDCard_Init(void);
extern void SDCard_Deinit(void);
extern int  SDCard_GetStatus(void);

//needs SDCard_Init() first
#if 0
extern int Test_SDCard(void);
#endif

#endif /* SDCARD_H_ */
