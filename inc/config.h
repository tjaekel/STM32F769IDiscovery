/**
  ******************************************************************************
  * @file    config.h
  * @author  Torsten Jaekel
  * @version V1.1.0
  * @date    04-September-2016
  * @brief   The QSPI config store and restore
  ******************************************************************************
  */

#ifndef CONFIG_H_
#define CONFIG_H_

#include "stm32f7xx.h"
#include "stm32f769i_discovery_qspi.h"

#define QSPI_SECTOR_CONFIG		0

/**
 * define the pattern to recognize a valid QSPI flash config
 */
#define	CONFIG_VALID_ID			0x434F4E46			/*!< an ID which tells us it is valid */
													//if we read from QSPI, to know
/**
 * coding of autoCfg
 */
#define	AUTO_CFG_DISABLED		0
#define	AUTO_CFG_ENABLED		1

/**
 * format for the configVal - bit mask
 */
#define	USB_AUDIO				1					/*!< use USB for audio, not for SD Card storage management */

/**
 * coding parameters for activated filters - activeFilter - bit coding
 */
#define	FILT_NO_FILTERS			0x00000000			/*!< all filters disabled */
#define FILT_BASS_BOOST			0x00000001			/*!< Bass Boost for speakers */

/**
 * coding parameters for netConfig - bit coding
 */
#define	NET_DHCP				1					/*!< with DHCP or static IP address */
#define	NET_WEB_SERVER			2					/*!< with or w/o web server */

/**
 * coding parameters for fftType - enum
 */
#define	FFT_DISPL_DOTS			1
#define	FFT_DISPL_BARS			2
#define	FFT_DISPL_LINE			3

/**
 * define the default config
 */
#define	DEF_AUTO_CFG			AUTO_CFG_DISABLED
#define	DEF_CONFIG_VAL			USB_AUDIO
#define	DEF_ACTIVE_FILTER		FILT_NO_FILTERS
#define	DEF_NET_CFG				NET_DHCP
#define	DEF_FFT_TYPE_CFG		FFT_DISPL_LINE

typedef struct {
	uint32_t	validID;							/*!< pattern to recognize if config is valid on QPSI flash */
	uint32_t	autoCfg;							/*!< do we start automatically with saved config? */
	uint32_t	configVal;							/*!< upper 16bit for output, lower 16bit for input interfaces */
	uint32_t	activeFilter;						/*!< which filters are activated */
	uint32_t	netConfig;							/*!< DHCP, fix IP address */
	uint32_t	fftType;							/*!< which display type for FFT */
} tConfig;

extern uint8_t ReadSysConfig(tConfig *defConfig);
extern uint8_t WriteSysConfig();

extern tConfig DefSysConfig;						//don't touch, have it as default config
extern tConfig SysConfig;							//use this as system config
extern uint8_t SysConfigDirty;						//a flag if we had to save config (changed)

#endif /* CONFIG_H_ */
