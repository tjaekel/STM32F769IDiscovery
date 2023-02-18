/**
  ******************************************************************************
  * @file    usbd_des_msc.c
  * @author  Torsten Jaekel
  * @version V1.1.0
  * @date    04-September-2016
  * @brief   The USB enumeration descriptors for MSC (storage)
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usbd_core.h"
#include "usbd_desc_msc.h"
#include "usbd_conf_msc.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define USBD_VID_MSC                      0x0483
#define USBD_PID_MSC                      0x5720
#define USBD_LANGID_STRING_MSC            0x409
#define USBD_MANUFACTURER_STRING_MSC      "STMicroelectronics"
#define USBD_PRODUCT_HS_STRING_MSC        "Mass Storage in HS Mode"
#define USBD_PRODUCT_FS_STRING_MSC        "Mass Storage in FS Mode"
#define USBD_CONFIGURATION_HS_STRING_MSC  "MSC Config"
#define USBD_INTERFACE_HS_STRING_MSC      "MSC Interface"
#define USBD_CONFIGURATION_FS_STRING_MSC  "MSC Config"
#define USBD_INTERFACE_FS_STRING_MSC      "MSC Interface"

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
uint8_t *USBD_MSC_DeviceDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *USBD_MSC_LangIDStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *USBD_MSC_ManufacturerStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *USBD_MSC_ProductStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *USBD_MSC_SerialStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *USBD_MSC_ConfigStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *USBD_MSC_InterfaceStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
#ifdef USB_SUPPORT_USER_STRING_DESC
uint8_t *USBD_MSC_USRStringDesc(USBD_SpeedTypeDef speed, uint8_t idx, uint16_t *length);  
#endif /* USB_SUPPORT_USER_STRING_DESC */

/* Private variables ---------------------------------------------------------*/
USBD_DescriptorsTypeDef MSC_Desc = {
  USBD_MSC_DeviceDescriptor,
  USBD_MSC_LangIDStrDescriptor, 
  USBD_MSC_ManufacturerStrDescriptor,
  USBD_MSC_ProductStrDescriptor,
  USBD_MSC_SerialStrDescriptor,
  USBD_MSC_ConfigStrDescriptor,
  USBD_MSC_InterfaceStrDescriptor,  
};

/* USB Standard Device Descriptor */
#if defined ( __ICCARM__ ) /*!< IAR Compiler */
  #pragma data_alignment=4   
#endif
__ALIGN_BEGIN uint8_t USBD_DeviceDesc_MSC[USB_LEN_DEV_DESC] __ALIGN_END = {
  0x12,                       /* bLength */
  USB_DESC_TYPE_DEVICE,       /* bDescriptorType */
  0x00,                       /* bcdUSB */
  0x02,
  0x00,                       /* bDeviceClass */
  0x00,                       /* bDeviceSubClass */
  0x00,                       /* bDeviceProtocol */
  USB_MAX_EP0_SIZE,           /* bMaxPacketSize */
  LOBYTE(USBD_VID_MSC),           /* idVendor */
  HIBYTE(USBD_VID_MSC),           /* idVendor */
  LOBYTE(USBD_PID_MSC),           /* idVendor */
  HIBYTE(USBD_PID_MSC),           /* idVendor */
  0x00,                       /* bcdDevice rel. 2.00 */
  0x02,
  USBD_IDX_MFC_STR,           /* Index of manufacturer string */
  USBD_IDX_PRODUCT_STR,       /* Index of product string */
  USBD_IDX_SERIAL_STR,        /* Index of serial number string */
  USBD_MAX_NUM_CONFIGURATION  /* bNumConfigurations */
}; /* USB_DeviceDescriptor */

/* USB Standard Device Descriptor */
#if defined ( __ICCARM__ ) /*!< IAR Compiler */
  #pragma data_alignment=4   
#endif
__ALIGN_BEGIN uint8_t USBD_LangIDDesc_MSC[USB_LEN_LANGID_STR_DESC] __ALIGN_END = {
  USB_LEN_LANGID_STR_DESC,         
  USB_DESC_TYPE_STRING,       
  LOBYTE(USBD_LANGID_STRING_MSC),
  HIBYTE(USBD_LANGID_STRING_MSC),
};

uint8_t USBD_StringSerial_MSC[USB_SIZ_STRING_SERIAL_MSC] =
{
  USB_SIZ_STRING_SERIAL_MSC,
  USB_DESC_TYPE_STRING,    
};


#if defined ( __ICCARM__ ) /*!< IAR Compiler */
  #pragma data_alignment=4   
#endif
__ALIGN_BEGIN uint8_t USBD_StrDesc_MSC[USBD_MAX_STR_DESC_SIZ] __ALIGN_END;

