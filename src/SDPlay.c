/**
  ******************************************************************************
  * @file    SDPlay.c
  * @author  Torsten Jaekel
  * @version V1.1.0
  * @date    04-September-2016
  * @brief   The SD Card WAV file player
  ******************************************************************************
  */

#include "SDCard.h"
#include "SDPlay.h"
#include "GUI_pages.h"
//#include <string.h>
#include "usbd_audio.h"						//same size for buffer
//#include "Audio_FiFo.h"

TSDPlayMgt SDPlayObj = {
		.availFiles = 0,
		.actIndex   = -1,
		.prevIndex	= -1,
		.recX       = 0,
		.recY       = 0,
		.state		= SD_STOPPED,
		.prevState  = SD_STOPPED
};

static uint8_t sBuffer[512];				//one SD card sector
uint8_t sSDBuffer[AUDIO_TOTAL_BUF_SIZE];

//forward declaration
static int SDPlay_Stop(void);

void SDPlay_GUIPage(void)
{
	/* set the background layer */
	//BSP_LCD_SelectLayer(0);
	//BSP_LCD_SelectLayer(1);
	BSP_LCD_DrawBitmap(0, 0, (uint8_t *)PAGE_SD);

    /*Refresh the LCD display*/
    HAL_DSI_Refresh(&hdsi_discovery);
}

void SDPlay_Init(void)
{
	//SDPlay_GUIPage();
	SDCard_Init();

	SDPlay_Stop();
	SDPlayObj.availFiles = 0;
	SDPlayObj.actIndex   = -1;
}

static int SDPlay_AnalyseWAVfile(unsigned char *buf, int *wordLen, int *sampleRate, unsigned long *chunks, unsigned long *offset)
{
	unsigned short usVal;
	unsigned long  ulVal;
	int i;
	unsigned char *startBuf;

	startBuf = buf;

	if (strncmp((char *)buf, "RIFF", 4) != 0)
	{
		//Tprintf("*E: not an audio file\n");
		return 0;
	}

	buf += 8;
	if (strncmp((char *)buf, "WAVEfmt ", 8) != 0)
	{
		//Tprintf("*E: not a wave file\n");
		return 0;
	}

	buf += 12;
	usVal = *((unsigned short *)buf);
	if (usVal != 0x1)
	{
		//Tprintf("*E: not a PCM file\n");
		return 0;
	}

	buf += 2;
	usVal = *((unsigned short *)buf);
	if (usVal != 2)
	{
		//Tprintf("*E: not a stereo file\n");
		return 0;
	}

	buf += 2;
	*sampleRate = *((unsigned long *)buf);		//sample rate in Hz

	buf += 10;
	usVal = *((unsigned short *)buf);
	*wordLen = (int)usVal;						//word length, bits for one channel

	buf += 2;
	//find "data"
	for (i = 0; i < 48; i++)
	{
		if (strncmp((char *)buf, "data", 4) == 0)
		{
			//OK, have found "data", get the chunk size
			buf += 4;
			if ((unsigned long)buf & 0x1)
			{
				//Tprintf("*E: data not short aligned\n");
				return 0;
			}

			usVal  = *((unsigned short *)buf);
			buf += 2;
			ulVal  = usVal;
			usVal  = *((unsigned short *)buf);
			ulVal |= usVal << 16;
			buf += 2;

			*offset = (unsigned long)(buf - startBuf);
			*chunks = ulVal;
			return 1;
		}
		buf++;
	}

	//Tprintf("*E: no data found\n");
	return 0;
}

