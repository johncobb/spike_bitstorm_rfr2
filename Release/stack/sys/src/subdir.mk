################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../stack/sys/src/sys.c \
../stack/sys/src/sysEncrypt.c \
../stack/sys/src/sysTimer.c 

OBJS += \
./stack/sys/src/sys.o \
./stack/sys/src/sysEncrypt.o \
./stack/sys/src/sysTimer.o 

C_DEPS += \
./stack/sys/src/sys.d \
./stack/sys/src/sysEncrypt.d \
./stack/sys/src/sysTimer.d 


# Each subdirectory must supply rules for building sources it contributes
stack/sys/src/%.o: ../stack/sys/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"/Users/jcobb/Documents/workspace/spike_bitstorm_rfr2/stack/hal/atmega256rfr2/inc" -I"/Users/jcobb/Documents/workspace/spike_bitstorm_rfr2/stack/apps/BitStorm_LM" -I"/Users/jcobb/Documents/workspace/spike_bitstorm_rfr2/stack/service/inc" -I"/Users/jcobb/Documents/workspace/spike_bitstorm_rfr2/stack/phy/atmegarfr2/inc" -I"/Users/jcobb/Documents/workspace/spike_bitstorm_rfr2/stack/nwk/inc" -I"/Users/jcobb/Documents/workspace/spike_bitstorm_rfr2/stack/sys/inc" -I"/Users/jcobb/Documents/workspace/spike_bitstorm_rfr2/stack/hal/drivers/atmega256rfr2" -DPHY_ATMEGARFR2 -DPLATFORM_BITSTORM_ZB_B -DHAL_ATMEGA256RFR2 -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega256rfr2 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


