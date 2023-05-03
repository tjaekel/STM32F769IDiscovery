/**
  ******************************************************************************
  * @file    usbd_audio.c
  * @author  MCD Application Team
  * @version V2.4.2
  * @date    11-December-2015
  * @brief   This file provides the Audio core functions.
  *
  * @verbatim
  *      
  *          ===================================================================      
  *                                AUDIO Class  Description
  *          ===================================================================
 *           This driver manages the Audio Class 1.0 following the "USB Device Class Definition for
  *           Audio Devices V1.0 Mar 18, 98".
  *           This driver implements the following aspects of the specification:
  *             - Device descriptor management
  *             - Configuration descriptor management
  *             - Standard AC Interface Descriptor management
  *             - 1 Audio Streaming Interface (with single channel, PCM, Stereo mode)
  *             - 1 Audio Streaming Endpoint
  *             - 1 Audio Terminal Input (1 channel)
  *             - Audio Class-Specific AC Interfaces
  *             - Audio Class-Specific AS Interfaces
  *             - AudioControl Requests: only SET_CUR and GET_CUR requests are supported (for Mute)
  *             - Audio Feature Unit (limited to Mute control)
  *             - Audio Synchronization type: Asynchronous
  *             - Single fixed audio sampling rate (configurable in usbd_conf.h file)
  *          The current audio class version supports the following audio features:
  *             - Pulse Coded Modulation (PCM) format
  *             - sampling rate: 48KHz. 
  *             - Bit resolution: 16
  *             - Number of channels: 2
  *             - No volume control
  *             - Mute/Unmute capability
  *             - Asynchronous Endpoints 
  *          
  * @note     In HS mode and when the DMA is used, all variables and data structures
  *           dealing with the DMA during the transaction process should be 32-bit aligned.
  *           
  *      
  *  @endverbatim
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "AudioPlayer.h"
#include "Mem2MemDMA.h"
//#include "usbd_audio.h"
#include "usbd_desc.h"
#include "usbd_ctlreq.h"

#include "SPKR_BassBoost.h"

/** @addtogroup STM32_USB_DEVICE_LIBRARY
  * @{
  */


/** @defgroup USBD_AUDIO 
  * @brief usbd core module
  * @{
  */ 

/** @defgroup USBD_AUDIO_Private_TypesDefinitions
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup USBD_AUDIO_Private_Defines
  * @{
  */ 

/**
  * @}
  */ 


/** @defgroup USBD_AUDIO_Private_Macros
  * @{
  */ 
#define AUDIO_SAMPLE_FREQ(frq)      (uint8_t)(frq), (uint8_t)((frq >> 8)), (uint8_t)((frq >> 16))

#define AUDIO_PACKET_SZE(frq)       (uint8_t)(((frq * 2 * 2)/1000) & 0xFF), \
                                    (uint8_t)((((frq * 2 * 2)/1000) >> 8) & 0xFF)

extern uint32_t EP1PktLen;

/**
  * @}
  */ 

/** @defgroup USBD_AUDIO_Private_FunctionPrototypes
  * @{
  */

static uint8_t  USBD_AUDIO_Init (USBD_HandleTypeDef *pdev, 
                               uint8_t cfgidx);

static uint8_t  USBD_AUDIO_DeInit (USBD_HandleTypeDef *pdev, 
                                 uint8_t cfgidx);

static uint8_t  USBD_AUDIO_Setup (USBD_HandleTypeDef *pdev, 
                                USBD_SetupReqTypedef *req);

static uint8_t  *USBD_AUDIO_GetCfgDesc (uint16_t *length);

static uint8_t  *USBD_AUDIO_GetDeviceQualifierDesc (uint16_t *length);

static uint8_t  USBD_AUDIO_DataIn (USBD_HandleTypeDef *pdev, uint8_t epnum);

static uint8_t  USBD_AUDIO_DataOut (USBD_HandleTypeDef *pdev, uint8_t epnum);

static uint8_t  USBD_AUDIO_EP0_RxReady (USBD_HandleTypeDef *pdev);

static uint8_t  USBD_AUDIO_EP0_TxReady (USBD_HandleTypeDef *pdev);

static uint8_t  USBD_AUDIO_SOF (USBD_HandleTypeDef *pdev);

static uint8_t  USBD_AUDIO_IsoINIncomplete (USBD_HandleTypeDef *pdev, uint8_t epnum);

static uint8_t  USBD_AUDIO_IsoOutIncomplete (USBD_HandleTypeDef *pdev, uint8_t epnum);

static void AUDIO_REQ_GetCurrent(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);

static void AUDIO_REQ_SetCurrent(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);

/**
  * @}
  */ 

/** @defgroup USBD_AUDIO_Private_Variables
  * @{
  */ 

USBD_ClassTypeDef USBD_AUDIO =
{
  USBD_AUDIO_Init,
  USBD_AUDIO_DeInit,
  USBD_AUDIO_Setup,
  USBD_AUDIO_EP0_TxReady,  
  USBD_AUDIO_EP0_RxReady,
  USBD_AUDIO_DataIn,
  USBD_AUDIO_DataOut,
  USBD_AUDIO_SOF,
  USBD_AUDIO_IsoINIncomplete,
  USBD_AUDIO_IsoOutIncomplete,      
  USBD_AUDIO_GetCfgDesc,
  USBD_AUDIO_GetCfgDesc, 
  USBD_AUDIO_GetCfgDesc,
  USBD_AUDIO_GetDeviceQualifierDesc,
};