static int SDPlay_StartPlay(void)
{
	uint32_t bytesread;
	FRESULT res;
	int err;
	int wordLen, sampleRate;
	unsigned long chunks, offset;

	if (
		((SDPlayObj.prevState != SD_PAUSE) && (SDPlayObj.prevState != SD_PLAY)) ||
		(SDPlayObj.prevIndex != SDPlayObj.actIndex)
	)
	{
		AUDIO_PLAYER_ClrBuf();
		if (SDPlayObj.prevState != SD_STOPPED)
			f_close(&SDPlayObj.FileDesc);
		SDPlayObj.state = SD_STOPPED;
	}
	else
	{
		SDPlayObj.state 	= SD_PLAY;
		SDPlayObj.prevState = SD_PLAY;

		return 1;
	}

	if (SDPlayObj.actIndex < 0)
	{
		SDPlayObj.state = SD_STOPPED;
		SDPlayObj.prevState = SD_STOPPED;

		return 0;						//error, index is not valid, not yet initialized
	}

	//just if we have a selected file, single file play, take it from files[],
	//open file, decode WAV and prepare for reading PCM samples
	res = f_open(&SDPlayObj.FileDesc, SDPlayObj.files[SDPlayObj.actIndex], FA_READ);
	if (res != FR_OK)
	{
		SDPlayObj.state = SD_STOPPED;
		SDPlayObj.prevState = SD_STOPPED;

		return 0;						//error, cannot open file
	}

	res = f_read(&SDPlayObj.FileDesc, sBuffer, sizeof(sBuffer), (UINT*)&bytesread);
	if((bytesread == 0) || (res != FR_OK))
	{
		f_close(&SDPlayObj.FileDesc);
		SDPlayObj.state = SD_STOPPED;
		SDPlayObj.prevState = SD_STOPPED;

		return 0;						//cannot open file or file is empty, not a WAV file
	}

	err = SDPlay_AnalyseWAVfile(sBuffer, &wordLen, &sampleRate, &chunks, &offset);
	if ( ! err)
	{
		//WAV file format error, close file
		f_close(&SDPlayObj.FileDesc);
		SDPlayObj.state = SD_STOPPED;
		SDPlayObj.prevState = SD_STOPPED;

		return 0;
	}

	//check the sample rate and configure system
	if (sampleRate == 44100)
	{
		SysInterfaces.sampleFreq = sampleRate;
		AUDIO_PLAYER_Init(SysInterfaces.outSelection, sampleRate);
	}
	else
	{
		//48 KHz as default
		SysInterfaces.sampleFreq = sampleRate;
		AUDIO_PLAYER_Init(SysInterfaces.outSelection, sampleRate);
	}

	{
		int actIndex, availFiles;
		actIndex = SDPlayObj.actIndex;
		availFiles = SDPlayObj.availFiles;

		SDPlayObj.actIndex = actIndex;
		SDPlayObj.availFiles = availFiles;
	}

	//check if wordLen is correct
	if (wordLen != 16)
	{
		f_close(&SDPlayObj.FileDesc);
		SDPlayObj.state = SD_STOPPED;
		SDPlayObj.prevState = SD_STOPPED;

		return 0;
	}
	//check if sampleRate is 48 or 44.1 KHz and set config
	if ((sampleRate != 48000) && (sampleRate != 44100))
	{
		f_close(&SDPlayObj.FileDesc);
		SDPlayObj.state = SD_STOPPED;
		SDPlayObj.prevState = SD_STOPPED;

		return 0;
	}

	//OK, now set file pointer to start of PCM data words
	res = f_lseek(&SDPlayObj.FileDesc, (DWORD)offset);
	if (res != FR_OK)
	{
		f_close(&SDPlayObj.FileDesc);
		SDPlayObj.state = SD_STOPPED;
		SDPlayObj.prevState = SD_STOPPED;

		return 0;
	}

	SDPlayObj.state = SD_PLAY;
	SDPlayObj.prevState = SD_PLAY;
	SDPlayObj.prevIndex = SDPlayObj.actIndex;

	return 1;							//no error
}

static int SDPlay_PlaySingle(void)
{
	//hijack the the USB buffer and use the same functions as used for USB
	//to copy the SD Player buffer into output buffer
	uint32_t bytesread = 0;
	FRESULT res = FR_DISK_ERR;

	res = f_read(&SDPlayObj.FileDesc, sSDBuffer, sizeof(sSDBuffer), (UINT*)&bytesread);

	if((bytesread == 0) || (res != FR_OK))
	{
		//file end?
		f_close(&SDPlayObj.FileDesc);
		AUDIO_PLAYER_ClrBuf();
		SDPlayObj.state = SD_STOPPED;
		SDPlayObj.prevState = SD_STOPPED;

		return 0;
	}

	return 1;
}

static int SDPlay_Pause(void)
{
	AUDIO_PLAYER_ClrBuf();
	SDPlayObj.prevState = SD_PAUSE;
	SDPlayObj.state = SD_PAUSE;

	return 1;
}

static int SDPlay_Stop(void)
{
	if (SDPlayObj.state != SD_STOPPED)
	{
		memset(sSDBuffer, 0, sizeof(sSDBuffer));
		AUDIO_PLAYER_ClrBuf();
		SDPlayObj.state = SD_STOPPED;
		f_close(&SDPlayObj.FileDesc);
	}
	SDPlayObj.prevState = SD_STOPPED;

	return 1;							//no error
}

