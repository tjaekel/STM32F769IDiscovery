/**
  ******************************************************************************
  * @file    usbd_desc_msc.h
  * @author  Torsten Jaekel
  * @version V1.1.0
  * @date    04-September-2016
  * @brief   The interface for the USB MSC storage USB enumeration
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_DESC_MSC_H
#define __USBD_DESC_MSC_H

/* Includes ------------------------------------------------------------------*/
#include "usbd_def.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define         DEVICE_ID1_MSC          (0x1FFF7A10)
#define         DEVICE_ID2_MSC          (0x1FFF7A14)
#define         DEVICE_ID3_MSC          (0x1FFF7A18)

#define  USB_SIZ_STRING_SERIAL_MSC       0x1A
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern USBD_DescriptorsTypeDef MSC_Desc;

#endif /* __USBD_DESC_MSC_H */
 
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
