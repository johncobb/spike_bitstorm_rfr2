################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../stack/apps/BitStorm_LM/BaseNode.c \
../stack/apps/BitStorm_LM/Coordinator.c \
../stack/apps/BitStorm_LM/EndDevice.c \
../stack/apps/BitStorm_LM/Router.c \
../stack/apps/BitStorm_LM/Sensor_ADC.c \
../stack/apps/BitStorm_LM/at.c \
../stack/apps/BitStorm_LM/eeprom.c \
../stack/apps/BitStorm_LM/main.c 

OBJS += \
./stack/apps/BitStorm_LM/BaseNode.o \
./stack/apps/BitStorm_LM/Coordinator.o \
./stack/apps/BitStorm_LM/EndDevice.o \
./stack/apps/BitStorm_LM/Router.o \
./stack/apps/BitStorm_LM/Sensor_ADC.o \
./stack/apps/BitStorm_LM/at.o \
./stack/apps/BitStorm_LM/eeprom.o \
./stack/apps/BitStorm_LM/main.o 

C_DEPS += \
./stack/apps/BitStorm_LM/BaseNode.d \
./stack/apps/BitStorm_LM/Coordinator.d \
./stack/apps/BitStorm_LM/EndDevice.d \
./stack/apps/BitStorm_LM/Router.d \
./stack/apps/BitStorm_LM/Sensor_ADC.d \
./stack/apps/BitStorm_LM/at.d \
./stack/apps/BitStorm_LM/eeprom.d \
./stack/apps/BitStorm_LM/main.d 


# Each subdirectory must supply rules for building sources it contributes
stack/apps/BitStorm_LM/%.o: ../stack/apps/BitStorm_LM/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"/Users/jcobb/Documents/workspace/spike_rfr2/stack/sys/inc" -I"/Users/jcobb/Documents/workspace/spike_rfr2/stack/apps/BitStorm_LM" -I"/Users/jcobb/Documents/workspace/spike_rfr2/stack/hal/drivers/atmega256rfr2" -I"/Users/jcobb/Documents/workspace/spike_rfr2/stack/hal/atmega256rfr2/inc" -I"/Users/jcobb/Documents/workspace/spike_rfr2/stack/nwk/inc" -I"/Users/jcobb/Documents/workspace/spike_rfr2/stack/phy/atmegarfr2/inc" -I"/Users/jcobb/Documents/workspace/spike_rfr2/stack/service/inc" -DPHY_ATMEGARFR2 -DHAL_ATMEGA256RFR2 -DPLATFORM_BITSTORM_ZB_B -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega256rfr2 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