static int SDPlay_StartAll(void)
{
	//TODO
	// for now we ignore
	SDPlayObj.state = SDPlayObj.prevState;

	return 1;
}

int SDPlay_Run(void)
{
	switch (SDPlayObj.state)
	{
		case SD_STARTPLAY 		: return SDPlay_StartPlay();
		case SD_PLAY      		: return SDPlay_PlaySingle();
		case SD_STOP	  		: return SDPlay_Stop();
		case SD_PAUSE	  		: return SDPlay_Pause();
		case SD_STARTPLAYALL 	: return SDPlay_StartAll();
		default			  		: return 0;	//error, unknown state or already stopped
	}
}

int SDPlay_ScanFiles(char* rpath)        				/* Start node to be scanned (also used as work area) */
{
	static char path[60];

    FRESULT res;
    FILINFO fno;
    DIR dir;
    char *fn;   										/* This function assumes non-Unicode configuration */
	static char text[75];								/* static just to avoid to waste stack space */
	int numFiles;

    int x, y;
    strcpy(path, rpath);

    x = SD_START_X;
	y = SD_START_Y;
    numFiles = 0;
#if _USE_LFN
    static char lfn[_MAX_LFN + 1];   					/* Buffer to store the LFN */
    fno.lfname = lfn;
    fno.lfsize = sizeof lfn;
#endif

    if ( ! SDCard_GetStatus())
    {
    	return 0;										/* error, no files */
    }

	BSP_LCD_SetFont(&SD_FONT);							//use smaller font here
	BSP_LCD_SetBackColor(LCD_COLOR_DARKGREEN2);

    res = f_opendir(&dir, path);                       	/* Open the directory */
    if (res == FR_OK)
    {
        //i = strlen(path);
        for (;;)
        {
            res = f_readdir(&dir, &fno);				/* Read a directory item */
            if (res != FR_OK || fno.fname[0] == 0)
            	break;									/* Break on error or end of dir */
            if (fno.fname[0] == '.')
            	continue;								/* Ignore dot entry */
#if _USE_LFN
            fn = *fno.lfname ? fno.lfname : fno.fname;
#else
            fn = fno.fname;
#endif
            //ATT: recursive print for directories - does not work with absolute LCD display positioning and _USE_LFN!
            if (fno.fattrib & AM_DIR)
            {
#if 0
            	/* It is a directory */
                sprintf(&path[i], "/%s", fn);
                res = scan_files(path);
                path[i] = 0;
                if (res != FR_OK)
                	break;
#endif
            }
            else
            {
            	/* It is a file */
            	if (strstr(fn, ".wav"))
            	{
            		sprintf(text, "%s/%s", path, fn);

            		//store the file name in SDPlayObj
            		strcpy(SDPlayObj.files[numFiles], text);

            		if (strlen(text) > MAX_SD_FILE_LEN)
            			text[MAX_SD_FILE_LEN] = '\0';

            		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
             		BSP_LCD_DisplayStringAt(x, y, (uint8_t *)text, LEFT_MODE);

            		y += SD_COL_Y;
            		numFiles++;

            		if (numFiles == (MAX_SD_FILES_ROW * 2))
            			break;

            		if (numFiles == MAX_SD_FILES_ROW)
            		{
            			x = SD_COL_X + SD_START_X;
            			y = SD_START_Y;
            		}
            	}
            }
        }
        f_closedir(&dir);
    }

    //BSP_LCD_SetFont(&LCD_DEFAULT_FONT); // restore font size

    /*Refresh the LCD display*/
    HAL_DSI_Refresh(&hdsi_discovery);

    SDPlayObj.availFiles = numFiles;
    return numFiles;
}
/*
 * Non-blocking file selection for SD Player
 */
