# STM32F769IDiscovery
 Audio Project STM32F769I-Discovery

## Features
- LCD Display with VU peak and FFT (VU-Meter and Spectrum Display)
- ARM DSP FFT
- select audio interfaces onboard (MIC, USB In from PC, SPDIF, LineIn, SDCard file ...)
- output any audio source via USB (as USB in on PC)
- prepared for external DIGI-FP module (enable via macro)
- USB host for USB MIC (compile option)
- MIC as onboard digital MICs works: just: enable 44.1 KHz (48 KHz has artefacts)
- Line In "pass-through": get Line In and output on Headphone - works now
- SD Card player: tested with 48 KHz WAV files (it is agnostic to WAV-file information)

## Options
- use external DIGI-FP module (for in/out interfaces, including to
  use and configure the SRC chip on DIGI-FP board - a HW extension)
- "hijack" USB option (used as USB input from PC as source) in order
  to connect an USB MIC (miniDSP UMIK-1 right now only, as 24bit, stereo, 48KHz)
  see macro defined as: TRY_USBH_MIC
  use USBH (host) interface for USB MICs

## Remark
- project has been converted to a STM32CubeIDE project (not AC6, SystemWorkbench anymore)
- in order to use: in addition to the MCU firmware (code, flash the ROM), the LCD images have to be located in on-board QSPI flash:
  a separate file needed to flash the the images for LCD (used as touchscreen), via debugger and external loader!
- internal audio pipeline is 48 KHz or 44.1 KHz, 16bit, stereo: other formats (e.g. from USB MIC as 24bit) are converted

## Tested MICs:
- as USB MIC: digiDSP UMIK-1 - OK
- Rode: VideoMIC NTG: works as USB or as analog in (Line In) - OK

## Known issues:
- onboard mics do not work with 48 KHz sample clock (GUI does not allow to set)
- Line In to Headphone Out is sensitive for volume: there can be a "feedback" (potentially due to DAC WM8994 has analog feedback path)
- all the PLLs were trimmed for most accurate 1 KHz test tone generation


