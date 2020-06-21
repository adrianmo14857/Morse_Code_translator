################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../i2c-lcd.c 

OBJS += \
./i2c-lcd.o 

C_DEPS += \
./i2c-lcd.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DSTM32F401xE -I"C:/Users/adria/OneDrive/Pulpit/morseTranslator/Inc" -I"C:/Users/adria/OneDrive/Pulpit/morseTranslator/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/adria/OneDrive/Pulpit/morseTranslator/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/adria/OneDrive/Pulpit/morseTranslator/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/adria/OneDrive/Pulpit/morseTranslator/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