/* Private functions ---------------------------------------------------------*/
static void IntToUnicode (uint32_t value , uint8_t *pbuf , uint8_t len);
static void Get_SerialNum(void);

/**
  * @brief  Returns the device descriptor. 
  * @param  speed: Current device speed
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t *USBD_MSC_DeviceDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
  *length = sizeof(USBD_DeviceDesc_MSC);
  return (uint8_t*)USBD_DeviceDesc_MSC;
}

/**
  * @brief  Returns the LangID string descriptor.        
  * @param  speed: Current device speed
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t *USBD_MSC_LangIDStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
  *length = sizeof(USBD_LangIDDesc_MSC);
  return (uint8_t*)USBD_LangIDDesc_MSC;
}

/**
  * @brief  Returns the product string descriptor. 
  * @param  speed: Current device speed
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t *USBD_MSC_ProductStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
  if(speed == USBD_SPEED_HIGH)
  {   
    USBD_GetString((uint8_t *)USBD_PRODUCT_HS_STRING_MSC, USBD_StrDesc_MSC, length);
  }
  else
  {
    USBD_GetString((uint8_t *)USBD_PRODUCT_FS_STRING_MSC, USBD_StrDesc_MSC, length);
  }
  return USBD_StrDesc_MSC;
}

/**
  * @brief  Returns the manufacturer string descriptor. 
  * @param  speed: Current device speed
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t *USBD_MSC_ManufacturerStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
  USBD_GetString((uint8_t *)USBD_MANUFACTURER_STRING_MSC, USBD_StrDesc_MSC, length);
  return USBD_StrDesc_MSC;
}

/**
  * @brief  Returns the serial number string descriptor.        
  * @param  speed: Current device speed
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t *USBD_MSC_SerialStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
  *length = USB_SIZ_STRING_SERIAL_MSC;
  
  /* Update the serial number string descriptor with the data from the unique ID*/
  Get_SerialNum();
  
  return (uint8_t*)USBD_StringSerial_MSC;
}

/**
  * @brief  Returns the configuration string descriptor.    
  * @param  speed: Current device speed
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t *USBD_MSC_ConfigStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
  if(speed == USBD_SPEED_HIGH)
  {  
    USBD_GetString((uint8_t *)USBD_CONFIGURATION_HS_STRING_MSC, USBD_StrDesc_MSC, length);
  }
  else
  {
    USBD_GetString((uint8_t *)USBD_CONFIGURATION_FS_STRING_MSC, USBD_StrDesc_MSC, length);
  }
  return USBD_StrDesc_MSC;
}

/**
  * @brief  Returns the interface string descriptor.        
  * @param  speed: Current device speed
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t *USBD_MSC_InterfaceStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
  if(speed == USBD_SPEED_HIGH)
  {
    USBD_GetString((uint8_t *)USBD_INTERFACE_HS_STRING_MSC, USBD_StrDesc_MSC, length);
  }
  else
  {
    USBD_GetString((uint8_t *)USBD_INTERFACE_FS_STRING_MSC, USBD_StrDesc_MSC, length);
  }
  return USBD_StrDesc_MSC;
}

/**
  * @brief  Create the serial number string descriptor 
  * @param  None 
  * @retval None
  */
static void Get_SerialNum(void)
{
  uint32_t deviceserial0, deviceserial1, deviceserial2;
  
  deviceserial0 = *(uint32_t*)DEVICE_ID1_MSC;
  deviceserial1 = *(uint32_t*)DEVICE_ID2_MSC;
  deviceserial2 = *(uint32_t*)DEVICE_ID3_MSC;
  
  deviceserial0 += deviceserial2;
  
  if (deviceserial0 != 0)
  {
    IntToUnicode (deviceserial0, (uint8_t*)&USBD_StringSerial_MSC[2] ,8);
    IntToUnicode (deviceserial1, (uint8_t*)&USBD_StringSerial_MSC[18] ,4);
  }
}

/**
  * @brief  Convert Hex 32Bits value into char 
  * @param  value: value to convert
  * @param  pbuf: pointer to the buffer 
  * @param  len: buffer length
  * @retval None
  */
static void IntToUnicode (uint32_t value , uint8_t *pbuf , uint8_t len)
{
  uint8_t idx = 0;
  
  for( idx = 0; idx < len; idx ++)
  {
    if( ((value >> 28)) < 0xA )
    {
      pbuf[ 2* idx] = (value >> 28) + '0';
    }
    else
    {
      pbuf[2* idx] = (value >> 28) + 'A' - 10; 
    }
    
    value = value << 4;
    
    pbuf[ 2* idx + 1] = 0;
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
