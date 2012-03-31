################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
drivers/gps.obj: ../drivers/gps.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"D:/ti/ccsv5/tools/compiler/msp430/bin/cl430" -vmspx --abi=coffabi -O0 -g --include_path="D:/ti/ccsv5/ccs_base/msp430/include" --include_path="C:/Users/Aviv/workspace_v5_1/GPS Tracker" --include_path="C:/Users/Aviv/workspace_v5_1/GPS Tracker/drivers/fs/src" --include_path="D:/ti/ccsv5/tools/compiler/msp430/include" --define=__MSP430F2618__ --diag_warning=225 --display_error_number --printf_support=minimal --preproc_with_compile --preproc_dependency="drivers/gps.pp" --obj_directory="drivers" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/timer.obj: ../drivers/timer.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"D:/ti/ccsv5/tools/compiler/msp430/bin/cl430" -vmspx --abi=coffabi -O0 -g --include_path="D:/ti/ccsv5/ccs_base/msp430/include" --include_path="C:/Users/Aviv/workspace_v5_1/GPS Tracker" --include_path="C:/Users/Aviv/workspace_v5_1/GPS Tracker/drivers/fs/src" --include_path="D:/ti/ccsv5/tools/compiler/msp430/include" --define=__MSP430F2618__ --diag_warning=225 --display_error_number --printf_support=minimal --preproc_with_compile --preproc_dependency="drivers/timer.pp" --obj_directory="drivers" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/uart.obj: ../drivers/uart.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"D:/ti/ccsv5/tools/compiler/msp430/bin/cl430" -vmspx --abi=coffabi -O0 -g --include_path="D:/ti/ccsv5/ccs_base/msp430/include" --include_path="C:/Users/Aviv/workspace_v5_1/GPS Tracker" --include_path="C:/Users/Aviv/workspace_v5_1/GPS Tracker/drivers/fs/src" --include_path="D:/ti/ccsv5/tools/compiler/msp430/include" --define=__MSP430F2618__ --diag_warning=225 --display_error_number --printf_support=minimal --preproc_with_compile --preproc_dependency="drivers/uart.pp" --obj_directory="drivers" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