/* USB AUDIO device Configuration Descriptor */
__ALIGN_BEGIN static uint8_t USBD_AUDIO_CfgDescOut[USB_AUDIO_CONFIG_DESC_SIZE_OUT] __ALIGN_END =
{
  /* Configuration 1 */
  0x09,                                 /* bLength */
  USB_DESC_TYPE_CONFIGURATION,          /* bDescriptorType */
  LOBYTE(USB_AUDIO_CONFIG_DESC_SIZE_OUT),    /* wTotalLength */
  HIBYTE(USB_AUDIO_CONFIG_DESC_SIZE_OUT),
  0x02,	                                /* bNumInterfaces */
  0x01,                                 /* bConfigurationValue */
  0x00,                                 /* iConfiguration */
  0xC0,                                 /* bmAttributes  BUS Powered*/
  0x32,                                 /* bMaxPower = 100 mA*/
  /* 09 byte*/
  
  /* USB Speaker Standard interface descriptor */
  AUDIO_INTERFACE_DESC_SIZE,            /* bLength */
  USB_DESC_TYPE_INTERFACE,              /* bDescriptorType */
  0x00,                                 /* bInterfaceNumber 0 */
  0x00,                                 /* bAlternateSetting */
  0x00,                               	/* bNumEndpoints */
  USB_DEVICE_CLASS_AUDIO,               /* bInterfaceClass */
  AUDIO_SUBCLASS_AUDIOCONTROL,          /* bInterfaceSubClass */
  AUDIO_PROTOCOL_UNDEFINED,             /* bInterfaceProtocol */
  0x00,                                 /* iInterface */
  /* 09 byte*/
  
  /* USB Speaker Class-specific AC Interface Descriptor */
  AUDIO_INTERFACE_DESC_SIZE,            /* bLength */
  AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
  AUDIO_CONTROL_HEADER,                 /* bDescriptorSubtype */
  0x00,          /* 1.00 */             /* bcdADC */
  0x01,
  0x27,                                 /* wTotalLength = 39*/
  0x00,
  0x01,                                 /* bInCollection */
  0x01,                                 /* baInterfaceNr */
  /* 09 byte*/
  
  /* USB Speaker Input Terminal Descriptor */
  AUDIO_INPUT_TERMINAL_DESC_SIZE,       /* bLength */
  AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
  AUDIO_CONTROL_INPUT_TERMINAL,         /* bDescriptorSubtype */
  0x01,                                 /* bTerminalID */
  0x01,                                 /* wTerminalType AUDIO_TERMINAL_USB_STREAMING 0x0101 */
  0x01,
  0x00,                                 /* bAssocTerminal */
  0x02,	//0x01	                            /* bNrChannels */
  0x00,                                 /* wChannelConfig 0x0000 Mono */
  0x03, //0x00 Stereo
  0x00,                                 /* iChannelNames */
  0x00,                                 /* iTerminal */
  /* 12 byte*/
  
  /* USB Speaker Audio Feature Unit Descriptor */
  0x09,                                 /* bLength */
  AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
  AUDIO_CONTROL_FEATURE_UNIT,           /* bDescriptorSubtype */
  AUDIO_OUT_STREAMING_CTRL,             /* bUnitID */
  0x01,                                 /* bSourceID */
  0x01,                                 /* bControlSize */
  AUDIO_CONTROL_MUTE,// |AUDIO_CONTROL_VOLUME, /* bmaControls(0) */
  0,                                    /* bmaControls(1) */
  0x00,                                 /* iTerminal */
  /* 09 byte*/
  
  /*USB Speaker Output Terminal Descriptor */
  0x09,      /* bLength */
  AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
  AUDIO_CONTROL_OUTPUT_TERMINAL,        /* bDescriptorSubtype */
  0x03,                                 /* bTerminalID */
  0x01,                                 /* wTerminalType  0x0301*/
  0x03,
  0x00,                                 /* bAssocTerminal */
  0x02,                                 /* bSourceID */
  0x00,                                 /* iTerminal */
  /* 09 byte*/
  
  /* USB Speaker Standard AS Interface Descriptor - Audio Streaming Zero Bandwidth */
  /* Interface 1, Alternate Setting 0                                             */
  AUDIO_INTERFACE_DESC_SIZE,  			/* bLength */
  USB_DESC_TYPE_INTERFACE,        		/* bDescriptorType */
  0x01,                                 /* bInterfaceNumber  1 */
  0x00,                                 /* bAlternateSetting */
  0x00, //0x00                                /* bNumEndpoints */
  USB_DEVICE_CLASS_AUDIO,               /* bInterfaceClass */
  AUDIO_SUBCLASS_AUDIOSTREAMING,        /* bInterfaceSubClass */
  AUDIO_PROTOCOL_UNDEFINED,             /* bInterfaceProtocol */
  0x00,                                 /* iInterface */
  /* 09 byte*/
  
  /* USB Speaker Standard AS Interface Descriptor - Audio Streaming Operational */
  /* Interface 1, Alternate Setting 1                                           */
  AUDIO_INTERFACE_DESC_SIZE,  			/* bLength */
  USB_DESC_TYPE_INTERFACE,        		/* bDescriptorType */
  0x01,                                 /* bInterfaceNumber  1 */
  0x01,                                 /* bAlternateSetting */
  0x01,	                                /* bNumEndpoints */
  USB_DEVICE_CLASS_AUDIO,               /* bInterfaceClass */
  AUDIO_SUBCLASS_AUDIOSTREAMING,        /* bInterfaceSubClass */
  AUDIO_PROTOCOL_UNDEFINED,             /* bInterfaceProtocol */
  0x00,                                 /* iInterface */
  /* 09 byte*/
  
  /* USB Speaker Audio Streaming Interface Descriptor */
  AUDIO_STREAMING_INTERFACE_DESC_SIZE,  /* bLength */
  AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
  AUDIO_STREAMING_GENERAL,              /* bDescriptorSubtype */
  0x01,                                 /* bTerminalLink */
  0x01,                                 /* bDelay */
  0x01,                                 /* wFormatTag AUDIO_FORMAT_PCM  0x0001*/
  0x00,
  /* 07 byte*/
  
  /* USB Speaker Audio Type III Format Interface Descriptor */
  //0x0B,                                 /* bLength */
  14,
  AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
  AUDIO_STREAMING_FORMAT_TYPE,          /* bDescriptorSubtype */
  AUDIO_FORMAT_TYPE_III,                /* bFormatType */ 
  0x02,                                 /* bNrChannels */
  0x02,                                 /* bSubFrameSize :  2 Bytes per frame (16bits) */
  16,                                   /* bBitResolution (16-bits per sample) */ 
  //0x01,                                 /* bSamFreqType only one frequency supported */
  //AUDIO_SAMPLE_FREQ(USBD_AUDIO_FREQ),   /* Audio sampling frequency coded on 3 bytes */
  /* 11 byte*/
  0x02,                                 /* bSamFreqType two frequencies supported */
  AUDIO_SAMPLE_FREQ(USBD_AUDIO_FREQ),   /* Audio sampling frequency coded on 3 bytes */
  AUDIO_SAMPLE_FREQ(USBD_AUDIO_FREQ2),	/* Audio sampling frequency coded on 3 bytes */
  /* 11 byte*/
  
  /* Endpoint 1 - Standard Descriptor */
  AUDIO_STANDARD_ENDPOINT_DESC_SIZE,    /* bLength */
  USB_DESC_TYPE_ENDPOINT,               /* bDescriptorType */
  AUDIO_OUT_EP,                         /* bEndpointAddress 1 out end point*/
  USBD_EP_TYPE_ISOC,                    /* bmAttributes */
  AUDIO_PACKET_SZE(USBD_AUDIO_FREQ),    /* wMaxPacketSize in Bytes (Freq(Samples)*2(Stereo)*2(HalfWord)) */
  0x01,                                 /* bInterval */
  0x00,                                 /* bRefresh */
  0x00,                                 /* bSynchAddress */
  /* 09 byte*/

  /* Endpoint - Audio Streaming Descriptor*/
  AUDIO_STREAMING_ENDPOINT_DESC_SIZE,   /* bLength */
  AUDIO_ENDPOINT_DESCRIPTOR_TYPE,       /* bDescriptorType */
  AUDIO_ENDPOINT_GENERAL,               /* bDescriptor */
  0x00,                                 /* bmAttributes */
  0x00,                                 /* bLockDelayUnits */
  0x00,                                 /* wLockDelay */
  0x00
  /* 07 byte*/
};

