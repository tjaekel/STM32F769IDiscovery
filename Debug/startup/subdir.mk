################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../startup/startup_stm32f769xx.s 

OBJS += \
./startup/startup_stm32f769xx.o 

S_DEPS += \
./startup/startup_stm32f769xx.d 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.s startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -c -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/inc" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/CMSIS/core" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/CMSIS/device" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/HAL_Driver/Inc/Legacy" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/HAL_Driver/Inc" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Middlewares/ST/STM32_Audio/Addons/PDM" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Middlewares/ST/STM32_USB_Device_Library/Class/AUDIO/Inc" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Middlewares/ST/STM32_USB_Host_Library/Class/AUDIO/Inc" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Middlewares/ST/STM32_USB_Host_Library/Core/Inc" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Middlewares/Third_Party/FatFs/src" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Middlewares/Third_Party/FatFs/src/drivers" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Middlewares/Third_Party/FreeRTOS/Source/include" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Middlewares/Third_Party/LwIP/src/include/lwip" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Middlewares/Third_Party/LwIP/src/include/lwip/apps" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Middlewares/Third_Party/LwIP/src/include/lwip/priv" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Middlewares/Third_Party/LwIP/src/include/netif" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Middlewares/Third_Party/LwIP/src/include/netif/ppp" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Middlewares/Third_Party/LwIP/src/include/netif/ppp/polarssl" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Middlewares/Third_Party/LwIP/src/include/posix" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Middlewares/Third_Party/LwIP/src/include/posix/sys" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Middlewares/Third_Party/LwIP/system/arch" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Middlewares/Third_Party/LwIP/system/noOS" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Middlewares/Third_Party/LwIP/system/OS" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Utilities/Components/ampire480272" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Utilities/Components/ampire640480" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Utilities/Components/Common" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Utilities/Components/exc7200" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Utilities/Components/ft5336" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Utilities/Components/ft6x06" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Utilities/Components/mfxstm32l152" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Utilities/Components/mx25l512" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Utilities/Components/n25q128a" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Utilities/Components/n25q512a" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Utilities/Components/otm8009a" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Utilities/Components/ov9655" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Utilities/Components/rk043fn48h" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Utilities/Components/s5k5cag" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Utilities/Components/st7735" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Utilities/Components/stmpe811" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Utilities/Components/ts3510" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Utilities/Components/wm8994" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Utilities/Fonts" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Utilities/Log" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Utilities" -I"C:/Users/tj/Documents/AC6_Workbench/STM32F769IDiscovery/Utilities/STM32F769I-Discovery" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-startup

clean-startup:
	-$(RM) ./startup/startup_stm32f769xx.d ./startup/startup_stm32f769xx.o

.PHONY: clean-startup

