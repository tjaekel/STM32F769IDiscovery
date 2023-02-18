################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Utilities/Components/st7735/st7735.c 

OBJS += \
./Utilities/Components/st7735/st7735.o 

C_DEPS += \
./Utilities/Components/st7735/st7735.d 


# Each subdirectory must supply rules for building sources it contributes
Utilities/Components/st7735/%.o Utilities/Components/st7735/%.su: ../Utilities/Components/st7735/%.c Utilities/Components/st7735/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DSTM32F769NIHx -DSTM32F769I_DISCO -DSTM32F7 -DSTM32 -DNO_DEBUG -DUSE_HAL_DRIVER -DSTM32F769xx -DINCLUDE_FATFS -DARM_MATH_CM7 -DUSE_USB_HS -D__FPU_PRESENT=1 -D__VFP_FP -DFPU_USED=1 -DNON_DIGI_FP -DSTM_SPDIF_OUT -DUSE_SD_CARD -DNON_USE_DMA2D -DUART_THREAD -c -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@" @"Utilities/Components/st7735/st7735.c_includes.args"

clean: clean-Utilities-2f-Components-2f-st7735

clean-Utilities-2f-Components-2f-st7735:
	-$(RM) ./Utilities/Components/st7735/st7735.d ./Utilities/Components/st7735/st7735.o ./Utilities/Components/st7735/st7735.su

.PHONY: clean-Utilities-2f-Components-2f-st7735

