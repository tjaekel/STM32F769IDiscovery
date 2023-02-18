################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Utilities/Components/wm8994/wm8994.c 

OBJS += \
./Utilities/Components/wm8994/wm8994.o 

C_DEPS += \
./Utilities/Components/wm8994/wm8994.d 


# Each subdirectory must supply rules for building sources it contributes
Utilities/Components/wm8994/%.o Utilities/Components/wm8994/%.su: ../Utilities/Components/wm8994/%.c Utilities/Components/wm8994/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DSTM32F769NIHx -DSTM32F769I_DISCO -DSTM32F7 -DSTM32 -DNO_DEBUG -DUSE_HAL_DRIVER -DSTM32F769xx -DINCLUDE_FATFS -DARM_MATH_CM7 -DUSE_USB_HS -D__FPU_PRESENT=1 -D__VFP_FP -DFPU_USED=1 -DNON_DIGI_FP -DSTM_SPDIF_OUT -DUSE_SD_CARD -DNON_USE_DMA2D -DUART_THREAD -c -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@" @"Utilities/Components/wm8994/wm8994.c_includes.args"

clean: clean-Utilities-2f-Components-2f-wm8994

clean-Utilities-2f-Components-2f-wm8994:
	-$(RM) ./Utilities/Components/wm8994/wm8994.d ./Utilities/Components/wm8994/wm8994.o ./Utilities/Components/wm8994/wm8994.su

.PHONY: clean-Utilities-2f-Components-2f-wm8994

