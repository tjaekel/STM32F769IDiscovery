/**
  ******************************************************************************
  * @file    SRC_i2s_in.c
  * @author  Torsten Jaekel
  * @version V1.1.0
  * @date    04-September-2016
  * @brief   The I2S digital audio input from DIGI-FP (for Lyrebird APP Ultra)
  ******************************************************************************
  */

#include "main.h"
#include "AudioPlayer.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

SAI_HandleTypeDef               haudio_in_src_sai;

/*
 * TODO why these callbacks are never called?
 */
void SRC_RX_TransferComplete_CallBack(struct __DMA_HandleTypeDef * hdma);
void SRC_RX_HalfTransfer_CallBack(struct __DMA_HandleTypeDef * hdma);

/**
  * @brief  Initializes SAI Audio IN MSP for DIGI-FP I2S input from interfaces
  * @param  hsai: SAI handle
  * @retval None
  */
static void SAI_SRC_IN_MspInit(SAI_HandleTypeDef *hsai, void *Params)
{
  (void)Params;
  static DMA_HandleTypeDef hdma_sai_src_rx;
  GPIO_InitTypeDef  gpio_init_structure;

  /* Enable SAI clock */
  AUDIO_IN_SAI_SRC_CLK_ENABLE();

  /* Enable SD GPIO clock */
  AUDIO_IN_SAI_SRC_SD_ENABLE();
  /* CODEC_SAI pin configuration: SD pin */
  gpio_init_structure.Pin = AUDIO_IN_SAI_SRC_SD_PIN;
  gpio_init_structure.Mode = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull = GPIO_NOPULL;
  gpio_init_structure.Speed = GPIO_SPEED_FAST;
  gpio_init_structure.Alternate = AUDIO_IN_SAI_SRC_AF;
  HAL_GPIO_Init(AUDIO_IN_SAI_SRC_SD_GPIO_PORT, &gpio_init_structure);

  /* Enable the DMA clock */
  AUDIO_IN_SAI_SRC_DMAx_CLK_ENABLE();

  if(hsai->Instance == AUDIO_IN_SAI_SRC)
  {
    /* Configure the hdma_sai_rx handle parameters */
    hdma_sai_src_rx.Init.Channel             = AUDIO_IN_SAI_SRC_DMAx_CHANNEL;
    hdma_sai_src_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    hdma_sai_src_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
    hdma_sai_src_rx.Init.MemInc              = DMA_MINC_ENABLE;
    hdma_sai_src_rx.Init.PeriphDataAlignment = AUDIO_IN_SAI_SRC_DMAx_PERIPH_DATA_SIZE;
    hdma_sai_src_rx.Init.MemDataAlignment    = AUDIO_IN_SAI_SRC_DMAx_MEM_DATA_SIZE;
    hdma_sai_src_rx.Init.Mode                = DMA_CIRCULAR;
    hdma_sai_src_rx.Init.Priority            = DMA_PRIORITY_HIGH;
    hdma_sai_src_rx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
    hdma_sai_src_rx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
    hdma_sai_src_rx.Init.MemBurst            = DMA_MBURST_INC4;
    hdma_sai_src_rx.Init.PeriphBurst         = DMA_PBURST_INC4;

    hdma_sai_src_rx.Instance = AUDIO_IN_SAI_SRC_DMAx_STREAM;

    hdma_sai_src_rx.XferCpltCallback = SRC_RX_TransferComplete_CallBack;
    hdma_sai_src_rx.XferHalfCpltCallback = SRC_RX_HalfTransfer_CallBack;

    /* Associate the DMA handle */
    __HAL_LINKDMA(hsai, hdmarx, hdma_sai_src_rx);

    /* Deinitialize the Stream for new transfer */
    HAL_DMA_DeInit(&hdma_sai_src_rx);

    /* Configure the DMA Stream */
    HAL_DMA_Init(&hdma_sai_src_rx);
  }

  /* SAI DMA IRQ Channel configuration */
  HAL_NVIC_SetPriority(AUDIO_IN_SAI_SRC_DMAx_IRQ, AUDIO_IN_SRC_IRQ_PREPRIO, 0);
  HAL_NVIC_EnableIRQ(AUDIO_IN_SAI_SRC_DMAx_IRQ);
}

#if 0
/**
  * @brief  De-Initializes SAI Audio IN MSP.
  * @param  hsai: SAI handle
  * @retval None
  */
static void SAI_SRC_IN_MspDeInit(SAI_HandleTypeDef *hsai, void *Params)
{
  GPIO_InitTypeDef  gpio_init_structure;

  /* SAI DMA IRQ Channel deactivation */
  HAL_NVIC_DisableIRQ(AUDIO_IN_SAI_SRC_DMAx_IRQ);

  if(hsai->Instance == AUDIO_IN_SAI_SRC)
  {
    /* Deinitialize the DMA stream */
    HAL_DMA_DeInit(hsai->hdmarx);
  }

  /* Disable SAI peripheral */
  __HAL_SAI_DISABLE(hsai);

  /* Deactivates CODEC_SAI pin SD by putting them in input mode */
  gpio_init_structure.Pin = AUDIO_IN_SAI_SRC_SD_PIN;
  HAL_GPIO_DeInit(AUDIO_IN_SAI_SRC_SD_GPIO_PORT, gpio_init_structure.Pin);

  /* Disable SAI clock */
  AUDIO_IN_SAI_SRC_CLK_DISABLE();
}
#endif

/**
  * @brief  Initializes the Audio Codec audio interface (SAI) for the I2S input from SRC, DIGI-FP
  * @param  AudioFreq: Audio frequency to be configured for the SAI peripheral.
  * @note   The default SlotActive configuration is set to CODEC_AUDIOFRAME_SLOT_0123
  *         and user can update this configuration using
  * @retval None
  */
