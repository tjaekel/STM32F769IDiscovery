/**
  ******************************************************************************
  * @file    SPDIF_Tx.c
  * @author  Torsten Jaekel
  * @version V1.1.0
  * @date    04-September-2016
  * @brief   The S/PDIF Tx on STM board
  ******************************************************************************
  */

#include "main.h"
#include "AudioPlayer.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

SAI_HandleTypeDef            spdiftxhsai;

static uint8_t SPDIF_TX_buf[AUDIO_OUT_BUFFER_SIZE];

/**
  * @brief  Initialize the SPDIF Tx transmission, forward as SPDIF out
  * @param  None
  * @retval None
  */
void SPDIF_TX_Init(void)
{
	/* Configure SAI in SPDIF-Tx Mode */
	spdiftxhsai.Instance 		    		= SAI2_Block_A;
	spdiftxhsai.Init.AudioMode      		= SAI_MODEMASTER_TX;
	spdiftxhsai.Init.Synchro        		= SAI_ASYNCHRONOUS;
	spdiftxhsai.Init.SynchroExt     		= SAI_SYNCEXT_DISABLE;
	spdiftxhsai.Init.OutputDrive    		= SAI_OUTPUTDRIVE_DISABLE;
	spdiftxhsai.Init.NoDivider      		= SAI_MASTERDIVIDER_ENABLE;
	spdiftxhsai.Init.FIFOThreshold  		= SAI_FIFOTHRESHOLD_EMPTY;
	spdiftxhsai.Init.AudioFrequency 		= SAI_AUDIO_FREQUENCY_96K; //WHY????
	spdiftxhsai.Init.Mckdiv         		= 0;
	spdiftxhsai.Init.MonoStereoMode 		= SAI_STEREOMODE;
	spdiftxhsai.Init.CompandingMode 		= SAI_NOCOMPANDING;
	spdiftxhsai.Init.TriState       		= SAI_OUTPUT_NOTRELEASED;
	spdiftxhsai.Init.Protocol       		= SAI_SPDIF_PROTOCOL;
	spdiftxhsai.Init.DataSize       		= SAI_DATASIZE_16;
	spdiftxhsai.Init.FirstBit       		= SAI_FIRSTBIT_MSB;
	spdiftxhsai.Init.ClockStrobing  		= SAI_CLOCKSTROBING_FALLINGEDGE;

	spdiftxhsai.FrameInit.FrameLength       = 64;
	spdiftxhsai.FrameInit.ActiveFrameLength = 32;
	spdiftxhsai.FrameInit.FSDefinition      = SAI_FS_STARTFRAME;
	spdiftxhsai.FrameInit.FSPolarity        = SAI_FS_ACTIVE_LOW;
	spdiftxhsai.FrameInit.FSOffset          = SAI_FS_FIRSTBIT;

	spdiftxhsai.SlotInit.FirstBitOffset 	= 0;
	spdiftxhsai.SlotInit.SlotSize       	= SAI_SLOTSIZE_DATASIZE;
	spdiftxhsai.SlotInit.SlotNumber     	= 4;
	spdiftxhsai.SlotInit.SlotActive     	= CODEC_AUDIOFRAME_SLOT_0123;	//0x0000000F; SAI_SLOTACTIVE_ALL;

	if(HAL_SAI_Init(&spdiftxhsai)!= HAL_OK )
	{
	    return;
	}
}

/**
  * @brief  Start the SPDIF Tx transmission, output
  * @param  None
  * @retval None
  */
void SPDIF_TX_Start(void)
{
	/* Start transmission (DMA mode) */
	HAL_SAI_Transmit_DMA(&spdiftxhsai, (uint8_t *)SPDIF_TX_buf, sizeof(SPDIF_TX_buf) / sizeof(uint16_t));
}

void __attribute__((section("ITCM"))) SPDIF_Tx_CpyBuf(uint8_t *dst, uint8_t *src, int len)
{
	while (len)
	{
		*dst++ = *(src + 1);
		*dst++ = *(src + 0);
		src += 2;
		len -= 2;
	}
}

volatile int SPDIFtxBuf = 0;

/**
  * @brief  ISR Handler for the SPDIF Tx (second buffer full)
  * @param  None
  * @retval None
  */
void SPDIF_TX_TransferComplete_CallBack(void)
{
#if 0
	uint8_t *buf, *outBuf;
	int bufLen;

	bufLen = AUDIO_PLAYER_GetBuffer(&buf);

	if (AUDIO_PLAYER_GetState() == 0)
		outBuf = buf;
	else
		outBuf = buf + bufLen / 2;

	SPDIF_Tx_CpyBuf(&SPDIF_TX_buf[AUDIO_TOTAL_BUF_SIZE], outBuf, bufLen / 2);
#else
	SPDIFtxBuf = 1;
#endif
}

/**
  * @brief  ISR Handler for the SPDIF Tx (first buffer full)
  * @param  None
  * @retval None
  */
void SPDIF_TX_HalfTransfer_CallBack(void)
{
#if 0
	uint8_t *buf, *outBuf;
	int bufLen;

	bufLen = AUDIO_PLAYER_GetBuffer(&buf);

	if (AUDIO_PLAYER_GetState() == 0)
		outBuf = buf;
	else
		outBuf = buf + bufLen / 2;

	SPDIF_Tx_CpyBuf(&SPDIF_TX_buf[0], outBuf, bufLen / 2);
#else
	SPDIFtxBuf = 0;
#endif
}

void SPDIF_TX_OutBuf(void)
{
	uint8_t *buf, *outBuf;
	int bufLen;

	bufLen = AUDIO_PLAYER_GetBuffer(&buf);

	if (AUDIO_PLAYER_GetState() == 0)
		outBuf = buf;
	else
		outBuf = buf + bufLen / 2;

	if (SPDIFtxBuf)
		SPDIF_Tx_CpyBuf(&SPDIF_TX_buf[AUDIO_TOTAL_BUF_SIZE], outBuf, bufLen / 2);
	else
		SPDIF_Tx_CpyBuf(&SPDIF_TX_buf[0], outBuf, bufLen / 2);
}
