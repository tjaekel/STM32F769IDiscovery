################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_abs_f32.c \
../CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_abs_q15.c \
../CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_abs_q31.c \
../CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_abs_q7.c \
../CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_add_f32.c \
../CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_add_q15.c \
../CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_add_q31.c \
../CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_add_q7.c \
../CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_f32.c \
../CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_q15.c \
../CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_q31.c \
../CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_q7.c \
../CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_mult_f32.c \
../CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_mult_q15.c \
../CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_mult_q31.c \
../CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_mult_q7.c \
../CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_negate_f32.c \
../CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_negate_q15.c \
../CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_negate_q31.c \
../CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_negate_q7.c \
../CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_offset_f32.c \
../CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_offset_q15.c \
../CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_offset_q31.c \
../CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_offset_q7.c \
../CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_scale_f32.c \
../CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_scale_q15.c \
../CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_scale_q31.c \
../CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_scale_q7.c \
../CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_shift_q15.c \
../CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_shift_q31.c \
../CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_shift_q7.c \
../CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_sub_f32.c \
../CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_sub_q15.c \
../CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_sub_q31.c \
../CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_sub_q7.c 

OBJS += \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_abs_f32.o \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_abs_q15.o \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_abs_q31.o \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_abs_q7.o \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_add_f32.o \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_add_q15.o \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_add_q31.o \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_add_q7.o \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_f32.o \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_q15.o \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_q31.o \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_q7.o \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_mult_f32.o \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_mult_q15.o \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_mult_q31.o \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_mult_q7.o \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_negate_f32.o \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_negate_q15.o \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_negate_q31.o \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_negate_q7.o \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_offset_f32.o \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_offset_q15.o \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_offset_q31.o \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_offset_q7.o \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_scale_f32.o \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_scale_q15.o \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_scale_q31.o \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_scale_q7.o \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_shift_q15.o \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_shift_q31.o \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_shift_q7.o \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_sub_f32.o \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_sub_q15.o \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_sub_q31.o \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_sub_q7.o 

C_DEPS += \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_abs_f32.d \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_abs_q15.d \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_abs_q31.d \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_abs_q7.d \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_add_f32.d \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_add_q15.d \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_add_q31.d \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_add_q7.d \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_f32.d \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_q15.d \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_q31.d \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_q7.d \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_mult_f32.d \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_mult_q15.d \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_mult_q31.d \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_mult_q7.d \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_negate_f32.d \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_negate_q15.d \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_negate_q31.d \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_negate_q7.d \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_offset_f32.d \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_offset_q15.d \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_offset_q31.d \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_offset_q7.d \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_scale_f32.d \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_scale_q15.d \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_scale_q31.d \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_scale_q7.d \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_shift_q15.d \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_shift_q31.d \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_shift_q7.d \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_sub_f32.d \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_sub_q15.d \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_sub_q31.d \
./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_sub_q7.d 


# Each subdirectory must supply rules for building sources it contributes
CMSIS/DSP_Lib/Source/BasicMathFunctions/%.o CMSIS/DSP_Lib/Source/BasicMathFunctions/%.su CMSIS/DSP_Lib/Source/BasicMathFunctions/%.cyclo: ../CMSIS/DSP_Lib/Source/BasicMathFunctions/%.c CMSIS/DSP_Lib/Source/BasicMathFunctions/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DSTM32F769NIHx -DSTM32F769I_DISCO -DSTM32F7 -DSTM32 -DNO_DEBUG -DUSE_HAL_DRIVER -DSTM32F769xx -DINCLUDE_FATFS -DARM_MATH_CM7 -DUSE_USB_HS -D__FPU_PRESENT=1 -D__VFP_FP -DFPU_USED=1 -DNO_DIGI_FP -DSTM_SPDIF_OUT -DUSE_SD_CARD -DNO_USE_DMA2D -DUART_THREAD -DTRY_USBH_MIC -DLCD_UPDATE -DMIC24BIT -c -Og -ffunction-sections -Wall -Wextra -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@" @"CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_abs_f32.c_includes.args"

