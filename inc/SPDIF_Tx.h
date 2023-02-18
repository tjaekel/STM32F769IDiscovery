/**
  ******************************************************************************
  * @file    SPDIF_Tx.h
  * @author  Torsten Jaekel
  * @version V1.1.0
  * @date    04-September-2016
  * @brief   The S/PDIF Tx audio interfaces
  ******************************************************************************
  */

#ifndef SPDIF_TX_H_
#define SPDIF_TX_H_

#include "AudioPlayer.h"

void SPDIF_TX_Init(void);
void SPDIF_TX_Start(void);

void SPDIF_TX_CpyBuf(uint8_t *dst, uint8_t *src, int len);

void SPDIF_TX_TransferComplete_CallBack(void);
void SPDIF_TX_HalfTransfer_CallBack(void);

void SPDIF_TX_OutBuf(void);

#endif /* SPDIF_TX_H_ */
