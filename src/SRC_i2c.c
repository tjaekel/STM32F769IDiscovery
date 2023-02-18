/**
  ******************************************************************************
  * @file    SRC_i2c.c
  * @author  Torsten Jaekel
  * @version V1.1.0
  * @date    04-September-2016
  * @brief   The I2C interface to the DIGI-FP board (as Lyrebird APP Ultra)
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"				//we need the AUDIO_FREQ_DEFAULT macro
#include "SRC_i2c.h"

#define WITH_I2C_TEST

/* Local Variables -----------------------------------------------------------*/
static I2C_HandleTypeDef hI2CSRCHandler = {0};

/* Forward Prototypes --------------------------------------------------------*/

/****************************************************************************/

/*
 * This is the config for SRC chip, via I2C.
 * It configures as:
 * - DIR, SPDIF, AES3 Rx3 input
 * - PortA as I2S Out and In (through STM board)
 * - SRC and DIT output as S/PDIF, AES3
 * - S/PDIF out is 96 KHz, I2S on PortA 48 KHz
 * - S/PDIF is 24bit, 48 KHz on DIR (input)
 * - add later: Port B used for Raspberry Pi - route RPi I2S Out to DAC on Port A
 */
uint8_t SRC_Config_Seq_I2C[] = {
//group with pairs REG_ADDR, REG_VALUE, we send each line as I2C write transaction

//set register page to 0
0x7f, 0x00,

//dir setup is default with audio muted for loss of lock condition and rxcko enabled
//==> set here which RXn input we use, bit [1:0]
//select the Rx reference clock: MCLK for Lyrebird board
//0x0d, 0x0B,					//for Lyrebird Board, RX 4
0x0d, 0x09,						//==> for DIGI-FP board, SPDIF in (RX 2), RX 1 = TOSLINK, RX 3 = AES3, RX 4 = not used, MCLK, RX 2

0x0e, 0x09,						//Rx Ctrl - stop if loss of lock, audio data stopped if loss of lock, pass through, RXCKO enabled
/* change to this for the 22.xxxx MHz MEMS, it is not very stable, looks like we have lock loss */
//0x0e, 0x11,					//Rx Ctrl - don't stop if loss of lock, audio data not stopped if loss of lock, pass through, RXCKO enabled

#if AUDIO_FREQ_DEFAULT == 48000
//XXXX pll1 setup for 12.288mhz MCLK, rxcki P is 1, J is 8 D is 0 - for 48KHz audio
0x0f, 0x12,		//P3 P2 P1  P0  _ J5  J4  J3 J2
0x10, 0x00,		//J1 J0 D13 D12 _ D11 D10 D9 D8
0x11, 0x00,		//D7 D6 D5  D4  _ D3  D2  D1 D0
#else
//pll1 setup for 22.5792mhz rxcki p is 2 j is 8 d is d'7075 = h'1BA3 - for 44.1KHz audio
0x0f, 0x22,
0x10, 0x1B,		//xx01_1011_1010_0011 - here: J1 J0 . 0 1 _ 1 0 1 1
0x11, 0xA3,		//here: 1 0 1 0 _ 0 0 1 1
#endif

//set gpo1 for dir nonaudio flag
0x1b, 0x06,
//set gpo2 for dir nonvalid flag
0x1c, 0x07,
//set gpo3 for dir emphasis flag
0x1d, 0x05,
//set gpo4 for dir parity error flag
0x1e, 0x0d,
//src setup is default with the dir as the input data source

//mclk is the ref clock source - select DIR as SRC input
0x2d, 0x02,

0x2e, 0x00,
//XXXX word length: 16bit
0x2f, 0xC0,
//left, right attenuation
0x30, 0x00,
0x31, 0x00,

//dit setup with the src as the input data source and mclk as master clock
//mclk divider is 512 to set the output frame rate
//block start is an output and valid audio is indicated
//c and u data is sourced from the data buffers in register page 2

//temporary:
0x07, 0x64,

0x08, 0x00,
0x09, 0x01,
//power up all blocks except port B, remove reset
0x01, 0x37,
//disable dit buffer transfers
0x08, 0x08,

//configure port A: Output Data Source: master, DIR, 24bit left-justified
//0x03, 0x28,
//configure port A: Output Data Source: master, SRC, 24bit left-justified
//0x03, 0x38,

//XXXX configure port A: Output Data Source: slave, DIR, 24bit, I2S
0x03, 0x21,
//configure port A: Output Data Source: master, Port B, 24bit, I2S - XXXX
////0x03, 0x19,

//XXXX configure 48KHz (divide by 256) - otherwise default is 192KHz, MCLK as default clock
0x04, 0x01,

//config Port B as slave and input for SRC, I2S format
0x05, 0x01,

//config Port B as slave and input for SRC, I2S format, Port B is input for Port A - XXXX
////0x05, 0x11,

//set register page to 2 - DIT Channel Status and User Data Buffers
0x7f, 0x02,
//set DIT channel status data for bytes 0 thru 2
//professional use pcm audio no emphasis locked 48khz sampling freq
//all other channel status bytes are set to default 0x00 value
0x00, 0x81,
0x01, 0x81,
0x02, 0x10,
0x03, 0x10,
0x04, 0x34,
0x05, 0x34,

//set register page to 0 - Control
0x7f, 0x00,
//enable dit buffer transfers
0x08, 0x00,

//dit setup for 96k output, upsample by x2 !

//select SRC as Tx Input Source
0x07, 0x24,						//==> here TXDIF, / 256 as x2 upsample rate, set here for x1, x2, x4
//0x07, 0x34,					//==> here TXDIF, / 512 as x1 upsample rate, set here for x1, x2, x4
//select SRC as Tx Input Source - XXXX
////0x07, 0x2C,

//bypass multiplexer output - select here on bit [7:6] BYPMUX[1:0] for DIR RXn (0..3), here still 0
0x08, 0x08,

//set register page to 2 - DIT Channel Status and User Data Buffers
0x7f, 0x02,
0x00, 0x80,
0x01, 0x80,
0x08, 0x08,
0x09, 0x08,

//set register page to 0 - Control
0x7f, 0x00,
//enable DIT AES3 Encoder Output
0x08, 0x00,

//dit setup for 192k
//0x07, 0x1c,
//0x08, 0x08,

//0x7f, 0x02,
//0x00, 0x80,
//0x01, 0x80,
//0x08, 0x18,
//0x09, 0x18,

//0x7f, 0x00,
//0x08, 0x00,
};

