################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CMSIS/DSP_Lib/Source/ControllerFunctions/arm_pid_init_f32.c \
../CMSIS/DSP_Lib/Source/ControllerFunctions/arm_pid_init_q15.c \
../CMSIS/DSP_Lib/Source/ControllerFunctions/arm_pid_init_q31.c \
../CMSIS/DSP_Lib/Source/ControllerFunctions/arm_pid_reset_f32.c \
../CMSIS/DSP_Lib/Source/ControllerFunctions/arm_pid_reset_q15.c \
../CMSIS/DSP_Lib/Source/ControllerFunctions/arm_pid_reset_q31.c \
../CMSIS/DSP_Lib/Source/ControllerFunctions/arm_sin_cos_f32.c \
../CMSIS/DSP_Lib/Source/ControllerFunctions/arm_sin_cos_q31.c 

OBJS += \
./CMSIS/DSP_Lib/Source/ControllerFunctions/arm_pid_init_f32.o \
./CMSIS/DSP_Lib/Source/ControllerFunctions/arm_pid_init_q15.o \
./CMSIS/DSP_Lib/Source/ControllerFunctions/arm_pid_init_q31.o \
./CMSIS/DSP_Lib/Source/ControllerFunctions/arm_pid_reset_f32.o \
./CMSIS/DSP_Lib/Source/ControllerFunctions/arm_pid_reset_q15.o \
./CMSIS/DSP_Lib/Source/ControllerFunctions/arm_pid_reset_q31.o \
./CMSIS/DSP_Lib/Source/ControllerFunctions/arm_sin_cos_f32.o \
./CMSIS/DSP_Lib/Source/ControllerFunctions/arm_sin_cos_q31.o 

C_DEPS += \
./CMSIS/DSP_Lib/Source/ControllerFunctions/arm_pid_init_f32.d \
./CMSIS/DSP_Lib/Source/ControllerFunctions/arm_pid_init_q15.d \
./CMSIS/DSP_Lib/Source/ControllerFunctions/arm_pid_init_q31.d \
./CMSIS/DSP_Lib/Source/ControllerFunctions/arm_pid_reset_f32.d \
./CMSIS/DSP_Lib/Source/ControllerFunctions/arm_pid_reset_q15.d \
./CMSIS/DSP_Lib/Source/ControllerFunctions/arm_pid_reset_q31.d \
./CMSIS/DSP_Lib/Source/ControllerFunctions/arm_sin_cos_f32.d \
./CMSIS/DSP_Lib/Source/ControllerFunctions/arm_sin_cos_q31.d 


# Each subdirectory must supply rules for building sources it contributes
CMSIS/DSP_Lib/Source/ControllerFunctions/%.o CMSIS/DSP_Lib/Source/ControllerFunctions/%.su CMSIS/DSP_Lib/Source/ControllerFunctions/%.cyclo: ../CMSIS/DSP_Lib/Source/ControllerFunctions/%.c CMSIS/DSP_Lib/Source/ControllerFunctions/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DSTM32F769NIHx -DSTM32F769I_DISCO -DSTM32F7 -DSTM32 -DNO_DEBUG -DUSE_HAL_DRIVER -DSTM32F769xx -DINCLUDE_FATFS -DARM_MATH_CM7 -DUSE_USB_HS -D__FPU_PRESENT=1 -D__VFP_FP -DFPU_USED=1 -DNO_DIGI_FP -DSTM_SPDIF_OUT -DUSE_SD_CARD -DNO_USE_DMA2D -DUART_THREAD -DTRY_USBH_MIC -DLCD_UPDATE -DMIC24BIT -c -Og -ffunction-sections -Wall -Wextra -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@" @"CMSIS/DSP_Lib/Source/ControllerFunctions/arm_pid_init_f32.c_includes.args"

clean: clean-CMSIS-2f-DSP_Lib-2f-Source-2f-ControllerFunctions

clean-CMSIS-2f-DSP_Lib-2f-Source-2f-ControllerFunctions:
	-$(RM) ./CMSIS/DSP_Lib/Source/ControllerFunctions/arm_pid_init_f32.cyclo ./CMSIS/DSP_Lib/Source/ControllerFunctions/arm_pid_init_f32.d ./CMSIS/DSP_Lib/Source/ControllerFunctions/arm_pid_init_f32.o ./CMSIS/DSP_Lib/Source/ControllerFunctions/arm_pid_init_f32.su ./CMSIS/DSP_Lib/Source/ControllerFunctions/arm_pid_init_q15.cyclo ./CMSIS/DSP_Lib/Source/ControllerFunctions/arm_pid_init_q15.d ./CMSIS/DSP_Lib/Source/ControllerFunctions/arm_pid_init_q15.o ./CMSIS/DSP_Lib/Source/ControllerFunctions/arm_pid_init_q15.su ./CMSIS/DSP_Lib/Source/ControllerFunctions/arm_pid_init_q31.cyclo ./CMSIS/DSP_Lib/Source/ControllerFunctions/arm_pid_init_q31.d ./CMSIS/DSP_Lib/Source/ControllerFunctions/arm_pid_init_q31.o ./CMSIS/DSP_Lib/Source/ControllerFunctions/arm_pid_init_q31.su ./CMSIS/DSP_Lib/Source/ControllerFunctions/arm_pid_reset_f32.cyclo ./CMSIS/DSP_Lib/Source/ControllerFunctions/arm_pid_reset_f32.d ./CMSIS/DSP_Lib/Source/ControllerFunctions/arm_pid_reset_f32.o ./CMSIS/DSP_Lib/Source/ControllerFunctions/arm_pid_reset_f32.su ./CMSIS/DSP_Lib/Source/ControllerFunctions/arm_pid_reset_q15.cyclo ./CMSIS/DSP_Lib/Source/ControllerFunctions/arm_pid_reset_q15.d ./CMSIS/DSP_Lib/Source/ControllerFunctions/arm_pid_reset_q15.o ./CMSIS/DSP_Lib/Source/ControllerFunctions/arm_pid_reset_q15.su ./CMSIS/DSP_Lib/Source/ControllerFunctions/arm_pid_reset_q31.cyclo ./CMSIS/DSP_Lib/Source/ControllerFunctions/arm_pid_reset_q31.d ./CMSIS/DSP_Lib/Source/ControllerFunctions/arm_pid_reset_q31.o ./CMSIS/DSP_Lib/Source/ControllerFunctions/arm_pid_reset_q31.su ./CMSIS/DSP_Lib/Source/ControllerFunctions/arm_sin_cos_f32.cyclo ./CMSIS/DSP_Lib/Source/ControllerFunctions/arm_sin_cos_f32.d ./CMSIS/DSP_Lib/Source/ControllerFunctions/arm_sin_cos_f32.o ./CMSIS/DSP_Lib/Source/ControllerFunctions/arm_sin_cos_f32.su ./CMSIS/DSP_Lib/Source/ControllerFunctions/arm_sin_cos_q31.cyclo ./CMSIS/DSP_Lib/Source/ControllerFunctions/arm_sin_cos_q31.d ./CMSIS/DSP_Lib/Source/ControllerFunctions/arm_sin_cos_q31.o ./CMSIS/DSP_Lib/Source/ControllerFunctions/arm_sin_cos_q31.su

.PHONY: clean-CMSIS-2f-DSP_Lib-2f-Source-2f-ControllerFunctions
