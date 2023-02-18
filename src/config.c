/**
  ******************************************************************************
  * @file    config.c
  * @author  Torsten Jaekel
  * @version V1.1.0
  * @date    04-September-2016
  * @brief   The QSPI permanent config store and restore
  ******************************************************************************
  */

#include "config.h"
#include "main.h"			//for the global stuff
#include "FFT.h"
#include <string.h>			//for memcpy

/**
 * for the configuration we use a structure - have a default config if ReadSysConfig fails
 */
tConfig DefSysConfig = {
		.validID    	= CONFIG_VALID_ID,		//do not change this location and pattern
		.autoCfg		= DEF_AUTO_CFG,
		.configVal  	= DEF_CONFIG_VAL,
		.activeFilter 	= DEF_ACTIVE_FILTER,
		.netConfig		= DEF_NET_CFG,
		.fftType		= DEF_FFT_TYPE_CFG
};

tConfig SysConfig;								//we initialize via ReadSysConfig
uint8_t SysConfigDirty = 0;						//if set to 1 - SysConfig changed and save it

uint8_t ReadSysConfig(tConfig *defConfig)
{
	uint8_t result;

	QSPI_Info info;
	BSP_QSPI_Init();

	HAL_Delay(200);

	BSP_QSPI_GetInfo(&info);

	result = BSP_QSPI_Read((uint8_t *)&SysConfig, QSPI_SECTOR_CONFIG * info.EraseSectorSize, sizeof(SysConfig));
	if (result == QSPI_OK)
	{
		//check if we see the validID, otherwise take the provided defConfig
		if (SysConfig.validID == CONFIG_VALID_ID)
		{
			//the SysConfig is read properly

			//configure system
			if (SysConfig.fftType == FFT_DISPL_DOTS)
			{
				FFT_SetBarType(1);
			}
			if (SysConfig.fftType == FFT_DISPL_BARS)
			{
				FFT_SetBarType(0);
			}
			if (SysConfig.fftType == FFT_DISPL_LINE)
			{
				FFT_SetBarType(2);
			}
			return 0;
		}
		else
		{
			//copy now the defConfig to SysConfig, we have failed to read QSPI
			memcpy(&SysConfig, defConfig, sizeof(tConfig));
		}
	}

	return 1;	//there was an error, but you can continue to use SysConfig (defaults)
}

uint8_t WriteSysConfig()
{
	QSPI_Info info;
	uint8_t err;

	/*
	 * ATT: after Memory Mapped enabled - we cannot erase and write anymore, so in the config write function
	 * we reset to indirect mode before we enable Memory Mapping again.
	 */

	/* with memory mapped we cannot write - so we configure back to default mode */
	BSP_QSPI_Init();

	/*
	 * we have trouble with the timing - sector erase fails! add delays
	 */
	HAL_Delay(200);

	BSP_QSPI_GetInfo(&info);

	//before we write new data - we have to erase a sector - use the info in order to know how large a sector or page is
	//sector 0 is used for our config structure

	//ATT: Erase_Block uses a byte address, not a block number, but this address is not rounded to a sector start, taken as it is
	//better to make sure it is the start address of a sector

	err = BSP_QSPI_Erase_Block(QSPI_SECTOR_CONFIG * info.EraseSectorSize);
	if ( ! err)
	{
		HAL_Delay(200);
		err = BSP_QSPI_Write((uint8_t *)&SysConfig, QSPI_SECTOR_CONFIG * info.EraseSectorSize, sizeof(SysConfig));
		HAL_Delay(200);
	}

	/* change back to memory mapped mode as the default mode */
	BSP_QSPI_EnableMemoryMappedMode();

	return err;
}
