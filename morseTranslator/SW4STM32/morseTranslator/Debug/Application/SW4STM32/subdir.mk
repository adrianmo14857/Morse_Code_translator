################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
C:/Users/adria/OneDrive/Pulpit/stm/morseTranslator/SW4STM32/startup_stm32f401xe.s 

C_SRCS += \
C:/Users/adria/OneDrive/Pulpit/stm/morseTranslator/SW4STM32/syscalls.c 

OBJS += \
./Application/SW4STM32/startup_stm32f401xe.o \
./Application/SW4STM32/syscalls.o 

C_DEPS += \
./Application/SW4STM32/syscalls.d 


# Each subdirectory must supply rules for building sources it contributes
Application/SW4STM32/startup_stm32f401xe.o: C:/Users/adria/OneDrive/Pulpit/stm/morseTranslator/SW4STM32/startup_stm32f401xe.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Assembler'
	@echo %cd%
	arm-none-eabi-as -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Application/SW4STM32/syscalls.o: C:/Users/adria/OneDrive/Pulpit/stm/morseTranslator/SW4STM32/syscalls.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DSTM32F401xE -I"C:/Users/adria/OneDrive/Pulpit/stm/morseTranslator/Inc" -I"C:/Users/adria/OneDrive/Pulpit/stm/morseTranslator/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/adria/OneDrive/Pulpit/stm/morseTranslator/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/adria/OneDrive/Pulpit/stm/morseTranslator/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/adria/OneDrive/Pulpit/stm/morseTranslator/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