static void SAI_SRC_IN_Init(uint32_t AudioFreq)
{
  /* Initialize SAI2 block A in SLAVE RX synchronous from SAI1 block A */
  /* Initialize the haudio_in_sai Instance parameter */
  ////haudio_in_src_sai.Instance = AUDIO_IN_SAI_SRC;

  /* Disable SAI peripheral to allow access to SAI internal registers */
  __HAL_SAI_DISABLE(&haudio_in_src_sai);

  /* Configure SAI_Block_x
  LSBFirst: Disabled
  DataSize: 16
  synchronized with SAI1 */
  haudio_in_src_sai.Init.MonoStereoMode 		= SAI_STEREOMODE;
  haudio_in_src_sai.Init.AudioFrequency 		= AudioFreq;
  haudio_in_src_sai.Init.AudioMode      		= SAI_MODESLAVE_RX;
  haudio_in_src_sai.Init.NoDivider      		= SAI_MASTERDIVIDER_ENABLE;
  haudio_in_src_sai.Init.Protocol       		= SAI_FREE_PROTOCOL;
  haudio_in_src_sai.Init.DataSize       		= SAI_DATASIZE_16;
  haudio_in_src_sai.Init.FirstBit       		= SAI_FIRSTBIT_MSB;
  haudio_in_src_sai.Init.ClockStrobing  		= SAI_CLOCKSTROBING_RISINGEDGE;
  haudio_in_src_sai.Init.Synchro        		= SAI_SYNCHRONOUS_EXT_SAI1;
  haudio_in_src_sai.Init.OutputDrive    		= SAI_OUTPUTDRIVE_DISABLE;
  haudio_in_src_sai.Init.FIFOThreshold  		= SAI_FIFOTHRESHOLD_1QF;
  haudio_in_src_sai.Init.SynchroExt     		= SAI_SYNCEXT_DISABLE;
  haudio_in_src_sai.Init.CompandingMode 		= SAI_NOCOMPANDING;
  haudio_in_src_sai.Init.TriState       		= SAI_OUTPUT_RELEASED;
  haudio_in_src_sai.Init.Mckdiv         		= 0;

  /* Configure SAI_Block_x Frame
  Frame Length: 64
  Frame active Length: 32
  FS Definition: Start frame + Channel Side identification
  FS Polarity: FS active Low
  FS Offset: FS asserted one bit before the first bit of slot 0 */
  haudio_in_src_sai.FrameInit.FrameLength       = 64;
  haudio_in_src_sai.FrameInit.ActiveFrameLength = 32;
  haudio_in_src_sai.FrameInit.FSDefinition      = SAI_FS_CHANNEL_IDENTIFICATION;
  haudio_in_src_sai.FrameInit.FSPolarity        = SAI_FS_ACTIVE_LOW;
  haudio_in_src_sai.FrameInit.FSOffset          = SAI_FS_BEFOREFIRSTBIT;

  /* Configure SAI Block_x Slot
  Slot First Bit Offset: 0
  Slot Size  : 16
  Slot Number: 4
  Slot Active: All slot active */
  haudio_in_src_sai.SlotInit.FirstBitOffset = 0;
  haudio_in_src_sai.SlotInit.SlotSize       = SAI_SLOTSIZE_DATASIZE;
  haudio_in_src_sai.SlotInit.SlotNumber     = 4;
  haudio_in_src_sai.SlotInit.SlotActive     = CODEC_AUDIOFRAME_SLOT_02;

  HAL_SAI_Init(&haudio_in_src_sai);

  /* Enable SAI peripheral */
  __HAL_SAI_ENABLE(&haudio_in_src_sai);
}

#if 0
/**
  * @brief  Deinitializes the output Audio Codec audio interface (SAI).
  * @retval None
  */
static void SAI_SRC_IN_DeInit(void)
{
  /* Initialize the haudio_in_sai Instance parameter */
  haudio_in_src_sai.Instance = AUDIO_IN_SAI_SRC;;
  /* Disable SAI peripheral */
  __HAL_SAI_DISABLE(&haudio_in_src_sai);

  HAL_SAI_DeInit(&haudio_in_src_sai);
}
#endif

/**
  * @brief  Initialize the DIGI-FP SRC I2S input (for SPDIF, TOSLINK, AES3 input)
  * @param  None
  * @retval None
  */
void SRC_Rx_Init(void)
{
	haudio_in_src_sai.Instance = AUDIO_IN_SAI_SRC;

	SAI_SRC_IN_MspInit(&haudio_in_src_sai, NULL);

	SAI_SRC_IN_Init(AUDIO_FREQ_DEFAULT);
}

/**
  * @brief  Start the DIGI-FP SRC reception, I2S input
  * @param  None
  * @retval None
  */
void SRC_Rx_Start(void)
{
	int bufSize;
	uint8_t *buffer;

	bufSize = AUDIO_PLAYER_GetBuffer(&buffer);
	/* Start transmission (DMA mode) */
	HAL_SAI_Receive_DMA(&haudio_in_src_sai, buffer, bufSize / sizeof(uint16_t));
}

/*
 * TODO why these callbacks are never called?
 */

/**
  * @brief  ISR Handler for the SPDIF Tx (second buffer full)
  * @param  None
  * @retval None
  */
void SRC_RX_TransferComplete_CallBack(struct __DMA_HandleTypeDef * hdma)
{
	(void)hdma;
}

/**
  * @brief  ISR Handler for the SPDIF Tx (first buffer full)
  * @param  None
  * @retval None
  */
void SRC_RX_HalfTransfer_CallBack(struct __DMA_HandleTypeDef * hdma)
{
	(void)hdma;
}
