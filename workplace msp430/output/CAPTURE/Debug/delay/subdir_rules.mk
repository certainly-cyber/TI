################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
delay/%.obj: ../delay/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"D:/ccs/tools/compiler/ti-cgt-arm_20.2.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="D:/workplace msp430/output/CAPTURE/lib" --include_path="D:/workplace msp430/output/CAPTURE/oled" --include_path="D:/workplace msp430/output/CAPTURE/delay" --include_path="D:/workplace msp430/output/CAPTURE/src" --include_path="D:/workplace msp430/output/CAPTURE/driverlib" --include_path="D:/ccs/ccs_base/arm/include" --include_path="D:/ccs/ccs_base/arm/include/CMSIS" --include_path="D:/workplace msp430/output/CAPTURE" --include_path="D:/ccs/tools/compiler/ti-cgt-arm_20.2.1.LTS/include" --advice:power="all" --define=__MSP432P401R__ --define=ccs -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="delay/$(basename $(<F)).d_raw" --obj_directory="delay" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


