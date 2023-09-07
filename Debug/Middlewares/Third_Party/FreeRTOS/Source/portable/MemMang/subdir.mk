################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c 

C_DEPS += \
./Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.d 

OBJS += \
./Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.o 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.o: ../Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG '-DCMSIS_device_header=<stm32g0xx.h>' -DUSE_HAL_DRIVER -DSTM32G0B1xx -c -I../Core/Inc -I../USB_Device/App -I../USB_Device/Target -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0 -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Admin/STM32CubeIDE/workspace_1.6.0/Selex_Battery_Module/selex-fdk/bsp/include" -I"C:/Users/Admin/STM32CubeIDE/workspace_1.6.0/Selex_Battery_Module/selex-fdk/common/include" -I"C:/Users/Admin/STM32CubeIDE/workspace_1.6.0/Selex_Battery_Module/selex-fdk/config" -I"C:/Users/Admin/STM32CubeIDE/workspace_1.6.0/Selex_Battery_Module/selex-fdk/cores/include" -I"C:/Users/Admin/STM32CubeIDE/workspace_1.6.0/Selex_Battery_Module/selex-fdk/libs/canopen-clib" -I"C:/Users/Admin/STM32CubeIDE/workspace_1.6.0/Selex_Battery_Module/selex-fdk/utils/include" -I"C:/Users/Admin/STM32CubeIDE/workspace_1.6.0/Selex_Battery_Module/selex-fdk/thirdparties/cJSON" -I"C:/Users/Admin/STM32CubeIDE/workspace_1.6.0/Selex_Battery_Module/selex-fdk/thirdparties/littlefs" -I"C:/Users/Admin/STM32CubeIDE/workspace_1.6.0/Selex_Battery_Module/SBM/service/assign" -I"C:/Users/Admin/STM32CubeIDE/workspace_1.6.0/Selex_Battery_Module/SBM/service/config" -I"C:/Users/Admin/STM32CubeIDE/workspace_1.6.0/Selex_Battery_Module/SBM/service/sbp" -I"C:/Users/Admin/STM32CubeIDE/workspace_1.6.0/Selex_Battery_Module/SBM/board/porting/include" -I"C:/Users/Admin/STM32CubeIDE/workspace_1.6.0/Selex_Battery_Module/SBM/app/co_app/init" -I"C:/Users/Admin/STM32CubeIDE/workspace_1.6.0/Selex_Battery_Module/SBM/app/co_app/od" -I"C:/Users/Admin/STM32CubeIDE/workspace_1.6.0/Selex_Battery_Module/SBM/app/co_app/od/comm_od" -I"C:/Users/Admin/STM32CubeIDE/workspace_1.6.0/Selex_Battery_Module/SBM/app/co_app/od/manu_od" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

