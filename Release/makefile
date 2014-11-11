################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include stack/sys/src/subdir.mk
-include stack/phy/atmegarfr2/src/subdir.mk
-include stack/phy/atmega256rfr2/src/subdir.mk
-include stack/nwk/src/subdir.mk
-include stack/hal/drivers/atmega256rfr2/subdir.mk
-include stack/hal/atmega256rfr2/src/subdir.mk
-include stack/apps/BitStorm_LM/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
FLASH_IMAGE += \
spike_bitstorm_rfr2.hex \

SIZEDUMMY += \
sizedummy \


# All Target
all: spike_bitstorm_rfr2.elf secondary-outputs

# Tool invocations
spike_bitstorm_rfr2.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,spike_bitstorm_rfr2.map -mmcu=atmega256rfr2 -o "spike_bitstorm_rfr2.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

spike_bitstorm_rfr2.hex: spike_bitstorm_rfr2.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex spike_bitstorm_rfr2.elf  "spike_bitstorm_rfr2.hex"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: spike_bitstorm_rfr2.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega256rfr2 spike_bitstorm_rfr2.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(ASM_DEPS)$(FLASH_IMAGE)$(ELFS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS) spike_bitstorm_rfr2.elf
	-@echo ' '

secondary-outputs: $(FLASH_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