__ALIGN_BEGIN static uint8_t USBD_AUDIO_CfgDescIn[USB_AUDIO_CONFIG_DESC_SIZE_IN] __ALIGN_END =
{
0x09, /* bLength */
USB_DESC_TYPE_CONFIGURATION,          	/* bDescriptorType */
LOBYTE(USB_AUDIO_CONFIG_DESC_SIZE_IN),	/* wTotalLength */
HIBYTE(USB_AUDIO_CONFIG_DESC_SIZE_IN),
0x02, /* bNumInterfaces */
0x01, /* bConfigurationValue */
0x00, /* iConfiguration */
0x80, /* bmAttributes BUS Powered*/
0x32, /* bMaxPower = 100 mA*/  //0x0a

/* USB Speaker Standard AC Interface descriptor */
0x09,
0x04,
0x00,
0x00,
0x00,
0x01,
0x01,
0x00,
0x00,

/* Class specific AC interface descriptor */
0x09,
0x24,
0x01,
0x00,/* bcdADC */
0x01,
0x1E,/* wTotalLength - 30 */
0x00,
0x01, /* bInCollection */
0x01, /* baInterfaceNr(1) */

/* Input Terminal Descriptor */
0x0C, /* bLength */
0x24, /* bDescriptorType */
0x02, /* bDescriptorSubType */
0x01, /* bTerminalID */
0x02, /* wTerminalType */
0x06, /* wTerminalType */		//0x0201 = Microphone  //0x0401 = bidirectional head set, 0x0602 = Digital generic device
								//0x0603 = Line Interface, 0x0605 = SPDIF interface
0x00, /* bAssocTerminal */
0x02, /* bNrChannels */      	// <---- 0x02,  // two channels //0x01
0x03, /* wChannelConfig */   	// <---- 0x03,  // left front / right front //0x00
0x00, /* wChannelConfig */
0x00, /* iChannelName */
0x00, /* iTerminal */

/* Output Terminal Descriptor */
0x09, /* bLength */
0x24, /* bDescriptorType */
0x03, /* bDescriptorSubType */
0x02, /* bTerminalID */
0x01, /* wTerminalType */
0x01, /* wTerminalType */
0x00, /* bAssocTerminal */
0x01, /* bNrChannels */
0x00, /* wChannelConfig */

/* Standard AS Interface Descriptor (Alt. Set. 0) */
0x09, /* bLength */
0x04, /* bDescriptorType */
0x01, /* bInterfaceNum */
0x00, /* bAlternateSetting */
0x00, /* bNumEndpoints */
0x01, /* bInterfaceClass */
0x02, /* bInterfaceSubClass */
0x00, /* bInterfaceProtocol */
0x00, /* iInterface */

/* Standard AS Interface Descriptor (Alt. Set. 1) */
0x09, /* bLength */
0x04, /* bDescriptorType */
0x01, /* bInterfaceNum */
0x01, /* bAlternateSetting */
0x01, /* bNumEndpoints */
0x01, /* bInterfaceClass */
0x02, /* bInterfaceSubClass */
0x00, /* bInterfaceProtocol */
0x00, /* iInterface */

/* Class specific AS General Interface Descriptor */
0x07, /* bLength */
0x24, /* bDescriptorType */
0x01, /* bDescriptorSubType */
0x02, /* bTerminalLink */
0x01, /* bDelay */ //0x01
0x01, /* wFormatTag */
0x00, /* wFormatTag */

/* Format Type Descriptor */
//0x0B,
//0x24,
//0x02,
//0x01,		//0x01
//0x01,     /* bNrChannels */  // <---- 0x02, //Two channels //0x01
//0x02,
//0x10,
//0x01,
//0x44,     /*tSamFreq[0] */   // <---- 0x80,  // 48 kHz 	//0x40 //8 KHz  0x44  //44.1 KHz
//0xac,                        // <---- 0xBB,				//0x1f			0xAC
//0x00,

/* USB Speaker Audio Type III Format Interface Descriptor */
14,									  /* bLength */
AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
AUDIO_STREAMING_FORMAT_TYPE,          /* bDescriptorSubtype */
AUDIO_FORMAT_TYPE_III,                /* bFormatType */
0x02,                                 /* bNrChannels */  //--> with 2 channels Advanced disappears!!
0x02,                                 /* bSubFrameSize :  2 Bytes per frame (16bits) */
16,                                   /* bBitResolution (16-bits per sample) */
//0x01,                                 /* bSamFreqType only one frequency supported */
//AUDIO_SAMPLE_FREQ(USBD_AUDIO_FREQ),   /* Audio sampling frequency coded on 3 bytes */
/* 11 byte*/
0x02,                                 /* bSamFreqType two frequencies supported */
AUDIO_SAMPLE_FREQ(USBD_AUDIO_FREQ),   /* Audio sampling frequency coded on 3 bytes */
AUDIO_SAMPLE_FREQ(USBD_AUDIO_FREQ2),  /* Audio sampling frequency coded on 3 bytes */
/* 14 byte*/

/* Endpoint Descriptor */
0x09, /* bLength */
0x05, /* bDescriptorType */
AUDIO_IN_EP, /* bEndpointAddress */
0x01, /* bmAttributes */     		  // <---- 0x01, 0x05/0x0D invalid  // isoc, Synchronous //0x01
0xc0, /* wMaxPacketSize */   		  // <---- 0xC0,  // 192 bytes (2 bytes/ch * 2 ch * 48 samples / frame) //0x10
0x00, /* wMaxPacketSize */
0x01, /* bInterval */
0x00, /* bRefresh */
0x00, /* bSynchAddress */

/* Audio Data Endpoint Descriptor */
0x07,
0x25,
0x01,
0x00,
0x00, /* bLockDelayUnits */  		  // <---- 0x02,  // Decoded PCM samples  //0x00
0x00,
0x00
};