/******************************* I2C Routines *********************************/

/**
  * @brief  Initializes I2C MSP.
  * @param  i2c_handler : I2C handler
  * @retval None
  */
static void I2C1_MspInit(I2C_HandleTypeDef *i2c_handler)
{
  GPIO_InitTypeDef  gpio_init_structure;

  if (i2c_handler == (I2C_HandleTypeDef*)(&hI2CSRCHandler))
  {
    /*** Configure the GPIOs ***/
    /* Enable GPIO clock */
	__HAL_RCC_GPIOB_CLK_ENABLE();

    /* Configure I2C SCL as alternate function */
    gpio_init_structure.Pin = GPIO_PIN_8;
    gpio_init_structure.Mode = GPIO_MODE_AF_OD;
    gpio_init_structure.Pull = GPIO_NOPULL;
    gpio_init_structure.Speed = GPIO_SPEED_FAST;
    gpio_init_structure.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &gpio_init_structure);

    /* Configure I2C Rx as alternate function */
    gpio_init_structure.Pin = GPIO_PIN_9;
    HAL_GPIO_Init(GPIOB, &gpio_init_structure);

    /*** Configure the I2C peripheral ***/
    /* Enable I2C clock */
    __HAL_RCC_I2C1_CLK_ENABLE();

    /* Force the I2C peripheral clock reset */
    __HAL_RCC_I2C1_FORCE_RESET();

    /* Release the I2C peripheral clock reset */
    __HAL_RCC_I2C1_RELEASE_RESET();

    /* Enable and set I2Cx Interrupt to a lower priority */
    HAL_NVIC_SetPriority(I2C1_EV_IRQn, 0x05, 0);
    HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);

    /* Enable and set I2Cx Interrupt to a lower priority */
    HAL_NVIC_SetPriority(I2C1_ER_IRQn, 0x05, 0);
    HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);
  }
  else
  {
    return;
  }
}

/**
  * @brief  Initializes I2C HAL.
  * @param  i2c_handler : I2C handler
  * @retval None
  */
static void I2C1_Init(I2C_HandleTypeDef *i2c_handler)
{
  if(HAL_I2C_GetState(i2c_handler) == HAL_I2C_STATE_RESET)
  {
    if (i2c_handler == (I2C_HandleTypeDef*)(&hI2CSRCHandler))
    {
      /* Audio and LCD I2C configuration */
      i2c_handler->Instance = I2C1;
    }
    else
    {
      return;
    }
    i2c_handler->Init.Timing           = DISCOVERY_I2Cx_TIMING;
    i2c_handler->Init.OwnAddress1      = 0;
    i2c_handler->Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
    i2c_handler->Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
    i2c_handler->Init.OwnAddress2      = 0;
    i2c_handler->Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
    i2c_handler->Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;

    /* Init the I2C */
    I2C1_MspInit(i2c_handler);
    HAL_I2C_Init(i2c_handler);
  }
}