int SDPlay_FileSelectionNB(void)
{
	TS_StateTypeDef tsState;
	int x, y;
	int availLines;
	int actIndex;
	int button;

	//draw rectangle around the selected file name
	//if (SDPlayObj.recX)
	//{
	//	BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
	//	BSP_LCD_DrawRect(SDPlayObj.recX, SDPlayObj.recY, SD_COL_X, SD_COL_Y);
	//}

	button = 0;
	//get touch screen button
		if (SDPlayObj.availFiles)
		{
			BSP_TS_GetState(&tsState);
			//check which file we have touched
			if (tsState.touchDetected)
			{
				if (tsState.touchX[0] > SD_START_X && tsState.touchX[0] < (SD_MAX_X - SD_START_X) && tsState.touchY[0] > SD_START_Y && tsState.touchY[0] < SD_MAX_Y)
				{
					if (tsState.touchX[0] < SD_COL_X)
					{
						if (SDPlayObj.availFiles > MAX_SD_FILES_ROW)
							availLines = MAX_SD_FILES_ROW;
						else
							availLines = SDPlayObj.availFiles;
						x = SD_START_X / 2;
					}
					else
					{
						x = SD_COL_X + SD_START_X;
						if (SDPlayObj.availFiles > MAX_SD_FILES_ROW)
							availLines = SDPlayObj.availFiles - MAX_SD_FILES_ROW;								//the half due to two columns
						else
							availLines = 0;
					}
					y = tsState.touchY[0];
					y += SD_COL_Y - 1;
					y /= SD_COL_Y;

					if (availLines > (y - 2))
					{
						actIndex = y - 1;
						if (x > (SD_START_X / 2))
							actIndex += MAX_SD_FILES_ROW;

						y *= SD_COL_Y;
						y -= 21;

						if ((SDPlayObj.recX != x) || (SDPlayObj.recY != y))
						{
							//clear the recent green rectangle
							if (SDPlayObj.recX)
							{
								BSP_LCD_SetTextColor(LCD_COLOR_DARKGREEN2);
								BSP_LCD_DrawRect(SDPlayObj.recX, SDPlayObj.recY, SD_COL_X, SD_COL_Y);
							}

							//draw rectangle around the selected file name
							BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
							BSP_LCD_DrawRect(x, y, SD_COL_X, SD_COL_Y);
							SDPlayObj.recX = x;
							SDPlayObj.recY = y;

							SDPlayObj.actIndex = actIndex - 1;
						}
					}
				} //end if

				//check here if play button, dismiss ... pressed
				if (tsState.touchY[0] < SD_START_Y)
				{
					if ((tsState.touchX[0] > 230) && (tsState.touchX[0] < 340))
					{
						button = SD_B_PLAY;
					}
					if ((tsState.touchX[0] > 340) && (tsState.touchX[0] < 450))
					{
						button = SD_B_PAUSE;
					}
					if ((tsState.touchX[0] > 450) && (tsState.touchX[0] < 560))
					{
						button = SD_B_STOP;
					}
					if (tsState.touchX[0] > 760)
					{
						button = SD_B_EXIT;
					}
				}
			    /*Refresh the LCD display*/
			    HAL_DSI_Refresh(&hdsi_discovery);
			} //end if
		} //end if

	//check if new opened - clear the selection rectangle
	if (SDPlayObj.actIndex == -1)
	{
		if (SDPlayObj.recX)
		{
			BSP_LCD_SetTextColor(LCD_COLOR_DARKGREEN2);
			BSP_LCD_DrawRect(SDPlayObj.recX, SDPlayObj.recY, 220, 15);
		}

		SDPlayObj.recX = 0;
	}

	//check if control buttons are pressed
	if (button == SD_B_PLAY)
	{
		//start (single file) pressed
		SDPlayObj.state = SD_STARTPLAY;
		SDPlay_StartPlay();
	}

	if (button == SD_B_PAUSE)
	{
		//pause pressed
		SDPlay_Pause();
	}

	if (button == SD_B_STOP)
	{
		//stop pressed
		SDPlay_Stop();

		//clear the recent green rectangle
		if (SDPlayObj.recX)
		{
			BSP_LCD_SetTextColor(LCD_COLOR_DARKGREEN2);
			BSP_LCD_DrawRect(SDPlayObj.recX, SDPlayObj.recY, 220, 15);
		}

		SDPlayObj.recX = 0;

		SDPlayObj.actIndex = -1;
	}

	if (button == SD_B_EXIT) 				// done, return to main menu
	{
	}

	return button;
}

/*
 * Blocking (with mode = 0) or Non-blocking (mode = 1) file selection
 * for SD Player, mode controls also if we can use Exit button (if blocking then we stay in it)
 */
