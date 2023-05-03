################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/AudioPlayer.c \
../src/FFT.c \
../src/Mem2MemDMA.c \
../src/README.c \
../src/SDCard.c \
../src/SDPlay.c \
../src/SPDIF_Rx.c \
../src/SPDIF_Tx.c \
../src/SPKR_BassBoost.c \
../src/SRC_i2c.c \
../src/SRC_i2s_in.c \
../src/Tone_gen.c \
../src/VCP_UART.c \
../src/VUMeter.c \
../src/config.c \
../src/main.c \
../src/stm32f7xx_hal_msp.c \
../src/stm32f7xx_it.c \
../src/syscalls.c \
../src/system_stm32f7xx.c \
../src/uart_cmd.c \
../src/usb_host.c \
../src/usbd_conf_msc.c \
../src/usbd_desc_msc.c \
../src/usbd_storage.c \
../src/usbh_conf.c 

OBJS += \
./src/AudioPlayer.o \
./src/FFT.o \
./src/Mem2MemDMA.o \
./src/README.o \
./src/SDCard.o \
./src/SDPlay.o \
./src/SPDIF_Rx.o \
./src/SPDIF_Tx.o \
./src/SPKR_BassBoost.o \
./src/SRC_i2c.o \
./src/SRC_i2s_in.o \
./src/Tone_gen.o \
./src/VCP_UART.o \
./src/VUMeter.o \
./src/config.o \
./src/main.o \
./src/stm32f7xx_hal_msp.o \
./src/stm32f7xx_it.o \
./src/syscalls.o \
./src/system_stm32f7xx.o \
./src/uart_cmd.o \
./src/usb_host.o \
./src/usbd_conf_msc.o \
./src/usbd_desc_msc.o \
./src/usbd_storage.o \
./src/usbh_conf.o 

C_DEPS += \
./src/AudioPlayer.d \
./src/FFT.d \
./src/Mem2MemDMA.d \
./src/README.d \
./src/SDCard.d \
./src/SDPlay.d \
./src/SPDIF_Rx.d \
./src/SPDIF_Tx.d \
./src/SPKR_BassBoost.d \
./src/SRC_i2c.d \
./src/SRC_i2s_in.d \
./src/Tone_gen.d \
./src/VCP_UART.d \
./src/VUMeter.d \
./src/config.d \
./src/main.d \
./src/stm32f7xx_hal_msp.d \
./src/stm32f7xx_it.d \
./src/syscalls.d \
./src/system_stm32f7xx.d \
./src/uart_cmd.d \
./src/usb_host.d \
./src/usbd_conf_msc.d \
./src/usbd_desc_msc.d \
./src/usbd_storage.d \
./src/usbh_conf.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o src/%.su src/%.cyclo: ../src/%.c src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DSTM32F769NIHx -DSTM32F769I_DISCO -DSTM32F7 -DSTM32 -DNO_DEBUG -DUSE_HAL_DRIVER -DSTM32F769xx -DINCLUDE_FATFS -DARM_MATH_CM7 -DUSE_USB_HS -D__FPU_PRESENT=1 -D__VFP_FP -DFPU_USED=1 -DNO_DIGI_FP -DSTM_SPDIF_OUT -DUSE_SD_CARD -DNO_USE_DMA2D -DUART_THREAD -DTRY_USBH_MIC -DLCD_UPDATE -DMIC24BIT -c -Og -ffunction-sections -Wall -Wextra -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@" @"src/AudioPlayer.c_includes.args"

clean: clean-src

clean-src:
	-$(RM) ./src/AudioPlayer.cyclo ./src/AudioPlayer.d ./src/AudioPlayer.o ./src/AudioPlayer.su ./src/FFT.cyclo ./src/FFT.d ./src/FFT.o ./src/FFT.su ./src/Mem2MemDMA.cyclo ./src/Mem2MemDMA.d ./src/Mem2MemDMA.o ./src/Mem2MemDMA.su ./src/README.cyclo ./src/README.d ./src/README.o ./src/README.su ./src/SDCard.cyclo ./src/SDCard.d ./src/SDCard.o ./src/SDCard.su ./src/SDPlay.cyclo ./src/SDPlay.d ./src/SDPlay.o ./src/SDPlay.su ./src/SPDIF_Rx.cyclo ./src/SPDIF_Rx.d ./src/SPDIF_Rx.o ./src/SPDIF_Rx.su ./src/SPDIF_Tx.cyclo ./src/SPDIF_Tx.d ./src/SPDIF_Tx.o ./src/SPDIF_Tx.su ./src/SPKR_BassBoost.cyclo ./src/SPKR_BassBoost.d ./src/SPKR_BassBoost.o ./src/SPKR_BassBoost.su ./src/SRC_i2c.cyclo ./src/SRC_i2c.d ./src/SRC_i2c.o ./src/SRC_i2c.su ./src/SRC_i2s_in.cyclo ./src/SRC_i2s_in.d ./src/SRC_i2s_in.o ./src/SRC_i2s_in.su ./src/Tone_gen.cyclo ./src/Tone_gen.d ./src/Tone_gen.o ./src/Tone_gen.su ./src/VCP_UART.cyclo ./src/VCP_UART.d ./src/VCP_UART.o ./src/VCP_UART.su ./src/VUMeter.cyclo ./src/VUMeter.d ./src/VUMeter.o ./src/VUMeter.su ./src/config.cyclo ./src/config.d ./src/config.o ./src/config.su ./src/main.cyclo ./src/main.d ./src/main.o ./src/main.su ./src/stm32f7xx_hal_msp.cyclo ./src/stm32f7xx_hal_msp.d ./src/stm32f7xx_hal_msp.o ./src/stm32f7xx_hal_msp.su ./src/stm32f7xx_it.cyclo ./src/stm32f7xx_it.d ./src/stm32f7xx_it.o ./src/stm32f7xx_it.su ./src/syscalls.cyclo ./src/syscalls.d ./src/syscalls.o ./src/syscalls.su ./src/system_stm32f7xx.cyclo ./src/system_stm32f7xx.d ./src/system_stm32f7xx.o ./src/system_stm32f7xx.su ./src/uart_cmd.cyclo ./src/uart_cmd.d ./src/uart_cmd.o ./src/uart_cmd.su ./src/usb_host.cyclo ./src/usb_host.d ./src/usb_host.o ./src/usb_host.su ./src/usbd_conf_msc.cyclo ./src/usbd_conf_msc.d ./src/usbd_conf_msc.o ./src/usbd_conf_msc.su ./src/usbd_desc_msc.cyclo ./src/usbd_desc_msc.d ./src/usbd_desc_msc.o ./src/usbd_desc_msc.su ./src/usbd_storage.cyclo ./src/usbd_storage.d ./src/usbd_storage.o ./src/usbd_storage.su ./src/usbh_conf.cyclo ./src/usbh_conf.d ./src/usbh_conf.o ./src/usbh_conf.su

.PHONY: clean-src