/**
  * @brief  Manages error callback by re-initializing I2C.
  * @param  i2c_handler : I2C handler
  * @param  Addr: I2C Address
  * @retval None
  */
static void I2C1_Error(I2C_HandleTypeDef *i2c_handler, uint8_t Addr)
{
  /* De-initialize the I2C communication bus */
  HAL_I2C_DeInit(i2c_handler);

  /* Re-Initialize the I2C communication bus */
  I2C1_Init(i2c_handler);
}

/**
  * @brief  Reads multiple data.
  * @param  i2c_handler : I2C handler
  * @param  Addr: I2C address
  * @param  Reg: Reg address
  * @param  MemAddress: Memory address
  * @param  Buffer: Pointer to data buffer
  * @param  Length: Length of the data
  * @retval Number of read data
  */
static HAL_StatusTypeDef I2C1_ReadMultiple(I2C_HandleTypeDef *i2c_handler,
                                           uint8_t Addr,
                                           uint16_t Reg,
                                           uint16_t MemAddress,
                                           uint8_t *Buffer,
                                           uint16_t Length)
{
  HAL_StatusTypeDef status = HAL_OK;

  status = HAL_I2C_Mem_Read(i2c_handler, Addr, (uint16_t)Reg, MemAddress, Buffer, Length, 1000);

  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* I2C error occurred */
    I2C1_Error(i2c_handler, Addr);
  }
  return status;
}

/**
  * @brief  Writes a value in a register of the device through BUS in using DMA mode.
  * @param  i2c_handler : I2C handler
  * @param  Addr: Device address on BUS Bus.
  * @param  Reg: The target register address to write
  * @param  MemAddress: Memory address
  * @param  Buffer: The target register value to be written
  * @param  Length: buffer size to be written
  * @retval HAL status
  */
static HAL_StatusTypeDef I2C1_WriteMultiple(I2C_HandleTypeDef *i2c_handler,
                                            uint8_t Addr,
                                            uint16_t Reg,
                                            uint16_t MemAddress,
                                            uint8_t *Buffer,
                                            uint16_t Length)
{
  HAL_StatusTypeDef status = HAL_OK;

  status = HAL_I2C_Mem_Write(i2c_handler, Addr, (uint16_t)Reg, MemAddress, Buffer, Length, 1000);

  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Re-Initialize the I2C Bus */
    I2C1_Error(i2c_handler, Addr);
  }
  return status;
}

/**
  * @brief  Initializes Audio low level.
  * @retval None
  */
static void SRC_I2C_Init(void)
{
  I2C1_Init(&hI2CSRCHandler);
}

/**
  * @brief  Deinitializes Audio low level.
  * @retval None
  */
static void SRC_I2C_DeInit(void)
{
	/*
	 * @todo
	 * release the I2C, stop the SRC chip ...
	 */
}

/**
  * @brief  Writes a single data.
  * @param  Addr: I2C address
  * @param  Reg: Reg address
  * @param  Value: Data to be written
  * @retval None
  */
static void SRC_I2C_Write(uint8_t Addr, uint8_t Reg, uint8_t Value)
{
  I2C1_WriteMultiple(&hI2CSRCHandler, Addr, Reg, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&Value, 1);
}

/**
  * @brief  Reads a single data.
  * @param  Addr: I2C address
  * @param  Reg: Reg address
  * @retval Data to be read
  */
static uint8_t SRC_I2C_Read(uint8_t Addr, uint8_t Reg)
{
  uint8_t read_value;

  I2C1_ReadMultiple(&hI2CSRCHandler, Addr, Reg, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&read_value, 1);

  return read_value;
}

/******************************* Exported Routines **********************/

/*
 * This is the exported, global function.
 * It configures I2C, writes the config to SRC as well as CS2300 chip (as default config)
 */
