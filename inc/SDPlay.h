/**
  ******************************************************************************
  * @file    SDPlay.h
  * @author  Torsten Jaekel
  * @version V1.1.0
  * @date    04-September-2016
  * @brief   The SD Card wave file player interfaces
  ******************************************************************************
  */

#ifndef SDPLAY_H_
#define SDPLAY_H_

#include "ff.h"

typedef enum {
	SD_STOPPED,
	SD_STOP,
	SD_STARTPLAY,
	SD_STARTPLAYALL,
	SD_PLAY,
	SD_PLAYALL,
	SD_PAUSE,
	SD_PLAYERROR
} ESDState;

typedef struct {
	char 		files[2*14][_MAX_LFN + 1];
	int  		availFiles;
	int  		actIndex;
	int			prevIndex;
	int  		recX;
	int  		recY;
	ESDState  	state;
	ESDState	prevState;
	FIL			FileDesc;
} TSDPlayMgt;

extern TSDPlayMgt SDPlayObj;

#define MAX_SD_FILES_ROW	14
#define	MAX_SD_FILE_LEN		27

#define SD_START_X			10
#define SD_START_Y			46
#define	SD_MAX_X			800
#define	SD_MAX_Y			480

#define	SD_COL_X			(800/2 - 10)
#define SD_COL_Y			30

#define SD_FONT				Font20

#define	SD_B_PLAY			1
#define	SD_B_PAUSE			2
#define	SD_B_STOP			3
#define	SD_B_EXIT			4

extern void SDPlay_GUIPage(void);
extern void SDPlay_Init(void);
extern int  SDPlay_Run(void);
extern int  SDPlay_ScanFiles(char* rpath);
extern int  SDPlay_FileSelection(int mode);
extern int  SDPlay_FileSelectionNB(void);

#endif /* SDPLAY_H_ */