/* USB Standard Device Descriptor */
__ALIGN_BEGIN static uint8_t USBD_AUDIO_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC] __ALIGN_END=
{
  USB_LEN_DEV_QUALIFIER_DESC,
  USB_DESC_TYPE_DEVICE_QUALIFIER,
  0x00,
  0x02,
  0x00,
  0x00,
  0x00,
  0x40,
  0x01,
  0x00,
};

uint8_t *USBD_AUDIO_CfgDesc = USBD_AUDIO_CfgDescOut;
int USBD_AUDIO_CfgDescSize = sizeof(USBD_AUDIO_CfgDescOut);

/**
  * @}
  */ 

/** @defgroup USBD_AUDIO_Private_Functions
  * @{
  */ 

static USBD_AUDIO_HandleTypeDef usbd_AUDIO_Handle;

/**
  * @brief  USBD_AUDIO_Init
  *         Initialize the AUDIO interface
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t  USBD_AUDIO_Init (USBD_HandleTypeDef *pdev, 
                                 uint8_t cfgidx)
{
  USBD_AUDIO_HandleTypeDef   *haudio;
  (void)cfgidx;
  
  /* Open EP OUT */
  USBD_LL_OpenEP(pdev,
                 AUDIO_OUT_EP,
                 USBD_EP_TYPE_ISOC,
                 AUDIO_OUT_PACKET);
  
  //XXXX
  /* Open EP In */
  USBD_LL_OpenEP(pdev,
                 AUDIO_IN_EP,
                 USBD_EP_TYPE_ISOC,
                 AUDIO_IN_PACKET);

  /* Allocate Audio structure */
  //pdev->pClassData = USBD_malloc(sizeof (USBD_AUDIO_HandleTypeDef));
  pdev->pClassData = &usbd_AUDIO_Handle;
  
  if(pdev->pClassData == NULL)
  {
    return USBD_FAIL; 
  }
  else
  {
    haudio = (USBD_AUDIO_HandleTypeDef*) pdev->pClassData;
    haudio->alt_setting = 0;
    haudio->offset = AUDIO_OFFSET_UNKNOWN;
    haudio->wr_ptr = 0; 
    haudio->rd_ptr = 0;  
    haudio->rd_enable = 0;
    
    /* Initialize the Audio output Hardware layer */
    if (((USBD_AUDIO_ItfTypeDef *)pdev->pUserData)->Init(USBD_AUDIO_FREQ, /*AUDIO_DEFAULT_VOLUME*/ gCurVolume, 0) != USBD_OK)
    {
      return USBD_FAIL;
    }
    
    /* Prepare Out endpoint to receive 1st packet */ 
    USBD_LL_PrepareReceive(pdev,
                           AUDIO_OUT_EP,
                           haudio->buffer,                        
                           AUDIO_OUT_PACKET);      
  }
  return USBD_OK;
}