int SRC_I2C_Configure(void)
{
#ifdef WITH_I2C_TEST
	uint8_t regVal;
#endif
	int i;
	uint8_t *bPtr;

	//configure the SRC chip
	SRC_I2C_Init();

#if 1
	//reset the SRC chip
	SRC_I2C_Write(SRC_I2C_ADDR, 0x01, 0x80);
	//keep it active for min. 500 ns
	i = 0;
	while (i < 1000)
	{
		__NOP();
		i++;
	}
	SRC_I2C_Write(SRC_I2C_ADDR, 0x01, 0x00);
	//wait at least 500 us before we continue to write
	i = 0;
	while (i < 500000)
	{
		__NOP();
		i++;
	}
#endif

#ifdef WITH_I2C_TEST
	//for testing I2C
	regVal = SRC_I2C_Read(SRC_I2C_ADDR, 0x05);

	SRC_I2C_Write(SRC_I2C_ADDR, 0x05, 0xaa);

	regVal = SRC_I2C_Read(SRC_I2C_ADDR, 0x05);

	//if (regVal != 0xaa)
	//	return 1;			//error, WR + RD back has failed, I2C not working
	//Remark: with SCR4382 on DIGI_FP it does not work this way (not correct value back),
	//but we can configure the chip and check later
#endif

	bPtr = SRC_Config_Seq_I2C;
	//we use pairs as REG_ADDR, REG_VALUE
	for (i = 0; i < sizeof(SRC_Config_Seq_I2C) / 2; i++)
	{
		SRC_I2C_Write(SRC_I2C_ADDR, *(bPtr + 0), *(bPtr + 1));
		bPtr += 2;
	}

#ifdef WITH_I2C_TEST
	//read back the last written value, as cross check if OK
	regVal = SRC_I2C_Read(SRC_I2C_ADDR, 0x03);

	if (regVal != 0x21)		//IS THIS STILL CORRECT FOR ALL MACORS ??!!
		return 1;			//error, WR + RD back has failed, I2C not working
#endif

	return 0;				//no error
}

/**
 * release the SRC chip, stop CS2000, release and deinit I2C
 * @todo
 * still to implement to release I2C
 */
int SRC_I2C_Release(void)
{
	SRC_I2C_DeInit();

	return 0;				//no error
}

int SRC_ClockSource(EClockSrc src)
{
	//set the GPIO pin in order to select INT/EXT clock source (EXT is Word Clock)

	if (src == CLOCK_SRC_EXTERNAL)
		HAL_GPIO_WritePin(GPIOH, GPIO_PIN_6, GPIO_PIN_SET);		//selects Word Clock input (balanced or unbalanced: via jumper)
	else
		HAL_GPIO_WritePin(GPIOH, GPIO_PIN_6, GPIO_PIN_RESET);	//selects internal XTAL as clock

	return 0;				//no error
}

int SRC_RefClock(ERefClk clk)
{
	//set the GPIO pin in order to select which XGTAL to use, for 48 or 44.1KHz
	//ATT:
	//you must also use this function in order to set a proper RefClock on Word Clock PLL chip!
	//ATT:
	//we must reprogram also the PLL in the SRC chip

	if (clk == CLOCK_XTAL_2)
	{
		HAL_GPIO_WritePin(GPIOI, GPIO_PIN_3, GPIO_PIN_SET);		//selects XTAL 2, for 44.1KHz, as 22.5792MHz
		SRC_I2C_Write(SRC_I2C_ADDR, 0x0F, 0x22);				//unchanged
		SRC_I2C_Write(SRC_I2C_ADDR, 0x10, 0x1B);
		SRC_I2C_Write(SRC_I2C_ADDR, 0x11, 0xA3);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOI, GPIO_PIN_3, GPIO_PIN_RESET);	//selects XTAL 1, for 48KHz, as 24.576MHz
		SRC_I2C_Write(SRC_I2C_ADDR, 0x0F, 0x22);				//unchanged
		SRC_I2C_Write(SRC_I2C_ADDR, 0x10, 0x00);
		SRC_I2C_Write(SRC_I2C_ADDR, 0x11, 0x00);
	}

	return 0;				//no error
}

int SRC_AudioSource(EAudioSource src)
{
	int8_t tmp;

	//it writes SRC 0x0D, page 0 register via I2C
	//format: h'0000_10xx
	tmp = SRC_I2C_Read(SRC_I2C_ADDR, 0x0d);
	tmp &= 0xFC;
	tmp |= src;
	SRC_I2C_Write(SRC_I2C_ADDR, 0x0d, tmp);

	return 0;				//no error
}

