/**
  ******************************************************************************
  * @file    AudioPlayer.h
  * @author  Torsten Jaekel
  * @version V1.1.0
  * @date    04-September-2016
  * @brief   The audio player functions
  ******************************************************************************
  */

#ifndef AUDIOPLAYER_H_
#define AUDIOPLAYER_H_

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usbd_audio.h"

/* Exported constants --------------------------------------------------------*/
#define AUDIO_OUT_BUFFER_SIZE               (2 * AUDIO_TOTAL_BUF_SIZE)		//double buffering

#define AUDIO_DEFAULT_VOLUME                80	//50

/* Exported types ------------------------------------------------------------*/
typedef enum {
  BUFFER_OFFSET_NONE = 0,
  BUFFER_OFFSET_HALF,
  BUFFER_OFFSET_FULL,
}BUFFER_StateTypeDef;

/* Audio buffer control struct */
typedef struct {
  uint8_t buff[AUDIO_OUT_BUFFER_SIZE] __attribute__((aligned(4)));
  volatile BUFFER_StateTypeDef state;
  volatile uint32_t fptr;
  volatile int clockState;
}AUDIO_OUT_BufferTypeDef;

typedef enum {
  AUDIO_ERROR_NONE = 0,
  AUDIO_ERROR_IO,
  AUDIO_ERROR_EOF,
  AUDIO_ERROR_INVALID_VALUE,
}AUDIO_ErrorTypeDef;

extern AUDIO_OUT_BufferTypeDef  BufferCtl;
extern int gCurVolume;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
AUDIO_ErrorTypeDef AUDIO_PLAYER_Init(int outSelection, int sampleFreq);
AUDIO_ErrorTypeDef AUDIO_PLAYER_Start(void);
void AUDIO_PLAYER_Restart(void);
AUDIO_ErrorTypeDef AUDIO_PLAYER_Process(void);
AUDIO_ErrorTypeDef AUDIO_PLAYER_Stop(void);
int  AUDIO_PLAYER_GetClock(void);
void AUDIO_PLAYER_ReleaseClock(int trig);
int  AUDIO_PLAYER_GetBuffer(uint8_t **out_BufAddr);
void AUDIO_PLAYER_CpyBuf(uint8_t *buf);
void AUDIO_PLAYER_ClrBuf(void);
void AUDIO_PLAYER_QueueBuf(uint8_t *buf, int len);
int  AUDIO_PLAYER_GetState(void);
void AUDIO_PLAYER_IncHeartbeat(void);
unsigned long AUDIO_PLAYER_GetHeartbeat(void);
void AUDIO_PLAYER_UDANTERestart(void);
void AUDIO_Volume(int upDown);
int AUDIO_PLAYER_GetActiveBuffer(uint8_t **out_BufAddr);
int AUDIO_PLAYER_RunState(void);
int AUDIO_GetVolume(void);

#endif /* AUDIOPLAYER_H_ */
