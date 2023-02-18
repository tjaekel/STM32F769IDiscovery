/**
  ******************************************************************************
  * @file    usbd_storage.h
  * @author  Torsten Jaekel
  * @version V1.1.0
  * @date    04-September-2016
  * @brief   The interface for the USB MSC storage device
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_STORAGE_H_
#define __USBD_STORAGE_H_

/* Includes ------------------------------------------------------------------*/
#include "usbd_msc.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern USBD_StorageTypeDef  USBD_DISK_fops;

#endif /* __USBD_STORAGE_H_ */
 
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
