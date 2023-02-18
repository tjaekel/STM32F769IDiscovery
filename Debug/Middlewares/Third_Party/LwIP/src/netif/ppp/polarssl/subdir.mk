################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/LwIP/src/netif/ppp/polarssl/arc4.c \
../Middlewares/Third_Party/LwIP/src/netif/ppp/polarssl/des.c \
../Middlewares/Third_Party/LwIP/src/netif/ppp/polarssl/md4.c \
../Middlewares/Third_Party/LwIP/src/netif/ppp/polarssl/md5.c \
../Middlewares/Third_Party/LwIP/src/netif/ppp/polarssl/sha1.c 

OBJS += \
./Middlewares/Third_Party/LwIP/src/netif/ppp/polarssl/arc4.o \
./Middlewares/Third_Party/LwIP/src/netif/ppp/polarssl/des.o \
./Middlewares/Third_Party/LwIP/src/netif/ppp/polarssl/md4.o \
./Middlewares/Third_Party/LwIP/src/netif/ppp/polarssl/md5.o \
./Middlewares/Third_Party/LwIP/src/netif/ppp/polarssl/sha1.o 

C_DEPS += \
./Middlewares/Third_Party/LwIP/src/netif/ppp/polarssl/arc4.d \
./Middlewares/Third_Party/LwIP/src/netif/ppp/polarssl/des.d \
./Middlewares/Third_Party/LwIP/src/netif/ppp/polarssl/md4.d \
./Middlewares/Third_Party/LwIP/src/netif/ppp/polarssl/md5.d \
./Middlewares/Third_Party/LwIP/src/netif/ppp/polarssl/sha1.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/LwIP/src/netif/ppp/polarssl/%.o Middlewares/Third_Party/LwIP/src/netif/ppp/polarssl/%.su: ../Middlewares/Third_Party/LwIP/src/netif/ppp/polarssl/%.c Middlewares/Third_Party/LwIP/src/netif/ppp/polarssl/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DSTM32F769NIHx -DSTM32F769I_DISCO -DSTM32F7 -DSTM32 -DNO_DEBUG -DUSE_HAL_DRIVER -DSTM32F769xx -DINCLUDE_FATFS -DARM_MATH_CM7 -DUSE_USB_HS -D__FPU_PRESENT=1 -D__VFP_FP -DFPU_USED=1 -DNON_DIGI_FP -DSTM_SPDIF_OUT -DUSE_SD_CARD -DNON_USE_DMA2D -DUART_THREAD -c -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@" @"Middlewares/Third_Party/LwIP/src/netif/ppp/polarssl/arc4.c_includes.args"

clean: clean-Middlewares-2f-Third_Party-2f-LwIP-2f-src-2f-netif-2f-ppp-2f-polarssl

clean-Middlewares-2f-Third_Party-2f-LwIP-2f-src-2f-netif-2f-ppp-2f-polarssl:
	-$(RM) ./Middlewares/Third_Party/LwIP/src/netif/ppp/polarssl/arc4.d ./Middlewares/Third_Party/LwIP/src/netif/ppp/polarssl/arc4.o ./Middlewares/Third_Party/LwIP/src/netif/ppp/polarssl/arc4.su ./Middlewares/Third_Party/LwIP/src/netif/ppp/polarssl/des.d ./Middlewares/Third_Party/LwIP/src/netif/ppp/polarssl/des.o ./Middlewares/Third_Party/LwIP/src/netif/ppp/polarssl/des.su ./Middlewares/Third_Party/LwIP/src/netif/ppp/polarssl/md4.d ./Middlewares/Third_Party/LwIP/src/netif/ppp/polarssl/md4.o ./Middlewares/Third_Party/LwIP/src/netif/ppp/polarssl/md4.su ./Middlewares/Third_Party/LwIP/src/netif/ppp/polarssl/md5.d ./Middlewares/Third_Party/LwIP/src/netif/ppp/polarssl/md5.o ./Middlewares/Third_Party/LwIP/src/netif/ppp/polarssl/md5.su ./Middlewares/Third_Party/LwIP/src/netif/ppp/polarssl/sha1.d ./Middlewares/Third_Party/LwIP/src/netif/ppp/polarssl/sha1.o ./Middlewares/Third_Party/LwIP/src/netif/ppp/polarssl/sha1.su

.PHONY: clean-Middlewares-2f-Third_Party-2f-LwIP-2f-src-2f-netif-2f-ppp-2f-polarssl

