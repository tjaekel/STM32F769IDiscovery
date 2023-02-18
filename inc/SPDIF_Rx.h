/**
  ******************************************************************************
  * @file    SPDIF_Rx.h
  * @author  Torsten Jaekel
  * @version V1.1.0
  * @date    04-September-2016
  * @brief   The S/PDIF Rx audio interfaces
  ******************************************************************************
  */

#ifndef SPDIF_RX_H_
#define SPDIF_RX_H_

extern SPDIFRX_HandleTypeDef hspdif;

void SPDIF_RX_Msp_Init(void);
void SPDIF_RX_DMA_Init(void);
void SPDIF_RX_Init(void);
int SPDIF_RX_Start(void);

#endif /* SPDIF_RX_H_ */
