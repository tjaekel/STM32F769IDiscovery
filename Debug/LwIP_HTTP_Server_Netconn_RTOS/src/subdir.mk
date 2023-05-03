################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../LwIP_HTTP_Server_Netconn_RTOS/src/app_ethernet.c \
../LwIP_HTTP_Server_Netconn_RTOS/src/ethernetif.c \
../LwIP_HTTP_Server_Netconn_RTOS/src/fs.c \
../LwIP_HTTP_Server_Netconn_RTOS/src/httpserver-netconn.c \
../LwIP_HTTP_Server_Netconn_RTOS/src/main_http.c 

OBJS += \
./LwIP_HTTP_Server_Netconn_RTOS/src/app_ethernet.o \
./LwIP_HTTP_Server_Netconn_RTOS/src/ethernetif.o \
./LwIP_HTTP_Server_Netconn_RTOS/src/fs.o \
./LwIP_HTTP_Server_Netconn_RTOS/src/httpserver-netconn.o \
./LwIP_HTTP_Server_Netconn_RTOS/src/main_http.o 

C_DEPS += \
./LwIP_HTTP_Server_Netconn_RTOS/src/app_ethernet.d \
./LwIP_HTTP_Server_Netconn_RTOS/src/ethernetif.d \
./LwIP_HTTP_Server_Netconn_RTOS/src/fs.d \
./LwIP_HTTP_Server_Netconn_RTOS/src/httpserver-netconn.d \
./LwIP_HTTP_Server_Netconn_RTOS/src/main_http.d 


# Each subdirectory must supply rules for building sources it contributes
LwIP_HTTP_Server_Netconn_RTOS/src/%.o LwIP_HTTP_Server_Netconn_RTOS/src/%.su LwIP_HTTP_Server_Netconn_RTOS/src/%.cyclo: ../LwIP_HTTP_Server_Netconn_RTOS/src/%.c LwIP_HTTP_Server_Netconn_RTOS/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DSTM32F769NIHx -DSTM32F769I_DISCO -DSTM32F7 -DSTM32 -DNO_DEBUG -DUSE_HAL_DRIVER -DSTM32F769xx -DINCLUDE_FATFS -DARM_MATH_CM7 -DUSE_USB_HS -D__FPU_PRESENT=1 -D__VFP_FP -DFPU_USED=1 -DNO_DIGI_FP -DSTM_SPDIF_OUT -DUSE_SD_CARD -DNO_USE_DMA2D -DUART_THREAD -DTRY_USBH_MIC -DLCD_UPDATE -DMIC24BIT -c -Og -ffunction-sections -Wall -Wextra -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@" @"LwIP_HTTP_Server_Netconn_RTOS/src/app_ethernet.c_includes.args"

clean: clean-LwIP_HTTP_Server_Netconn_RTOS-2f-src

clean-LwIP_HTTP_Server_Netconn_RTOS-2f-src:
	-$(RM) ./LwIP_HTTP_Server_Netconn_RTOS/src/app_ethernet.cyclo ./LwIP_HTTP_Server_Netconn_RTOS/src/app_ethernet.d ./LwIP_HTTP_Server_Netconn_RTOS/src/app_ethernet.o ./LwIP_HTTP_Server_Netconn_RTOS/src/app_ethernet.su ./LwIP_HTTP_Server_Netconn_RTOS/src/ethernetif.cyclo ./LwIP_HTTP_Server_Netconn_RTOS/src/ethernetif.d ./LwIP_HTTP_Server_Netconn_RTOS/src/ethernetif.o ./LwIP_HTTP_Server_Netconn_RTOS/src/ethernetif.su ./LwIP_HTTP_Server_Netconn_RTOS/src/fs.cyclo ./LwIP_HTTP_Server_Netconn_RTOS/src/fs.d ./LwIP_HTTP_Server_Netconn_RTOS/src/fs.o ./LwIP_HTTP_Server_Netconn_RTOS/src/fs.su ./LwIP_HTTP_Server_Netconn_RTOS/src/httpserver-netconn.cyclo ./LwIP_HTTP_Server_Netconn_RTOS/src/httpserver-netconn.d ./LwIP_HTTP_Server_Netconn_RTOS/src/httpserver-netconn.o ./LwIP_HTTP_Server_Netconn_RTOS/src/httpserver-netconn.su ./LwIP_HTTP_Server_Netconn_RTOS/src/main_http.cyclo ./LwIP_HTTP_Server_Netconn_RTOS/src/main_http.d ./LwIP_HTTP_Server_Netconn_RTOS/src/main_http.o ./LwIP_HTTP_Server_Netconn_RTOS/src/main_http.su

.PHONY: clean-LwIP_HTTP_Server_Netconn_RTOS-2f-src

