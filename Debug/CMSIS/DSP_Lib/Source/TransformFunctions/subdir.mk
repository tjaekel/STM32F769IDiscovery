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
CMSIS/DSP_Lib/Source/TransformFunctions/%.o CMSIS/DSP_Lib/Source/TransformFunctions/%.su CMSIS/DSP_Lib/Source/TransformFunctions/%.cyclo: ../CMSIS/DSP_Lib/Source/TransformFunctions/%.c CMSIS/DSP_Lib/Source/TransformFunctions/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DSTM32F769NIHx -DSTM32F769I_DISCO -DSTM32F7 -DSTM32 -DNO_DEBUG -DUSE_HAL_DRIVER -DSTM32F769xx -DINCLUDE_FATFS -DARM_MATH_CM7 -DUSE_USB_HS -D__FPU_PRESENT=1 -D__VFP_FP -DFPU_USED=1 -DNO_DIGI_FP -DSTM_SPDIF_OUT -DUSE_SD_CARD -DNO_USE_DMA2D -DUART_THREAD -DTRY_USBH_MIC -DLCD_UPDATE -DMIC24BIT -c -Og -ffunction-sections -Wall -Wextra -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@" @"CMSIS/DSP_Lib/Source/TransformFunctions/arm_bitreversal.c_includes.args"
CMSIS/DSP_Lib/Source/TransformFunctions/%.o: ../CMSIS/DSP_Lib/Source/TransformFunctions/%.S CMSIS/DSP_Lib/Source/TransformFunctions/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -c -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/inc" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/CMSIS/core" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/CMSIS/device" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/HAL_Driver/Inc/Legacy" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/HAL_Driver/Inc" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Middlewares/ST/STM32_Audio/Addons/PDM" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Middlewares/ST/STM32_USB_Device_Library/Class/AUDIO/Inc" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Middlewares/ST/STM32_USB_Host_Library/Class/AUDIO/Inc" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Middlewares/ST/STM32_USB_Host_Library/Core/Inc" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Middlewares/Third_Party/FatFs/src" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Middlewares/Third_Party/FatFs/src/drivers" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Middlewares/Third_Party/FreeRTOS/Source/include" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Middlewares/Third_Party/LwIP/src/include/lwip" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Middlewares/Third_Party/LwIP/src/include/lwip/apps" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Middlewares/Third_Party/LwIP/src/include/lwip/priv" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Middlewares/Third_Party/LwIP/src/include/netif" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Middlewares/Third_Party/LwIP/src/include/netif/ppp" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Middlewares/Third_Party/LwIP/src/include/netif/ppp/polarssl" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Middlewares/Third_Party/LwIP/src/include/posix" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Middlewares/Third_Party/LwIP/src/include/posix/sys" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Middlewares/Third_Party/LwIP/system/arch" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Middlewares/Third_Party/LwIP/system/noOS" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Middlewares/Third_Party/LwIP/system/OS" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Utilities/Components/ampire480272" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Utilities/Components/ampire640480" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Utilities/Components/Common" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Utilities/Components/exc7200" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Utilities/Components/ft5336" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Utilities/Components/ft6x06" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Utilities/Components/mfxstm32l152" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Utilities/Components/mx25l512" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Utilities/Components/n25q128a" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Utilities/Components/n25q512a" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Utilities/Components/otm8009a" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Utilities/Components/ov9655" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Utilities/Components/rk043fn48h" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Utilities/Components/s5k5cag" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Utilities/Components/st7735" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Utilities/Components/stmpe811" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Utilities/Components/ts3510" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Utilities/Components/wm8994" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Utilities/Fonts" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Utilities/Log" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Utilities" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Utilities/STM32F769I-Discovery" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-CMSIS-2f-DSP_Lib-2f-Source-2f-TransformFunctions

clean-CMSIS-2f-DSP_Lib-2f-Source-2f-TransformFunctions:
	-$(RM) ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_bitreversal.cyclo ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_bitreversal.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_bitreversal.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_bitreversal.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_bitreversal2.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_bitreversal2.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_f32.cyclo ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_f32.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_f32.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_f32.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_q15.cyclo ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_q15.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_q15.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_q15.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_q31.cyclo ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_q31.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_q31.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_q31.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_f32.cyclo ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_f32.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_f32.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_f32.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_init_f32.cyclo ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_init_f32.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_init_f32.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_init_f32.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_init_q15.cyclo ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_init_q15.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_init_q15.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_init_q15.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_init_q31.cyclo ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_init_q31.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_init_q31.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_init_q31.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_q15.cyclo ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_q15.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_q15.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_q15.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_q31.cyclo ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_q31.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_q31.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_q31.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_f32.cyclo ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_f32.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_f32.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_f32.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_init_f32.cyclo ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_init_f32.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_init_f32.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_init_f32.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_init_q15.cyclo ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_init_q15.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_init_q15.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_init_q15.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_init_q31.cyclo ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_init_q31.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_init_q31.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_init_q31.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_q15.cyclo ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_q15.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_q15.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_q15.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_q31.cyclo ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_q31.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_q31.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_q31.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix8_f32.cyclo ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix8_f32.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix8_f32.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix8_f32.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_f32.cyclo ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_f32.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_f32.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_f32.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_init_f32.cyclo ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_init_f32.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_init_f32.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_init_f32.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_init_q15.cyclo ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_init_q15.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_init_q15.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_init_q15.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_init_q31.cyclo ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_init_q31.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_init_q31.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_init_q31.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_q15.cyclo ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_q15.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_q15.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_q15.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_q31.cyclo ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_q31.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_q31.o
	-$(RM) ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_q31.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_f32.cyclo ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_f32.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_f32.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_f32.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_fast_f32.cyclo ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_fast_f32.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_fast_f32.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_fast_f32.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_fast_init_f32.cyclo ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_fast_init_f32.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_fast_init_f32.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_fast_init_f32.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_init_f32.cyclo ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_init_f32.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_init_f32.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_init_f32.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_init_q15.cyclo ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_init_q15.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_init_q15.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_init_q15.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_init_q31.cyclo ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_init_q31.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_init_q31.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_init_q31.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_q15.cyclo ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_q15.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_q15.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_q15.su ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_q31.cyclo ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_q31.d ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_q31.o ./CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_q31.su

.PHONY: clean-CMSIS-2f-DSP_Lib-2f-Source-2f-TransformFunctions

