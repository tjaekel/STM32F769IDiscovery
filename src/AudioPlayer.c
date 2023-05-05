/**
  ******************************************************************************
  * @file    AudioPlayer.c
  * @author  Torsten Jaekel
  * @version V1.1.0
  * @date    04-September-2016
  * @brief   The audio player functions
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "AudioPlayer.h"

extern SemaphoreHandle_t xSemaphoreAudio;

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/*static*/ AUDIO_OUT_BufferTypeDef  BufferCtl __attribute__((section("DTCM")));
static unsigned long sAudioHeartbeat = 0;

int gCurVolume = AUDIO_DEFAULT_VOLUME;					//the start volume

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

int __attribute__((section("ITCM_RAM"))) AUDIO_PLAYER_GetClock(void)
{
	//we will block here for max. 200 msec if we do not have an audio clock, so the GUI remains a bit responsive
	if (xSemaphoreTake(xSemaphoreAudio, 200) == pdTRUE)
	{
		BufferCtl.clockState = 0;
		return 1;
	}
	else
	{
		taskYIELD();
		return 0;
	}
}

void __attribute__((section("ITCM_RAM"))) AUDIO_PLAYER_ReleaseClock(int trig)
{
	static BaseType_t xHigherPriorityTaskWoken;

	//called from ISR - INT should be disabled !
	BufferCtl.clockState = trig;

	xHigherPriorityTaskWoken = pdFALSE;
	xSemaphoreGiveFromISR(xSemaphoreAudio, &xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

inline int AUDIO_PLAYER_GetState(void)
{
	if (BufferCtl.state == BUFFER_OFFSET_HALF)
		return 0;
	else
		return 1;
}

inline int AUDIO_PLAYER_GetBuffer(uint8_t **out_BufAddr)
{
	*out_BufAddr = BufferCtl.buff;
	return sizeof(BufferCtl.buff);
}

int __attribute__((section("ITCM_RAM"))) AUDIO_PLAYER_GetActiveBuffer(uint8_t **out_BufAddr)
{
	if (BufferCtl.state == BUFFER_OFFSET_HALF)
	{
		//first half buffer was played - return the other, first as active buffer - UFFER_OFFSET_HALF
		*out_BufAddr = BufferCtl.buff;
	}
	else
	{
		//second half buffer was played - return the second, active buffer - BUFFER_OFFSET_FULL
		*out_BufAddr = BufferCtl.buff + (sizeof(BufferCtl.buff) / 2);
		////*out_BufAddr = &BufferCtl.buff[AUDIO_TOTAL_BUF_SIZE];
	}

	return (sizeof(BufferCtl.buff) / 2);		//in bytes, half size of Double Buffer
	////return AUDIO_TOTAL_BUF_SIZE;
}

/**
  * @brief  Initializes Audio Interface
  * @param  None
  * @retval Audio error
  */
AUDIO_ErrorTypeDef AUDIO_PLAYER_Init(int outSelection, int sampleFreq)
{
  if (outSelection)
  {
	  if(BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_HEADPHONE, gCurVolume, sampleFreq) == 0)
	  {
		  BSP_AUDIO_OUT_SetAudioFrameSlot(CODEC_AUDIOFRAME_SLOT_02);
		  return AUDIO_ERROR_NONE;
	  }
	  else
	  {
		  return AUDIO_ERROR_IO;
	  }
  }
  else
  {
	  if(BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_SPEAKER, gCurVolume, sampleFreq) == 0)
	  {
		  BSP_AUDIO_OUT_SetAudioFrameSlot(CODEC_AUDIOFRAME_SLOT_13);
		  return AUDIO_ERROR_NONE;
	  }
	  else
	  {
		  return AUDIO_ERROR_IO;
	  }
  }
}

/**
  * @brief  Starts Audio streaming.
  * @param  idx: File index
  * @retval Audio error
  */
AUDIO_ErrorTypeDef AUDIO_PLAYER_Start(void)
{
	BSP_AUDIO_OUT_Play((uint16_t*)&BufferCtl.buff[0], AUDIO_OUT_BUFFER_SIZE);
	BufferCtl.clockState = 0;
    return AUDIO_ERROR_NONE;
}

void AUDIO_PLAYER_Restart(void)
{
	BSP_AUDIO_OUT_Restart((uint16_t*)&BufferCtl.buff[0], AUDIO_OUT_BUFFER_SIZE);
	BufferCtl.clockState = 0;
	BufferCtl.state = 0;
}

/**
  * @brief  Manages Audio process.
  * @param  None
  * @retval Audio error
  */
inline AUDIO_ErrorTypeDef AUDIO_PLAYER_Process(void)
{
  return AUDIO_ERROR_NONE;
}

/**
  * @brief  Stops Audio streaming.
  * @param  None
  * @retval Audio error
  */