/**
  * @brief  USBD_AUDIO_DeInit
  *         DeInitialize the AUDIO layer
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t  USBD_AUDIO_DeInit (USBD_HandleTypeDef *pdev, 
                                   uint8_t cfgidx)
{
  (void)cfgidx;

  /* Close EP OUT */
  USBD_LL_CloseEP(pdev, AUDIO_OUT_EP);

  /* Close EP IN */
  USBD_LL_CloseEP(pdev, AUDIO_IN_EP);

  /* DeInit  physical Interface components */
  if(pdev->pClassData != NULL)
  {
   ((USBD_AUDIO_ItfTypeDef *)pdev->pUserData)->DeInit(0);
    //USBD_free(pdev->pClassData);
    pdev->pClassData = NULL;
  }
  
  return USBD_OK;
}

/**
  * @brief  USBD_AUDIO_Setup
  *         Handle the AUDIO specific requests
  * @param  pdev: instance
  * @param  req: usb requests
  * @retval status
  */
static uint8_t  USBD_AUDIO_Setup (USBD_HandleTypeDef *pdev, 
                                  USBD_SetupReqTypedef *req)
{
  USBD_AUDIO_HandleTypeDef   *haudio;
  uint16_t len;
  uint8_t *pbuf;
  uint8_t ret = USBD_OK;
  haudio = (USBD_AUDIO_HandleTypeDef*) pdev->pClassData;
  
  switch (req->bmRequest & USB_REQ_TYPE_MASK)
  {
  case USB_REQ_TYPE_CLASS :  
    switch (req->bRequest)
    {
    case AUDIO_REQ_GET_CUR:
      AUDIO_REQ_GetCurrent(pdev, req);
      break;
      
    case AUDIO_REQ_SET_CUR:
      AUDIO_REQ_SetCurrent(pdev, req);   
      break;
      
    default:
      USBD_CtlError (pdev, req);
      ret = USBD_FAIL; 
    }
    break;
    
  case USB_REQ_TYPE_STANDARD:
    switch (req->bRequest)
    {
    case USB_REQ_GET_DESCRIPTOR:      
      if( (req->wValue >> 8) == AUDIO_DESCRIPTOR_TYPE)
      {
        pbuf = USBD_AUDIO_CfgDesc + 18;
        len = MIN(USB_AUDIO_DESC_SIZ , req->wLength);
        
        
        USBD_CtlSendData (pdev, 
                          pbuf,
                          len);
      }
      break;
      
    case USB_REQ_GET_INTERFACE :
      USBD_CtlSendData (pdev,
                        (uint8_t *)&(haudio->alt_setting),
                        1);
      break;
      
    case USB_REQ_SET_INTERFACE :
      if ((uint8_t)(req->wValue) <= USBD_MAX_NUM_INTERFACES)
      {
        haudio->alt_setting = (uint8_t)(req->wValue);
      }
      else
      {
        /* Call the error management function (command will be nacked */
        USBD_CtlError (pdev, req);
      }
      break;      
      
    default:
      USBD_CtlError (pdev, req);
      ret = USBD_FAIL;     
    }
  }
  return ret;
}


