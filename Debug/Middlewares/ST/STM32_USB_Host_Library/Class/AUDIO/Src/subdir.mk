################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/ST/STM32_USB_Host_Library/Class/AUDIO/Src/usbh_audio.c 

OBJS += \
./Middlewares/ST/STM32_USB_Host_Library/Class/AUDIO/Src/usbh_audio.o 

C_DEPS += \
./Middlewares/ST/STM32_USB_Host_Library/Class/AUDIO/Src/usbh_audio.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/ST/STM32_USB_Host_Library/Class/AUDIO/Src/%.o Middlewares/ST/STM32_USB_Host_Library/Class/AUDIO/Src/%.su Middlewares/ST/STM32_USB_Host_Library/Class/AUDIO/Src/%.cyclo: ../Middlewares/ST/STM32_USB_Host_Library/Class/AUDIO/Src/%.c Middlewares/ST/STM32_USB_Host_Library/Class/AUDIO/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DSTM32F769NIHx -DSTM32F769I_DISCO -DSTM32F7 -DSTM32 -DNO_DEBUG -DUSE_HAL_DRIVER -DSTM32F769xx -DINCLUDE_FATFS -DARM_MATH_CM7 -DUSE_USB_HS -D__FPU_PRESENT=1 -D__VFP_FP -DFPU_USED=1 -DNO_DIGI_FP -DSTM_SPDIF_OUT -DUSE_SD_CARD -DNO_USE_DMA2D -DUART_THREAD -DTRY_USBH_MIC -DLCD_UPDATE -DMIC24BIT -c -Og -ffunction-sections -Wall -Wextra -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@" @"Middlewares/ST/STM32_USB_Host_Library/Class/AUDIO/Src/usbh_audio.c_includes.args"

clean: clean-Middlewares-2f-ST-2f-STM32_USB_Host_Library-2f-Class-2f-AUDIO-2f-Src

clean-Middlewares-2f-ST-2f-STM32_USB_Host_Library-2f-Class-2f-AUDIO-2f-Src:
	-$(RM) ./Middlewares/ST/STM32_USB_Host_Library/Class/AUDIO/Src/usbh_audio.cyclo ./Middlewares/ST/STM32_USB_Host_Library/Class/AUDIO/Src/usbh_audio.d ./Middlewares/ST/STM32_USB_Host_Library/Class/AUDIO/Src/usbh_audio.o ./Middlewares/ST/STM32_USB_Host_Library/Class/AUDIO/Src/usbh_audio.su

.PHONY: clean-Middlewares-2f-ST-2f-STM32_USB_Host_Library-2f-Class-2f-AUDIO-2f-Src

