################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CMSIS/DSP_Lib/Source/TransformFunctions/arm_bitreversal.c \
../CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_f32.c \
../CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_q15.c \
../CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_q31.c \
../CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_f32.c \
../CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_init_f32.c \
../CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_init_q15.c \
../CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_init_q31.c \
../CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_q15.c \
../CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_q31.c \
../CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_f32.c \
../CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_init_f32.c \
../CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_init_q15.c \
../CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_init_q31.c \
../CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_q15.c \
../CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_q31.c \
../CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix8_f32.c \
../CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_f32.c \
../CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_init_f32.c \
../CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_init_q15.c \
../CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_init_q31.c \
../CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_q15.c \
../CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_q31.c \
../CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_f32.c \
../CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_fast_f32.c \
../CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_fast_init_f32.c \
../CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_init_f32.c \
../CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_init_q15.c \
../CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_init_q31.c \
../CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_q15.c \
../CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_q31.c 

S_UPPER_SRCS += \
../CMSIS/DSP_Lib/Source/TransformFunctions/arm_bitreversal2.S 

OBJS += \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_bitreversal.o \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_bitreversal2.o \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_f32.o \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_q15.o \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_q31.o \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_f32.o \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_init_f32.o \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_init_q15.o \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_init_q31.o \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_q15.o \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_q31.o \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_f32.o \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_init_f32.o \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_init_q15.o \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_init_q31.o \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_q15.o \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_q31.o \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix8_f32.o \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_f32.o \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_init_f32.o \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_init_q15.o \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_init_q31.o \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_q15.o \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_q31.o \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_f32.o \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_fast_f32.o \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_fast_init_f32.o \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_init_f32.o \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_init_q15.o \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_init_q31.o \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_q15.o \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_q31.o 

S_UPPER_DEPS += \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_bitreversal2.d 

C_DEPS += \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_bitreversal.d \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_f32.d \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_q15.d \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_q31.d \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_f32.d \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_init_f32.d \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_init_q15.d \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_init_q31.d \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_q15.d \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_q31.d \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_f32.d \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_init_f32.d \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_init_q15.d \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_init_q31.d \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_q15.d \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_q31.d \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix8_f32.d \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_f32.d \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_init_f32.d \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_init_q15.d \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_init_q31.d \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_q15.d \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_q31.d \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_f32.d \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_fast_f32.d \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_fast_init_f32.d \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_init_f32.d \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_init_q15.d \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_init_q31.d \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_q15.d \
./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_q31.d 


# Each subdirectory must supply rules for building sources it contributes
CMSIS/DSP_Lib/Source/TransformFunctions/%.o CMSIS/DSP_Lib/Source/TransformFunctions/%.su: ../CMSIS/DSP_Lib/Source/TransformFunctions/%.c CMSIS/DSP_Lib/Source/TransformFunctions/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DSTM32F769NIHx -DSTM32F769I_DISCO -DSTM32F7 -DSTM32 -DNO_DEBUG -DUSE_HAL_DRIVER -DSTM32F769xx -DINCLUDE_FATFS -DARM_MATH_CM7 -DUSE_USB_HS -D__FPU_PRESENT=1 -D__VFP_FP -DFPU_USED=1 -DNON_DIGI_FP -DSTM_SPDIF_OUT -DUSE_SD_CARD -DNON_USE_DMA2D -DUART_THREAD -c -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@" @"CMSIS/DSP_Lib/Source/TransformFunctions/arm_bitreversal.c_includes.args"
CMSIS/DSP_Lib/Source/TransformFunctions/%.o: ../CMSIS/DSP_Lib/Source/TransformFunctions/%.S CMSIS/DSP_Lib/Source/TransformFunctions/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@" "$<" @"CMSIS/DSP_Lib/Source/TransformFunctions/arm_bitreversal2.S_includes.args"

clean: clean-CMSIS-2f-DSP_Lib-2f-Source-2f-TransformFunctions

clean-CMSIS-2f-DSP_Lib-2f-Source-2f-TransformFunctions:
	-$(RM) ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_bitreversal.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_bitreversal.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_bitreversal.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_bitreversal2.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_bitreversal2.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_f32.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_f32.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_f32.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_q15.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_q15.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_q15.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_q31.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_q31.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_q31.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_f32.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_f32.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_f32.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_init_f32.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_init_f32.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_init_f32.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_init_q15.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_init_q15.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_init_q15.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_init_q31.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_init_q31.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_init_q31.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_q15.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_q15.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_q15.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_q31.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_q31.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_q31.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_f32.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_f32.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_f32.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_init_f32.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_init_f32.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_init_f32.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_init_q15.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_init_q15.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_init_q15.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_init_q31.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_init_q31.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_init_q31.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_q15.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_q15.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_q15.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_q31.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_q31.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_q31.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix8_f32.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix8_f32.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix8_f32.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_f32.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_f32.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_f32.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_init_f32.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_init_f32.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_init_f32.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_init_q15.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_init_q15.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_init_q15.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_init_q31.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_init_q31.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_init_q31.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_q15.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_q15.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_q15.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_q31.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_q31.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_q31.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_f32.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_f32.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_f32.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_fast_f32.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_fast_f32.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_fast_f32.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_fast_init_f32.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_fast_init_f32.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_fast_init_f32.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_init_f32.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_init_f32.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_init_f32.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_init_q15.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_init_q15.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_init_q15.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_init_q31.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_init_q31.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_init_q31.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_q15.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_q15.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_q15.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_q31.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_q31.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_q31.su

.PHONY: clean-CMSIS-2f-DSP_Lib-2f-Source-2f-TransformFunctions