AUDIO_ErrorTypeDef AUDIO_PLAYER_Stop(void)
{
  BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW);
  return AUDIO_ERROR_NONE;
}

/**
  * @brief  Calculates the remaining file size and new position of the pointer.
  * @param  None
  * @retval None
  */
void __attribute__((section("ITCM_RAM"))) BSP_AUDIO_OUT_TransferComplete_CallBack(void)
{
    BufferCtl.state = BUFFER_OFFSET_FULL;
    AUDIO_PLAYER_ReleaseClock(2);
}

/**
  * @brief  Manages the DMA Half Transfer complete interrupt.
  * @param  None
  * @retval None
  */
void __attribute__((section("ITCM_RAM"))) BSP_AUDIO_OUT_HalfTransfer_CallBack(void)
{
    BufferCtl.state = BUFFER_OFFSET_HALF;
    AUDIO_PLAYER_ReleaseClock(1);
}

/**
  * @brief  Copy USB or SD Player input buffer to SAI output buffer
  * @param  buf - pointer to the USB input samples
  * @retval None
  */
void __attribute__((section("ITCM_RAM"))) AUDIO_PLAYER_CpyBuf(uint8_t *buf)
{
	if (BufferCtl.state == BUFFER_OFFSET_FULL)
	{
		fast_memcpy(&BufferCtl.buff[AUDIO_TOTAL_BUF_SIZE], buf, AUDIO_TOTAL_BUF_SIZE);
	}
	else
	{
		fast_memcpy(&BufferCtl.buff[0], buf, AUDIO_TOTAL_BUF_SIZE);
	}

	AUDIO_PLAYER_IncHeartbeat();
}

/**
  * @brief  Clear SAI output buffer, used when we do not have valid audio
  * @retval None
  */
inline void AUDIO_PLAYER_ClrBuf(void)
{
	memset(&BufferCtl.buff[0], 0, sizeof(BufferCtl.buff));
}

static int initDone = 0;
static int oIdx = 0;

/**
  * @brief  Put the Network Audio samples into SAI output buffer
  * @param  buf - the pointer to the Audio Network samples
  * @param  len - the length of the Audio Samples, in bytes
  * @retval None
  */
void __attribute__((section("ITCM_RAM"))) AUDIO_PLAYER_QueueBuf(uint8_t *buf, int len)
{
	//just to make sure:
	if ((size_t)(oIdx + len) <= sizeof(BufferCtl.buff))
	{
		fast_memcpy(&BufferCtl.buff[oIdx], buf, len);
		oIdx += len;
	}
	else
	{
		//an error, what to do?
		oIdx = 0;
		return;
	}

	if ( ! initDone)
	{
		//tricky: how to restart audio out so that it is in sync?
		//this seems to work the best way, but not always, we need a new reset and restart
		if ((size_t)oIdx >= (sizeof(BufferCtl.buff) / 2 + 192))
		{
			AUDIO_PLAYER_Restart();
			initDone = 1;
		}
	}

	if ((size_t)oIdx >= sizeof(BufferCtl.buff))
		oIdx = 0;

	AUDIO_PLAYER_IncHeartbeat();
}

/**
  * @brief  Restart and sync audio for Network Audio
  * @param  None
  * @retval None
  */
void AUDIO_PLAYER_UDANTERestart(void)
{
	initDone = 0;
	oIdx = 0;
}

/**
  * @brief  Get the counter (heartbeat) if we have audio reception
  * @param  None
  * @retval ulong - the audio reception counter
  */
inline unsigned long AUDIO_PLAYER_GetHeartbeat(void)
{
	unsigned long x;
	x = sAudioHeartbeat;
	sAudioHeartbeat = 0;

#if 0
	/* TODO it must be supported for all input devices, e.g. also SPDIF Rx ! */
	//if no heartbeat - clear the buffer
	if ( ! x)
		memset(BufferCtl.buff, 0x00, sizeof(BufferCtl.buff));
#endif

	return x;
}

/**
  * @brief  Increment the audio reception counter (heartbeat)
  * @param  None
  * @retval None
  */
inline void AUDIO_PLAYER_IncHeartbeat(void)
{
	sAudioHeartbeat++;
}

void __attribute__((section("ITCM_RAM"))) AUDIO_Volume(int upDown)
{
	/* 1 : increase volume, in steps of 10
	 * 0 : decrease volume, in steps of 10
	 */
	int vol;

	vol  = gCurVolume;

	if (upDown == 0)
	{
		if (vol > 10)
			vol -= 10;
		else
			vol = 0;
	}
	else
	{
		if (vol < 90)
			vol += 10;
		else
			vol = 100;
	}

	BSP_AUDIO_OUT_SetVolume(vol);
	gCurVolume = vol;
}

/*******************************************************************************
                            Static Functions
*******************************************************************************/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
