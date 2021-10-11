################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
software/usart/%.obj: ../software/usart/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccs1030/ccs/tools/compiler/ti-cgt-arm_20.2.4.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/Users/certainly/Desktop/msp432_new/LMT70/hardware/LMT70" --include_path="C:/Users/certainly/Desktop/msp432_new/LMT70/software/delay" --include_path="C:/Users/certainly/Desktop/msp432_new/LMT70/software/I2c" --include_path="C:/Users/certainly/Desktop/msp432_new/LMT70/software/usart" --include_path="C:/ti/ccs1030/ccs/ccs_base/arm/include" --include_path="C:/ti/ccs1030/ccs/ccs_base/arm/include/CMSIS" --include_path="C:/Users/certainly/Desktop/msp432_new/LMT70/driverlib/MSP432P4xx" --include_path="C:/Users/certainly/Desktop/msp432_new/LMT70" --include_path="C:/ti/ccs1030/ccs/tools/compiler/ti-cgt-arm_20.2.4.LTS/include" --advice:power=all --define=__MSP432P401R__ --define=ccs -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="software/usart/$(basename $(<F)).d_raw" --obj_directory="software/usart" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