/**
  * @brief  USBD_AUDIO_GetCfgDesc 
  *         return configuration descriptor
  * @param  speed : current device speed
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
static uint8_t  *USBD_AUDIO_GetCfgDesc (uint16_t *length)
{
  *length = USBD_AUDIO_CfgDescSize;		//sizeof(USBD_AUDIO_CfgDesc);		//XXXX
  return USBD_AUDIO_CfgDesc;
}

#ifdef USB_IN_TEST
uint8_t USBInBuf[AUDIO_IN_PACKET] __attribute__((section("DTCM")));
#else
//use even multipliers!

//Use DDR or DTCM
/*
 * ATT: if DDR is used and it is cached - DMA does not work! (not cache-coherant)
 */
#if 0
#define USB_BUF_ADDR	0xC0200000				//DDR RAM
#define USB_BUF_SIZE	(2*AUDIO_TOTAL_BUF_SIZE)
#else
static uint8_t USBInBuf[2*AUDIO_TOTAL_BUF_SIZE] __attribute__((section("DTCM")));
#define USB_BUF_ADDR	(&USBInBuf[0])
#define USB_BUF_SIZE	(sizeof(USBInBuf))
#endif
#endif

static long unsigned int USBInIdxRd = 0;
static long unsigned int USBInIdxWr = 0;

/**
  * @brief  USBD_AUDIO_DataIn
  *         handle data IN Stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t  USBD_AUDIO_DataIn (USBD_HandleTypeDef *pdev, 
                              	   uint8_t epnum)
{
  (void)epnum;
  USBD_StatusTypeDef stat;

  //USBD_LL_FlushEP(pdev, AUDIO_IN_EP);
#ifndef USB_IN_TEST
  //stat = USBD_LL_Transmit(pdev, AUDIO_IN_EP, (uint8_t*)(&USBInBuf[USBInIdxRd]), AUDIO_IN_PACKET);//length in bytes
  stat = USBD_LL_Transmit(pdev, AUDIO_IN_EP, (uint8_t*)(USB_BUF_ADDR + USBInIdxRd), AUDIO_IN_PACKET);//length in bytes

  USBInIdxRd += AUDIO_IN_PACKET;
#else
  stat = USBD_LL_Transmit(pdev, AUDIO_IN_EP, USBInBuf, AUDIO_IN_PACKET);//length in bytes
#endif

  if (USBInIdxRd == AUDIO_TOTAL_BUF_SIZE)
  {
	  USBD_AUDIO_CpyBuf();
  }

  if (USBInIdxRd >= (2* AUDIO_TOTAL_BUF_SIZE))
  {
	  USBInIdxRd = 0;
	  USBD_AUDIO_CpyBuf();
  }

  //return USBD_OK;
  return (uint8_t)stat;
}

/**
  * @brief  USBD_AUDIO_EP0_RxReady
  *         handle EP0 Rx Ready event
  * @param  pdev: device instance
  * @retval status
  */
static uint8_t  USBD_AUDIO_EP0_RxReady (USBD_HandleTypeDef *pdev)
{
  USBD_AUDIO_HandleTypeDef   *haudio;
  haudio = (USBD_AUDIO_HandleTypeDef*) pdev->pClassData;
  
  if (haudio->control.cmd == AUDIO_REQ_SET_CUR)
  {/* In this driver, to simplify code, only SET_CUR request is managed */

    if (haudio->control.unit == AUDIO_OUT_STREAMING_CTRL)
    {
     ((USBD_AUDIO_ItfTypeDef *)pdev->pUserData)->MuteCtl(haudio->control.data[0]);     
      haudio->control.cmd = 0;
      haudio->control.len = 0;
    }
  } 

  return USBD_OK;
}
/**
  * @brief  USBD_AUDIO_EP0_TxReady
  *         handle EP0 TRx Ready event
  * @param  pdev: device instance
  * @retval status
  */
static uint8_t  USBD_AUDIO_EP0_TxReady (USBD_HandleTypeDef *pdev)
{
  /* Only OUT control data are processed */
  USBD_LL_FlushEP(pdev, AUDIO_IN_EP);	//very important!!!

#ifndef USB_IN_TEST
  USBInIdxRd = 0;
  USBInIdxWr = 0;
  USBD_LL_Transmit(pdev, AUDIO_IN_EP, (uint8_t *)(USB_BUF_ADDR + 0), AUDIO_IN_PACKET);//length in bytes
  memset(USB_BUF_ADDR, 0, USB_BUF_SIZE);		//clean to avoid strange sound
  USBD_AUDIO_CpyBuf();
#else
  int16_t *sineBuf;
  TONEGEN_GetSine(&sineBuf);
  fast_memcpy(USBInBuf, sineBuf, AUDIO_IN_PACKET);
  USBD_LL_Transmit(pdev, AUDIO_IN_EP, USBInBuf, AUDIO_IN_PACKET);//length in bytes
#endif
  return USBD_OK;
}
/**
  * @brief  USBD_AUDIO_SOF
  *         handle SOF event
  * @param  pdev: device instance
  * @retval status
  */
static uint8_t  USBD_AUDIO_SOF (USBD_HandleTypeDef *pdev)
{
  (void)pdev;

  return USBD_OK;
}

