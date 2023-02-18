/**
  ******************************************************************************
  * @file    SDCard.c
  * @author  Torsten Jaekel
  * @version V1.1.0
  * @date    04-September-2016
  * @brief   The SD Card audio WAV file handling
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "sdcard.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
FATFS SDFatFs;  /* File system object for SD card logical drive */
char SDPath[4]; /* SD card logical drive path */
static uint8_t sSDCardOpen = 0;
static uint8_t sSDCardLinked = 0;

FIL MyFile;     /* File object */

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

#if 0
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int Test_SDCard(void)
{
  FRESULT res;                                          /* FatFs function common result code */
  uint32_t byteswritten, bytesread;                     /* File write/read counts */
  uint8_t wtext[] = "If you see this: this is STM32 working with FatFs"; /* File write buffer */
  uint8_t rtext[100];                                   /* File read buffer */

  /*##-1- Link the micro SD disk I/O driver ##################################*/
  if(sSDCardOpen)
  {
        /*##-4- Create and Open a new text file object with write access #####*/
        if(f_open(&MyFile, "STM32.TXT", FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
        {
        	/* 'STM32.TXT' file Open for write Error */
        	return 3;			//error
        }
        else
        {
        	/*##-5- Write data to the text file ################################*/
        	res = f_write(&MyFile, wtext, sizeof(wtext), (void *)&byteswritten);

        	if((byteswritten == 0) || (res != FR_OK))
        	{
        		/* 'STM32.TXT' file Write or EOF Error */
        		return 4;			//error
        	}
        	else
        	{
        		//*##-6- Close the open text file #################################*/
        		f_close(&MyFile);

        		/*##-7- Open the text file object with read access ###############*/
        		if(f_open(&MyFile, "STM32.TXT", FA_READ) != FR_OK)
        		{
        			/* 'STM32.TXT' file Open for read Error */
        			return 5;			//error
        		}
        		else
        		{
        			/*##-8- Read data from the text file ###########################*/
        			res = f_read(&MyFile, rtext, sizeof(rtext), (UINT*)&bytesread);

        			if((bytesread == 0) || (res != FR_OK))
        			{
        				/* 'STM32.TXT' file Read or EOF Error */
        				return 6;		//error
        			}
        			else
        			{
        				/*##-9- Close the open text file #############################*/
        				f_close(&MyFile);

        				/*##-10- Compare read data with the expected data ############*/
        				if((bytesread != byteswritten))
        				{
        					/* Read data is different from the expected data */
        					return 7;		//error
        				}
        				else
        				{
        					/* Success of the demo: no error occurrence */
        					SDCard_Deinit();
        					return 0;		//no error
        				}
        			}
        		}
        	}
        }
  }

  SDCard_Deinit();

  return 0;
}
#endif

int SDCard_Init(void)
{
	/*##-1- Link the micro SD disk I/O driver ##################################*/
	if(FATFS_LinkDriverEx(&SD_Driver, SDPath, 0) == 0)
	{
		sSDCardLinked = 1;

		/*##-2- Register the file system object to the FatFs module ##############*/
		if(f_mount(&SDFatFs, (TCHAR const*)SDPath, 0) != FR_OK)
			  /* FatFs Initialization Error */
			  return 2;				//error
#if 0
		else
		{
			/*##-3- Create a FAT file system (format) on the logical drive #########*/
			/* WARNING: Formatting the uSD card will delete all content on the device */
			if(f_mkfs((TCHAR const*)SDPath, 0, 0) != FR_OK)
			{
		    	  /* FatFs Format Error */
		    	  return 3;			//error
			}

			sSDCardOpen = 1;
			return 0;				//no error
		}
#endif
	}

	sSDCardOpen = 1;
	return 0;
}

void SDCard_Deinit(void)
{
	if (sSDCardLinked)
	{
		/*##-11- Unlink the micro SD disk I/O driver ###############################*/
		sSDCardLinked = 0;
		sSDCardOpen = 0;
		FATFS_UnLinkDriver(SDPath);
	}
}

int SDCard_GetStatus(void)
{
	if (sSDCardOpen)
		return 1;
	else
		return 0;
}
