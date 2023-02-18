################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/FatFs/src/drivers/sd_diskio.c 

OBJS += \
./Middlewares/Third_Party/FatFs/src/drivers/sd_diskio.o 

C_DEPS += \
./Middlewares/Third_Party/FatFs/src/drivers/sd_diskio.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/FatFs/src/drivers/%.o Middlewares/Third_Party/FatFs/src/drivers/%.su: ../Middlewares/Third_Party/FatFs/src/drivers/%.c Middlewares/Third_Party/FatFs/src/drivers/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DSTM32F769NIHx -DSTM32F769I_DISCO -DSTM32F7 -DSTM32 -DNO_DEBUG -DUSE_HAL_DRIVER -DSTM32F769xx -DINCLUDE_FATFS -DARM_MATH_CM7 -DUSE_USB_HS -D__FPU_PRESENT=1 -D__VFP_FP -DFPU_USED=1 -DNON_DIGI_FP -DSTM_SPDIF_OUT -DUSE_SD_CARD -DNON_USE_DMA2D -DUART_THREAD -c -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@" @"Middlewares/Third_Party/FatFs/src/drivers/sd_diskio.c_includes.args"

clean: clean-Middlewares-2f-Third_Party-2f-FatFs-2f-src-2f-drivers

clean-Middlewares-2f-Third_Party-2f-FatFs-2f-src-2f-drivers:
	-$(RM) ./Middlewares/Third_Party/FatFs/src/drivers/sd_diskio.d ./Middlewares/Third_Party/FatFs/src/drivers/sd_diskio.o ./Middlewares/Third_Party/FatFs/src/drivers/sd_diskio.su

.PHONY: clean-Middlewares-2f-Third_Party-2f-FatFs-2f-src-2f-drivers