int SRC_SetSampleRate(ESampleRate rate)
{
	//it writes SRC 0x07, page 0 register via I2C
	//format: h'0xx0_0100
	uint8_t tmp;
	tmp = SRC_I2C_Read(SRC_I2C_ADDR, 0x07);
	switch (rate)
	{
	//is there still something wrong if set to 48 or 192KHz? for me the receiver stalls,
	//just the x2 as 96KHz works fine, but I hear the effect, the changed pitch
	case SRC_RATE_1 :	tmp &= 0x9F; tmp |= 0x60; break;	// it is a divider: /512 = 48KHz
	case SRC_RATE_2 :	tmp &= 0x9F; tmp |= 0x20; break;	// /256 = 96KHz
	case SRC_RATE_4 :	tmp &= 0x9F; tmp |= 0x00; break;	// /128 = 192KHz
	default : return 1;		//error - wrong selection
	}
	SRC_I2C_Write(SRC_I2C_ADDR, 0x07, tmp);

	return 0;				//no error
}

int SRC_SetRefClock(EClockSrc ref)
{
	/*
	 * This function does not seem to make any improvement or effect
	 */

	//we configure SRC to use MCLK (all external) with CLOCK_SRC_EXTERNAL,
	//or to change to use DIR recovered clock, RXCKO, with CLOCK_SRC_INTERNAL.
	//BUT: if internal: we cannot send to DIT, as I2S to S/PDIF: it will not work
	//if DIR does not have a clock (in and out just, not just out).

	/*
	 * ATT: it is still sensitive for WC: if we connect we change still CS2000 and clock (MCLK) on SRC,
	 * and SRC acts on it. But why ? - does it not relock clock and uses changed clock?
	 * With CLOCK_SRC_INTERNAL we want to use DIR recovered RXCKO - but not really an improvement!
	 * It should not sensitive for MCLK, but it is and follows Word Clock !
	 *
	 * Anyway: it sounds more clear with RXCKO (recovered DIR clock) (what I have assumed).
	 */
	uint8_t regVal;

	if (ref == CLOCK_SRC_EXTERNAL)
	{
		//configure all to use MCLK - Read-Modify-Write !
		regVal  = SRC_I2C_Read(SRC_I2C_ADDR, 0x04);		//Port A
		regVal &= 0xF3;				//[3:2] = b'00 for MCLK
		SRC_I2C_Write(SRC_I2C_ADDR, 0x04, regVal);

		//regVal = SRC_I2C_Read(SRC_I2C_ADDR, 0x06);		//Port B
		//regVal &= 0xF3;				//[3:2] = b'00 for MCLK
		//SRC_I2C_Write(SRC_I2C_ADDR, 0x06, regVal);

		regVal  = SRC_I2C_Read(SRC_I2C_ADDR, 0x07);		//Tx (DIT)
		regVal &= 0x7F;				//[7] = b'00 for MCLK
		SRC_I2C_Write(SRC_I2C_ADDR, 0x07, regVal);

		regVal  = SRC_I2C_Read(SRC_I2C_ADDR, 0x2D);		//SRC
		regVal &= 0xF3;				//[3:2] = b'00 for MCLK
		SRC_I2C_Write(SRC_I2C_ADDR, 0x2D, regVal);

		//optional: register 0x0E for RXCKO output, reg. 0x0D remains fix on 0xXXXX_1XXXX (MCLK, not RXCKI)
	}
	else
	{
		//configure all to use RXCKO (recovered DIR clock, needs an S/PDIF, AES3 ... input)
		regVal  = SRC_I2C_Read(SRC_I2C_ADDR, 0x04);		//Port A
		regVal &= 0xF3;				//[3:2] = b'00 for MCLK
		regVal |= 0x08;				//[3:2] = b'10 for RXCKO
		SRC_I2C_Write(SRC_I2C_ADDR, 0x04, regVal);

		//regVal  = SRC_I2C_Read(SRC_I2C_ADDR, 0x06);		//Port B
		//regVal &= 0xF3;				//[3:2] = b'00 for MCLK
		//regVal |= 0x08;				//RXCKO
		//SRC_I2C_Write(SRC_I2C_ADDR, 0x06, regVal);

		regVal  = SRC_I2C_Read(SRC_I2C_ADDR, 0x07);		//Tx (DIT)
		regVal &= 0x7F;				//[7] = b'00 for MCLK
		regVal |= 0x80;				//RXCKO
		SRC_I2C_Write(SRC_I2C_ADDR, 0x07, regVal);

		regVal  = SRC_I2C_Read(SRC_I2C_ADDR, 0x2D);		//SRC
		regVal &= 0xF3;				//[3:2] = b'00 for MCLK
		regVal |= 0x08;				//RXCKO
		SRC_I2C_Write(SRC_I2C_ADDR, 0x2D, regVal);
	}

	return 0;				//no error
}