/**
  * @brief  USBD_AUDIO_SOF
  *         handle SOF event
  * @param  pdev: device instance
  * @retval status
  */
void  USBD_AUDIO_Sync (USBD_HandleTypeDef *pdev, AUDIO_OffsetTypeDef offset)
{
  int8_t shift = 0;
  USBD_AUDIO_HandleTypeDef   *haudio;
  haudio = (USBD_AUDIO_HandleTypeDef*) pdev->pClassData;
  
  haudio->offset =  offset; 
  
  if(haudio->rd_enable == 1)
  {
    haudio->rd_ptr += AUDIO_TOTAL_BUF_SIZE/2;
    
    if (haudio->rd_ptr == AUDIO_TOTAL_BUF_SIZE)
    {
      /* roll back */
      haudio->rd_ptr = 0;
    }
  }
  
  if(haudio->rd_ptr > haudio->wr_ptr)
  {
    if((long unsigned int)(haudio->rd_ptr - haudio->wr_ptr) < AUDIO_OUT_PACKET)
    {
      shift = -4;
    }
    else if((long unsigned int)(haudio->rd_ptr - haudio->wr_ptr) > (AUDIO_TOTAL_BUF_SIZE - AUDIO_OUT_PACKET))
    {
      shift = 4;
    }    

  }
  else
  {
    if((long unsigned int)(haudio->wr_ptr - haudio->rd_ptr) < AUDIO_OUT_PACKET)
    {
      shift = 4;
    }
    else if((long unsigned int)(haudio->wr_ptr - haudio->rd_ptr) > (AUDIO_TOTAL_BUF_SIZE - AUDIO_OUT_PACKET))
    {
      shift = -4;
    }  
  }

  if(haudio->offset == AUDIO_OFFSET_FULL)
  {
    ((USBD_AUDIO_ItfTypeDef *)pdev->pUserData)->AudioCmd(&haudio->buffer[0],
                                                         AUDIO_TOTAL_BUF_SIZE/2 - shift,
                                                         AUDIO_CMD_PLAY); 
      haudio->offset = AUDIO_OFFSET_NONE;           
  }
}

/**
  * @brief  USBD_AUDIO_IsoINIncomplete
  *         handle data ISO IN Incomplete event
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t  USBD_AUDIO_IsoINIncomplete (USBD_HandleTypeDef *pdev, uint8_t epnum)
{
  (void)epnum;
  (void)pdev;

  return USBD_OK;
}
/**
  * @brief  USBD_AUDIO_IsoOutIncomplete
  *         handle data ISO OUT Incomplete event
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t  USBD_AUDIO_IsoOutIncomplete (USBD_HandleTypeDef *pdev, uint8_t epnum)
{
  (void)epnum;
  (void)pdev;

  return USBD_OK;
}

/**
  * @brief  USBD_AUDIO_DataOut
  *         handle data OUT Stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t  USBD_AUDIO_DataOut (USBD_HandleTypeDef *pdev, 
                              uint8_t epnum)
{
  USBD_AUDIO_HandleTypeDef   *haudio;
  haudio = (USBD_AUDIO_HandleTypeDef*) pdev->pClassData;
  
  if (epnum == AUDIO_OUT_EP)
  {
    /* Increment the Buffer pointer or roll it back when all buffers are full */

	//YYYY
    //haudio->wr_ptr += AUDIO_OUT_PACKET;
	haudio->wr_ptr += EP1PktLen;
    
    if (haudio->wr_ptr >= AUDIO_TOTAL_BUF_SIZE)
    {
    	uint16_t remSamples;
#if 0
    	if(haudio->offset == AUDIO_OFFSET_UNKNOWN)
    	{
    		((USBD_AUDIO_ItfTypeDef *)pdev->pUserData)->AudioCmd(&haudio->buffer[0],
                                                             AUDIO_TOTAL_BUF_SIZE/2,
                                                             AUDIO_CMD_START);
    		haudio->offset = AUDIO_OFFSET_NONE;
    	}
#else
    	AUDIO_PLAYER_CpyBuf(haudio->buffer);
#endif
    	remSamples = haudio->wr_ptr - AUDIO_TOTAL_BUF_SIZE;
    	if (remSamples)
    	{
    		memmove(haudio->buffer, &haudio->buffer[AUDIO_TOTAL_BUF_SIZE], remSamples);
    		haudio->wr_ptr = remSamples;
    	}
    	else
    		haudio->wr_ptr = 0;
    }
    
    if(haudio->rd_enable == 0)
    {
      if (haudio->wr_ptr == (AUDIO_TOTAL_BUF_SIZE / 2))
      {
        haudio->rd_enable = 1; 
      }
    }
    
    /* Prepare Out endpoint to receive next audio packet */
    USBD_LL_PrepareReceive(pdev,
                           AUDIO_OUT_EP,
                           &haudio->buffer[haudio->wr_ptr], 
                           AUDIO_OUT_PACKET);  
      
  }
  
  return USBD_OK;
}

/**
  * @brief  AUDIO_Req_GetCurrent
  *         Handles the GET_CUR Audio control request.
  * @param  pdev: instance
  * @param  req: setup class request
  * @retval status
  */
