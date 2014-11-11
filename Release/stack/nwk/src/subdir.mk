################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../stack/nwk/src/nwk.c \
../stack/nwk/src/nwkDataReq.c \
../stack/nwk/src/nwkFrame.c \
../stack/nwk/src/nwkGroup.c \
../stack/nwk/src/nwkRoute.c \
../stack/nwk/src/nwkRouteDiscovery.c \
../stack/nwk/src/nwkRx.c \
../stack/nwk/src/nwkSecurity.c \
../stack/nwk/src/nwkTx.c 

OBJS += \
./stack/nwk/src/nwk.o \
./stack/nwk/src/nwkDataReq.o \
./stack/nwk/src/nwkFrame.o \
./stack/nwk/src/nwkGroup.o \
./stack/nwk/src/nwkRoute.o \
./stack/nwk/src/nwkRouteDiscovery.o \
./stack/nwk/src/nwkRx.o \
./stack/nwk/src/nwkSecurity.o \
./stack/nwk/src/nwkTx.o 

C_DEPS += \
./stack/nwk/src/nwk.d \
./stack/nwk/src/nwkDataReq.d \
./stack/nwk/src/nwkFrame.d \
./stack/nwk/src/nwkGroup.d \
./stack/nwk/src/nwkRoute.d \
./stack/nwk/src/nwkRouteDiscovery.d \
./stack/nwk/src/nwkRx.d \
./stack/nwk/src/nwkSecurity.d \
./stack/nwk/src/nwkTx.d 


# Each subdirectory must supply rules for building sources it contributes
stack/nwk/src/%.o: ../stack/nwk/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"/Users/jcobb/Documents/workspace/spike_bitstorm_rfr2/stack/hal/atmega256rfr2/inc" -I"/Users/jcobb/Documents/workspace/spike_bitstorm_rfr2/stack/apps/BitStorm_LM" -I"/Users/jcobb/Documents/workspace/spike_bitstorm_rfr2/stack/service/inc" -I"/Users/jcobb/Documents/workspace/spike_bitstorm_rfr2/stack/phy/atmegarfr2/inc" -I"/Users/jcobb/Documents/workspace/spike_bitstorm_rfr2/stack/nwk/inc" -I"/Users/jcobb/Documents/workspace/spike_bitstorm_rfr2/stack/sys/inc" -I"/Users/jcobb/Documents/workspace/spike_bitstorm_rfr2/stack/hal/drivers/atmega256rfr2" -DPHY_ATMEGARFR2 -DPLATFORM_BITSTORM_ZB_B -DHAL_ATMEGA256RFR2 -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega256rfr2 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


