################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CMSIS/DSP_Lib/Source/CommonTables/arm_common_tables.c \
../CMSIS/DSP_Lib/Source/CommonTables/arm_const_structs.c 

OBJS += \
./CMSIS/DSP_Lib/Source/CommonTables/arm_common_tables.o \
./CMSIS/DSP_Lib/Source/CommonTables/arm_const_structs.o 

C_DEPS += \
./CMSIS/DSP_Lib/Source/CommonTables/arm_common_tables.d \
./CMSIS/DSP_Lib/Source/CommonTables/arm_const_structs.d 


# Each subdirectory must supply rules for building sources it contributes
CMSIS/DSP_Lib/Source/CommonTables/%.o CMSIS/DSP_Lib/Source/CommonTables/%.su: ../CMSIS/DSP_Lib/Source/CommonTables/%.c CMSIS/DSP_Lib/Source/CommonTables/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DSTM32F769NIHx -DSTM32F769I_DISCO -DSTM32F7 -DSTM32 -DNO_DEBUG -DUSE_HAL_DRIVER -DSTM32F769xx -DINCLUDE_FATFS -DARM_MATH_CM7 -DUSE_USB_HS -D__FPU_PRESENT=1 -D__VFP_FP -DFPU_USED=1 -DNON_DIGI_FP -DSTM_SPDIF_OUT -DUSE_SD_CARD -DNON_USE_DMA2D -DUART_THREAD -c -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@" @"CMSIS/DSP_Lib/Source/CommonTables/arm_common_tables.c_includes.args"

clean: clean-CMSIS-2f-DSP_Lib-2f-Source-2f-CommonTables

clean-CMSIS-2f-DSP_Lib-2f-Source-2f-CommonTables:
	-$(RM) ./CMSIS/DSP_Lib/Source/CommonTables/arm_common_tables.d ./CMSIS/DSP_Lib/Source/CommonTables/arm_common_tables.o ./CMSIS/DSP_Lib/Source/CommonTables/arm_common_tables.su ./CMSIS/DSP_Lib/Source/CommonTables/arm_const_structs.d ./CMSIS/DSP_Lib/Source/CommonTables/arm_const_structs.o ./CMSIS/DSP_Lib/Source/CommonTables/arm_const_structs.su

.PHONY: clean-CMSIS-2f-DSP_Lib-2f-Source-2f-CommonTables

