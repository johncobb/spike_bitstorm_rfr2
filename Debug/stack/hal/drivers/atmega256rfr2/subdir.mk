################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../stack/hal/drivers/atmega256rfr2/halSleep.c \
../stack/hal/drivers/atmega256rfr2/halUart.c 

OBJS += \
./stack/hal/drivers/atmega256rfr2/halSleep.o \
./stack/hal/drivers/atmega256rfr2/halUart.o 

C_DEPS += \
./stack/hal/drivers/atmega256rfr2/halSleep.d \
./stack/hal/drivers/atmega256rfr2/halUart.d 


# Each subdirectory must supply rules for building sources it contributes
stack/hal/drivers/atmega256rfr2/%.o: ../stack/hal/drivers/atmega256rfr2/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"/Users/jcobb/Documents/workspace/spike_rfr2/stack/sys/inc" -I"/Users/jcobb/Documents/workspace/spike_rfr2/stack/apps/BitStorm_LM" -I"/Users/jcobb/Documents/workspace/spike_rfr2/stack/hal/drivers/atmega256rfr2" -I"/Users/jcobb/Documents/workspace/spike_rfr2/stack/hal/atmega256rfr2/inc" -I"/Users/jcobb/Documents/workspace/spike_rfr2/stack/nwk/inc" -I"/Users/jcobb/Documents/workspace/spike_rfr2/stack/phy/atmegarfr2/inc" -I"/Users/jcobb/Documents/workspace/spike_rfr2/stack/service/inc" -DPHY_ATMEGARFR2 -DHAL_ATMEGA256RFR2 -DPLATFORM_BITSTORM_ZB_B -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega256rfr2 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