clean: clean-CMSIS-2f-DSP_Lib-2f-Source-2f-BasicMathFunctions

clean-CMSIS-2f-DSP_Lib-2f-Source-2f-BasicMathFunctions:
	-$(RM) ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_abs_f32.cyclo ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_abs_f32.d ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_abs_f32.o ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_abs_f32.su ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_abs_q15.cyclo ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_abs_q15.d ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_abs_q15.o ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_abs_q15.su ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_abs_q31.cyclo ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_abs_q31.d ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_abs_q31.o ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_abs_q31.su ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_abs_q7.cyclo ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_abs_q7.d ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_abs_q7.o ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_abs_q7.su ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_add_f32.cyclo ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_add_f32.d ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_add_f32.o ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_add_f32.su ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_add_q15.cyclo ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_add_q15.d ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_add_q15.o ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_add_q15.su ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_add_q31.cyclo ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_add_q31.d ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_add_q31.o ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_add_q31.su ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_add_q7.cyclo ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_add_q7.d ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_add_q7.o ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_add_q7.su ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_f32.cyclo ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_f32.d ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_f32.o ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_f32.su ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_q15.cyclo ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_q15.d ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_q15.o ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_q15.su ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_q31.cyclo ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_q31.d ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_q31.o ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_q31.su ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_q7.cyclo ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_q7.d ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_q7.o ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_q7.su ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_mult_f32.cyclo ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_mult_f32.d ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_mult_f32.o ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_mult_f32.su ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_mult_q15.cyclo ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_mult_q15.d ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_mult_q15.o ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_mult_q15.su ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_mult_q31.cyclo ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_mult_q31.d ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_mult_q31.o ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_mult_q31.su ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_mult_q7.cyclo ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_mult_q7.d ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_mult_q7.o ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_mult_q7.su ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_negate_f32.cyclo ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_negate_f32.d ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_negate_f32.o ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_negate_f32.su ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_negate_q15.cyclo ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_negate_q15.d ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_negate_q15.o ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_negate_q15.su ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_negate_q31.cyclo ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_negate_q31.d ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_negate_q31.o ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_negate_q31.su ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_negate_q7.cyclo ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_negate_q7.d ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_negate_q7.o ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_negate_q7.su ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_offset_f32.cyclo ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_offset_f32.d ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_offset_f32.o ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_offset_f32.su ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_offset_q15.cyclo ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_offset_q15.d ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_offset_q15.o ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_offset_q15.su ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_offset_q31.cyclo ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_offset_q31.d ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_offset_q31.o ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_offset_q31.su ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_offset_q7.cyclo ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_offset_q7.d ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_offset_q7.o ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_offset_q7.su ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_scale_f32.cyclo ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_scale_f32.d ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_scale_f32.o ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_scale_f32.su ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_scale_q15.cyclo
	-$(RM) ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_scale_q15.d ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_scale_q15.o ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_scale_q15.su ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_scale_q31.cyclo ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_scale_q31.d ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_scale_q31.o ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_scale_q31.su ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_scale_q7.cyclo ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_scale_q7.d ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_scale_q7.o ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_scale_q7.su ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_shift_q15.cyclo ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_shift_q15.d ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_shift_q15.o ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_shift_q15.su ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_shift_q31.cyclo ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_shift_q31.d ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_shift_q31.o ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_shift_q31.su ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_shift_q7.cyclo ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_shift_q7.d ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_shift_q7.o ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_shift_q7.su ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_sub_f32.cyclo ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_sub_f32.d ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_sub_f32.o ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_sub_f32.su ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_sub_q15.cyclo ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_sub_q15.d ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_sub_q15.o ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_sub_q15.su ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_sub_q31.cyclo ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_sub_q31.d ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_sub_q31.o ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_sub_q31.su ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_sub_q7.cyclo ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_sub_q7.d ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_sub_q7.o ./CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_sub_q7.su

.PHONY: clean-CMSIS-2f-DSP_Lib-2f-Source-2f-BasicMathFunctions