int SDPlay_FileSelection(int mode)
{
	/*
	 * SDCard File Page displayed
	 */

	TS_StateTypeDef tsState;
	int x, y;
	int availLines;
	int actIndex;
	int button;

	/* clear display and show GUI page */
	SDPlay_GUIPage();

	SDPlay_ScanFiles("");

	//draw rectangle around the selected file name
	if (SDPlayObj.recX)
	{
		BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
		BSP_LCD_DrawRect(SDPlayObj.recX, SDPlayObj.recY, SD_COL_X, SD_COL_Y);
	}

	button = 0;
	//get touch screen button
	while (1)
	{
		if (SDPlayObj.availFiles)
		{
			BSP_TS_GetState(&tsState);
			//check which file we have touched
			if (tsState.touchDetected)
			{
				if (tsState.touchX[0] > SD_START_X && tsState.touchX[0] < (SD_MAX_X - SD_START_X) && tsState.touchY[0] > SD_START_Y && tsState.touchY[0] < SD_MAX_Y)
				{
					if (tsState.touchX[0] < SD_COL_X)
					{
						if (SDPlayObj.availFiles > MAX_SD_FILES_ROW)
							availLines = MAX_SD_FILES_ROW;
						else
							availLines = SDPlayObj.availFiles;
						x = SD_START_X / 2;
					}
					else
					{
						x = SD_COL_X + SD_START_X;
						if (SDPlayObj.availFiles > MAX_SD_FILES_ROW)
							availLines = SDPlayObj.availFiles - MAX_SD_FILES_ROW;								//the half due to two columns
						else
							availLines = 0;
					}
					y = tsState.touchY[0];
					y += SD_COL_Y - 1;
					y /= SD_COL_Y;

					if (availLines > (y - 2))
					{
						actIndex = y - 1;
						if (x > (SD_START_X / 2))
							actIndex += MAX_SD_FILES_ROW;

						y *= SD_COL_Y;
						y -= 21;

						if ((SDPlayObj.recX != x) || (SDPlayObj.recY != y))
						{
							//clear the recent green rectangle
							if (SDPlayObj.recX)
							{
								BSP_LCD_SetTextColor(LCD_COLOR_DARKGREEN2);
								BSP_LCD_DrawRect(SDPlayObj.recX, SDPlayObj.recY, SD_COL_X, SD_COL_Y);
							}

							//draw rectangle around the selected file name
							BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
							BSP_LCD_DrawRect(x, y, SD_COL_X, SD_COL_Y);
							SDPlayObj.recX = x;
							SDPlayObj.recY = y;

							SDPlayObj.actIndex = actIndex - 1;
						}
					}
				} //end if

				//check here if play button, dismiss ... pressed
				if (tsState.touchY[0] < SD_START_Y)
				{
					if ((tsState.touchX[0] > 230) && (tsState.touchX[0] < 340))
					{
						button = SD_B_PLAY;
						break;
					}
					if ((tsState.touchX[0] > 340) && (tsState.touchX[0] < 450))
					{
						button = SD_B_PAUSE;
						break;
					}
					if ((tsState.touchX[0] > 450) && (tsState.touchX[0] < 560))
					{
						button = SD_B_STOP;
						break;
					}
					if (mode)
					{
						//exit just allowed if non-blocking - otherwise ignore exit
						if (tsState.touchX[0] > 650)
						{
							button = SD_B_EXIT;
							break;
						}
					}
				}
			    /*Refresh the LCD display*/
			    HAL_DSI_Refresh(&hdsi_discovery);
			} //end if
		}
		else
			break;		//error, no files
		if (mode)
			break;
	} //end while

	//check if new opened - clear the selection rectangle
	if (SDPlayObj.actIndex == -1)
	{
		if (SDPlayObj.recX)
		{
			BSP_LCD_SetTextColor(LCD_COLOR_DARKGREEN2);
			BSP_LCD_DrawRect(SDPlayObj.recX, SDPlayObj.recY, 220, 15);
		}

		SDPlayObj.recX = 0;
	}

	//check if control buttons are pressed
	if (button == SD_B_PLAY)
	{
		//start (single file) pressed
		SDPlayObj.state = SD_STARTPLAY;
		SDPlay_StartPlay();
	}

	if (button == SD_B_PAUSE)
	{
		//pause pressed
		SDPlayObj.state = SD_PAUSE;
	}

	if (button == SD_B_STOP)
	{
		SDPlay_Stop();

		//stop pressed
		SDPlayObj.state = SD_STOP;
		//clear the recent green rectangle
		if (SDPlayObj.recX)
		{
			BSP_LCD_SetTextColor(LCD_COLOR_DARKGREEN2);
			BSP_LCD_DrawRect(SDPlayObj.recX, SDPlayObj.recY, 220, 15);
		}

		SDPlayObj.recX = 0;

		SDPlayObj.actIndex = -1;
	}

	if (button == SD_B_EXIT) 				// done, return to main menu
	{
	}

	return button;
}
