################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := D:/ti/ccsv5/tools/compiler/msp430

ORDERED_OBJS += \
$(GEN_CMDS__FLAG) \
"./drivers/fs/ff.obj" \
"./drivers/fs/diskio.obj" \
"./drivers/fs/sd/sd_spi.obj" \
"./drivers/fs/sd/mmc.obj" \
"./drivers/uart.obj" \
"./drivers/timer.obj" \
"./drivers/gps.obj" \
"./main.obj" \
-l"libc.a" \
"../lnk_msp430f2618.cmd" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include drivers/subdir_vars.mk
-include drivers/fs/sd/subdir_vars.mk
-include drivers/fs/subdir_vars.mk
-include subdir_rules.mk
-include drivers/subdir_rules.mk
-include drivers/fs/sd/subdir_rules.mk
-include drivers/fs/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: GPS\ Tracker.out

# Tool invocations
GPS\ Tracker.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: MSP430 Linker'
	"D:/ti/ccsv5/tools/compiler/msp430/bin/cl430" -vmspx --abi=coffabi -O0 -g --define=__MSP430F2618__ --diag_warning=225 --display_error_number --printf_support=minimal -z -m"GPS Tracker.map" --stack_size=80 --heap_size=80 --use_hw_mpy=16 -i"D:/ti/ccsv5/ccs_base/msp430/include" -i"D:/ti/ccsv5/tools/compiler/msp430/lib" -i"D:/ti/ccsv5/tools/compiler/msp430/include" --reread_libs --warn_sections --rom_model -o "GPS Tracker.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(MSP430_EXECUTABLE_OUTPUTS__QUOTED) "GPS Tracker.out"
	-$(RM) "main.pp" "drivers\gps.pp" "drivers\timer.pp" "drivers\uart.pp" "drivers\fs\sd\mmc.pp" "drivers\fs\sd\sd_spi.pp" "drivers\fs\diskio.pp" "drivers\fs\ff.pp" 
	-$(RM) "main.obj" "drivers\gps.obj" "drivers\timer.obj" "drivers\uart.obj" "drivers\fs\sd\mmc.obj" "drivers\fs\sd\sd_spi.obj" "drivers\fs\diskio.obj" "drivers\fs\ff.obj" 
	-@echo 'Finished clean'
	-@echo ' '

post-build:
	-"D:\TAU\TI_CCS5\ccsv5\tools\compiler\msp430\bin\hex430.exe" --ti_txt "GPS Tracker.out" -o GPS.txt -order MS -romwidth 16
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
