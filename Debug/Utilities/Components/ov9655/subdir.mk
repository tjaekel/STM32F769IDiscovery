################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Utilities/Components/ov9655/ov9655.c 

OBJS += \
./Utilities/Components/ov9655/ov9655.o 

C_DEPS += \
./Utilities/Components/ov9655/ov9655.d 


# Each subdirectory must supply rules for building sources it contributes
Utilities/Components/ov9655/%.o Utilities/Components/ov9655/%.su Utilities/Components/ov9655/%.cyclo: ../Utilities/Components/ov9655/%.c Utilities/Components/ov9655/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DSTM32F769NIHx -DSTM32F769I_DISCO -DSTM32F7 -DSTM32 -DNO_DEBUG -DUSE_HAL_DRIVER -DSTM32F769xx -DINCLUDE_FATFS -DARM_MATH_CM7 -DUSE_USB_HS -D__FPU_PRESENT=1 -D__VFP_FP -DFPU_USED=1 -DNO_DIGI_FP -DSTM_SPDIF_OUT -DUSE_SD_CARD -DNO_USE_DMA2D -DUART_THREAD -DTRY_USBH_MIC -DLCD_UPDATE -DMIC24BIT -c -Og -ffunction-sections -Wall -Wextra -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@" @"Utilities/Components/ov9655/ov9655.c_includes.args"

clean: clean-Utilities-2f-Components-2f-ov9655

clean-Utilities-2f-Components-2f-ov9655:
	-$(RM) ./Utilities/Components/ov9655/ov9655.cyclo ./Utilities/Components/ov9655/ov9655.d ./Utilities/Components/ov9655/ov9655.o ./Utilities/Components/ov9655/ov9655.su

.PHONY: clean-Utilities-2f-Components-2f-ov9655

