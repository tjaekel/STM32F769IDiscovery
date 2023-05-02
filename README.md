# STM32F769IDiscovery
 Audio Project STM32F769I-Discovery
## Features
- LCD Display with VU peak and FFT (VU-Meter and Spectrum Display)
- ARM DSP FFT
- select audio interfaces (MIC, USB, SPDIF, LineIn, SDCard file ...)
- prepared for external DIGI-FP module (enable via macro)

## Options
- use external DIGI-FP module (for in/out interfaces, including to
  use and configure the SRC chip on DIGI-FP board)
- "hijack" USB option (used as USB input from PC as source) in order
  to connect an USB MIC (miniDSP UMIK-1 right now only, as 24bit, stereo, 48KHz)
  see macro defined as: TRY_USBH_MIC
  (it works, but does not display VU-Meter and FFT)

## Remark
- project has been converted to a STM32CubeIDE project (not AC6, SystemWorkbench anymore)
- in order to use: in addition to the MCU firmware (code, flash the ROM), the LCD images have to be located in on-board QSPI flash: a separate file needed to flash the the images for LCD (used as touchscreen), via debugger and external loader!