static void AUDIO_REQ_GetCurrent(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req)
{  
  USBD_AUDIO_HandleTypeDef   *haudio;
  haudio = (USBD_AUDIO_HandleTypeDef*) pdev->pClassData;
  
  memset(haudio->control.data, 0, 64);
  /* Send the current mute state */
  USBD_CtlSendData (pdev, 
                    haudio->control.data,
                    req->wLength);
}

/**
  * @brief  AUDIO_Req_SetCurrent
  *         Handles the SET_CUR Audio control request.
  * @param  pdev: instance
  * @param  req: setup class request
  * @retval status
  */
static void AUDIO_REQ_SetCurrent(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req)
{ 
  USBD_AUDIO_HandleTypeDef   *haudio;
  haudio = (USBD_AUDIO_HandleTypeDef*) pdev->pClassData;
  
  if (req->wLength)
  {
    /* Prepare the reception of the buffer over EP0 */
    USBD_CtlPrepareRx (pdev,
                       haudio->control.data,                                  
                       req->wLength);    
    
    haudio->control.cmd = AUDIO_REQ_SET_CUR;     /* Set the request value */
    haudio->control.len = req->wLength;          /* Set the request data length */
    haudio->control.unit = HIBYTE(req->wIndex);  /* Set the request target unit */
  }
}


/**
* @brief  DeviceQualifierDescriptor 
*         return Device Qualifier descriptor
* @param  length : pointer data length
* @retval pointer to descriptor buffer
*/
static uint8_t  *USBD_AUDIO_GetDeviceQualifierDesc (uint16_t *length)
{
  *length = sizeof (USBD_AUDIO_DeviceQualifierDesc);
  return USBD_AUDIO_DeviceQualifierDesc;
}

/**
* @brief  USBD_AUDIO_RegisterInterface
* @param  fops: Audio interface callback
* @retval status
*/
uint8_t  USBD_AUDIO_RegisterInterface  (USBD_HandleTypeDef   *pdev, 
                                        USBD_AUDIO_ItfTypeDef *fops)
{
  if(fops != NULL)
  {
    pdev->pUserData= fops;
  }
  return 0;
}

#if 0
void USBD_AUDIO_GetBuf(int bufState)
{
	int idx;

	if (bufState == 2)
	{
		//get the active buffer!
		idx = 0;
	}
	else
	{
		//get the active buffer!
		idx = AUDIO_TOTAL_BUF_SIZE;
	}

	//copy into unused buffer
	if (USBInIdxRel < 3)
		//fast_memcpy(&USBInBuf[USBInIdxWr], &BufferCtl.buff[idx], AUDIO_TOTAL_BUF_SIZE);
		fast_memcpy((void *)(USB_BUF_ADDR + USBInIdxWr), &BufferCtl.buff[idx], AUDIO_TOTAL_BUF_SIZE);

	if (USBInIdxRel == 1)
	{
		USBInIdxWr += AUDIO_TOTAL_BUF_SIZE;
		if (USBInIdxWr >= (USB_BUF_SIZE / 2))
			USBInIdxRel = 2;
		return;
	}
	if (USBInIdxRel == 2)
	{
		USBInIdxWr += AUDIO_TOTAL_BUF_SIZE;
		if (USBInIdxWr >= USB_BUF_SIZE)
			USBInIdxWr = 0;
	}
}
#endif

void USBD_AUDIO_CpyBuf(void)
{
	int idx;

	if (BufferCtl.state == BUFFER_OFFSET_FULL)
	{
		//get the inactive buffer!
		idx = AUDIO_TOTAL_BUF_SIZE;
	}
	else
	{
		//get the inactive buffer!
		idx = 0;
	}

	//copy into unused buffer - works on all memories
	//fast_memcpy((void *)(USB_BUF_ADDR + USBInIdxWr), &BufferCtl.buff[idx], AUDIO_TOTAL_BUF_SIZE);
	//works only on cache-coherent memories, DTCM!
	DMA_Mem2Mem_Start((uint32_t)(&BufferCtl.buff[idx]), (uint32_t)(USB_BUF_ADDR + USBInIdxWr), AUDIO_TOTAL_BUF_SIZE / 4);

	USBInIdxWr += AUDIO_TOTAL_BUF_SIZE;
	if (USBInIdxWr >= (2* AUDIO_TOTAL_BUF_SIZE))
		USBInIdxWr = 0;;
}

void USBD_AUDIO_SetMode(int mode)
{
	if ( ! mode)
	{
		//PC USB Audio Out
		USBD_AUDIO_CfgDesc = USBD_AUDIO_CfgDescOut;
		USBD_AUDIO_CfgDescSize = sizeof(USBD_AUDIO_CfgDescOut);

	}
	if (mode == 1)
	{
		//PC USB Audio In
		USBD_AUDIO_CfgDesc = USBD_AUDIO_CfgDescIn;
		USBD_AUDIO_CfgDescSize = sizeof(USBD_AUDIO_CfgDescIn);
	}
	if (mode == 2)
	{
		//MSC device - handled separately
	}
}

/**
  * @}
  */ 


/**
  * @}
  */ 


/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
