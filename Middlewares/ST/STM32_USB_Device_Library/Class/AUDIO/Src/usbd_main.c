/*
 * usbd_main.c
 *
 *  Created on: Mar 14, 2016
 *      Author: Torsten
 */

#include "main.h"

USBD_HandleTypeDef USBD_Device;

int gUSBInterface = 0;
////int gUSBrunning;
int gUSBMode = 0;				//0 is PC USB Out, 1 is PC USB In

void USB_Interface(int mode)
{
	if ( ! gUSBInterface)
	{
		if (mode < 3)
		{
			gUSBMode = mode;
			USBD_AUDIO_SetMode(mode);

#if 0
			//it does not work (yet) for 96KHz
			USBD_AUDIO_Set(mode);
#endif

			gUSBInterface = 1;

			if (mode != 2)
			{
				/* Init Device Library */
				USBD_Init(&USBD_Device, &AUDIO_Desc, 0);

				/* Add Supported Class */
				USBD_RegisterClass(&USBD_Device, USBD_AUDIO_CLASS);

				/* Add Interface call backs for AUDIO Class */
				USBD_AUDIO_RegisterInterface(&USBD_Device, &USBD_AUDIO_fops);
			}
			else
			{
				/* use USB as MSC device to manage files on SD Card */
				/* Init Device Library */
				USBD_Init(&USBD_Device, &MSC_Desc, 0);

				/* Add Supported Class */
				USBD_RegisterClass(&USBD_Device, USBD_MSC_CLASS);

				/* Add Storage callbacks for MSC Class */
				USBD_MSC_RegisterStorage(&USBD_Device, &USBD_DISK_fops);
			}

			/* Start Device Process */
			USBD_Start(&USBD_Device);
		}
		else
		if (mode == 3)
		{
			extern void MX_USB_HOST_Init(void);

			/* needed first to switch USB OTH INT handler! */
			gUSBInterface = 2;

			MX_USB_HOST_Init();
		}
	}
}

void USB_InterfaceDeinit(void)
{
	if (gUSBInterface == 1)
	{
		gUSBInterface = 0;

		USBD_DeInit(&USBD_Device);
	}

	//*** ATT: gUSBInterface = 2 for USBH does not have a Deinit!
}

void USB_ClearBuffer(void)
{
	//if (gUSBInterface)
	//	USBD_AUDIO_ClearBuffer(&USBD_Device);
}
