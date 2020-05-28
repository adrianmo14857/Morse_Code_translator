################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/adria/OneDrive/Pulpit/stm/morseTranslator/Src/system_stm32f4xx.c 

OBJS += \
./Drivers/CMSIS/system_stm32f4xx.o 

C_DEPS += \
./Drivers/CMSIS/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/system_stm32f4xx.o: C:/Users/adria/OneDrive/Pulpit/stm/morseTranslator/Src/system_stm32f4xx.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DSTM32F401xE -I"C:/Users/adria/OneDrive/Pulpit/stm/morseTranslator/Inc" -I"C:/Users/adria/OneDrive/Pulpit/stm/morseTranslator/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/adria/OneDrive/Pulpit/stm/morseTranslator/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/adria/OneDrive/Pulpit/stm/morseTranslator/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/adria/OneDrive/Pulpit/stm/morseTranslator/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


