################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Utilities/STM32F769I-Discovery/stm32f769i_discovery.c \
../Utilities/STM32F769I-Discovery/stm32f769i_discovery_audio.c \
../Utilities/STM32F769I-Discovery/stm32f769i_discovery_eeprom.c \
../Utilities/STM32F769I-Discovery/stm32f769i_discovery_lcd.c \
../Utilities/STM32F769I-Discovery/stm32f769i_discovery_qspi.c \
../Utilities/STM32F769I-Discovery/stm32f769i_discovery_sd.c \
../Utilities/STM32F769I-Discovery/stm32f769i_discovery_sdram.c \
../Utilities/STM32F769I-Discovery/stm32f769i_discovery_ts.c 

OBJS += \
./Utilities/STM32F769I-Discovery/stm32f769i_discovery.o \
./Utilities/STM32F769I-Discovery/stm32f769i_discovery_audio.o \
./Utilities/STM32F769I-Discovery/stm32f769i_discovery_eeprom.o \
./Utilities/STM32F769I-Discovery/stm32f769i_discovery_lcd.o \
./Utilities/STM32F769I-Discovery/stm32f769i_discovery_qspi.o \
./Utilities/STM32F769I-Discovery/stm32f769i_discovery_sd.o \
./Utilities/STM32F769I-Discovery/stm32f769i_discovery_sdram.o \
./Utilities/STM32F769I-Discovery/stm32f769i_discovery_ts.o 

C_DEPS += \
./Utilities/STM32F769I-Discovery/stm32f769i_discovery.d \
./Utilities/STM32F769I-Discovery/stm32f769i_discovery_audio.d \
./Utilities/STM32F769I-Discovery/stm32f769i_discovery_eeprom.d \
./Utilities/STM32F769I-Discovery/stm32f769i_discovery_lcd.d \
./Utilities/STM32F769I-Discovery/stm32f769i_discovery_qspi.d \
./Utilities/STM32F769I-Discovery/stm32f769i_discovery_sd.d \
./Utilities/STM32F769I-Discovery/stm32f769i_discovery_sdram.d \
./Utilities/STM32F769I-Discovery/stm32f769i_discovery_ts.d 


# Each subdirectory must supply rules for building sources it contributes
Utilities/STM32F769I-Discovery/%.o Utilities/STM32F769I-Discovery/%.su: ../Utilities/STM32F769I-Discovery/%.c Utilities/STM32F769I-Discovery/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DSTM32F769NIHx -DSTM32F769I_DISCO -DSTM32F7 -DSTM32 -DNO_DEBUG -DUSE_HAL_DRIVER -DSTM32F769xx -DINCLUDE_FATFS -DARM_MATH_CM7 -DUSE_USB_HS -D__FPU_PRESENT=1 -D__VFP_FP -DFPU_USED=1 -DNON_DIGI_FP -DSTM_SPDIF_OUT -DUSE_SD_CARD -DNON_USE_DMA2D -DUART_THREAD -c -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@" @"Utilities/STM32F769I-Discovery/stm32f769i_discovery.c_includes.args"

clean: clean-Utilities-2f-STM32F769I-2d-Discovery

clean-Utilities-2f-STM32F769I-2d-Discovery:
	-$(RM) ./Utilities/STM32F769I-Discovery/stm32f769i_discovery.d ./Utilities/STM32F769I-Discovery/stm32f769i_discovery.o ./Utilities/STM32F769I-Discovery/stm32f769i_discovery.su ./Utilities/STM32F769I-Discovery/stm32f769i_discovery_audio.d ./Utilities/STM32F769I-Discovery/stm32f769i_discovery_audio.o ./Utilities/STM32F769I-Discovery/stm32f769i_discovery_audio.su ./Utilities/STM32F769I-Discovery/stm32f769i_discovery_eeprom.d ./Utilities/STM32F769I-Discovery/stm32f769i_discovery_eeprom.o ./Utilities/STM32F769I-Discovery/stm32f769i_discovery_eeprom.su ./Utilities/STM32F769I-Discovery/stm32f769i_discovery_lcd.d ./Utilities/STM32F769I-Discovery/stm32f769i_discovery_lcd.o ./Utilities/STM32F769I-Discovery/stm32f769i_discovery_lcd.su ./Utilities/STM32F769I-Discovery/stm32f769i_discovery_qspi.d ./Utilities/STM32F769I-Discovery/stm32f769i_discovery_qspi.o ./Utilities/STM32F769I-Discovery/stm32f769i_discovery_qspi.su ./Utilities/STM32F769I-Discovery/stm32f769i_discovery_sd.d ./Utilities/STM32F769I-Discovery/stm32f769i_discovery_sd.o ./Utilities/STM32F769I-Discovery/stm32f769i_discovery_sd.su ./Utilities/STM32F769I-Discovery/stm32f769i_discovery_sdram.d ./Utilities/STM32F769I-Discovery/stm32f769i_discovery_sdram.o ./Utilities/STM32F769I-Discovery/stm32f769i_discovery_sdram.su ./Utilities/STM32F769I-Discovery/stm32f769i_discovery_ts.d ./Utilities/STM32F769I-Discovery/stm32f769i_discovery_ts.o ./Utilities/STM32F769I-Discovery/stm32f769i_discovery_ts.su

.PHONY: clean-Utilities-2f-STM32F769I-2d-Discovery

